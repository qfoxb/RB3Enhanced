/*
    RB3Enhanced - xbox360_content.c
    Helper hooks for Xbox 360 content loading and enumeration.
*/

#ifdef RB3E_XBOX

#include <xtl.h>
#include <stdio.h>
#include "ports.h"
#include "ppcasm.h"
#include "utilities.h"
#include "config.h"
#include "xbox360.h"
#include "rb3enhanced.h"

// TODO(Emma): dynamically allocate and resize this and then free once we're done
#define CONTENT_PRELOAD_MAX_COUNT 4000
#define CONTENT_PRELOAD_SAVEGAME_HANDLE 0x52423373    // 'RB3s'
#define CONTENT_PRELOAD_MARKETPLACE_HANDLE 0x5242336d // 'RB3m'
#define CONTENT_PROGRESS_OVERLAPPED_MAGIC 0x52423370  // 'RB3p'
static XCONTENT_CROSS_TITLE_DATA enumerated_savegame[CONTENT_PRELOAD_MAX_COUNT];
static XCONTENT_CROSS_TITLE_DATA enumerated_marketplace[CONTENT_PRELOAD_MAX_COUNT];
static int loaded_contents_savegame = 0;
static int loaded_contents_marketplace = 0;
static int contents_read_savegame = 0;
static int contents_read_marketplace = 0;
static int preload_finished_savegame = 0;
static int preload_finished_marketplace = 0;

void ContentPreloadThread(void *arg)
{
    HANDLE savegame_enumerator = NULL;
    HANDLE marketplace_enumerator = NULL;
    int r = 0;
    int items_returned = 1; // just need it to make xam happy
    int loop_iter = 0;
    int a = sizeof(enumerated_marketplace);

    // create the enumerators (0xFF = any user)
    if ((r = GameXContentCreateCrossTitleEnumerator(0xFF, 0, 1, 0, 1, NULL, &savegame_enumerator)) != 0)
    {
        RB3E_MSG("Failed to create cross-title savegame enumerator for content preloading! (Error %i)", r);
        savegame_enumerator = NULL;
    }
    if ((r = GameXContentCreateCrossTitleEnumerator(0xFF, 0, 2, 0, 1, NULL, &marketplace_enumerator)) != 0)
    {
        RB3E_MSG("Failed to create cross-title marketplace enumerator for content preloading! (Error %i)", r);
        marketplace_enumerator = NULL;
    }

    while (savegame_enumerator != NULL || marketplace_enumerator != NULL)
    {
        // enumerate through savegames
        if (savegame_enumerator != NULL)
        {
            if (loaded_contents_savegame >= CONTENT_PRELOAD_MAX_COUNT)
            {
                RB3E_MSG("Content preload on savegames has exceeded max count (%i)! Preloading has been disabled.", loaded_contents_savegame);
                CloseHandle(savegame_enumerator);
                savegame_enumerator = NULL;
                loaded_contents_savegame = 0;
            }
            else
            {
                r = GameXEnumerateCrossTitle(savegame_enumerator, &enumerated_savegame[loaded_contents_savegame], sizeof(XCONTENT_CROSS_TITLE_DATA), &items_returned, NULL);
                if (r != 0)
                {
                    RB3E_DEBUG("Finished enumerating savegame contents with error code %i.", r);
                    CloseHandle(savegame_enumerator);
                    savegame_enumerator = NULL;
                    preload_finished_savegame = 1;
                }
                loaded_contents_savegame += items_returned;
            }
        }
        // enumerate through marketplace items
        if (marketplace_enumerator != NULL)
        {
            if (loaded_contents_marketplace >= CONTENT_PRELOAD_MAX_COUNT)
            {
                RB3E_MSG("Content preload on marketplace has exceeded max count (%i)! Preloading has been disabled.", loaded_contents_marketplace);
                CloseHandle(marketplace_enumerator);
                marketplace_enumerator = NULL;
                loaded_contents_marketplace = 0;
            }
            else
            {
                r = GameXEnumerateCrossTitle(marketplace_enumerator, &enumerated_marketplace[loaded_contents_marketplace], sizeof(XCONTENT_CROSS_TITLE_DATA), &items_returned, NULL);
                if (r != 0)
                {
                    RB3E_DEBUG("Finished enumerating marketplace contents with r = %i.", r);
                    CloseHandle(marketplace_enumerator);
                    marketplace_enumerator = NULL;
                    preload_finished_marketplace = 1;
                }
                loaded_contents_marketplace += items_returned;
            }
        }
        if (loop_iter % 10 == 0) // don't sleep too much
            Sleep(1);
        loop_iter++;
    }

    RB3E_MSG("Content preload finished! Enumerated %i savegame packages, %i marketplace packages.", loaded_contents_savegame, loaded_contents_marketplace);
}

int GameXContentCreateCrossTitleEnumeratorHook(int user_index, int device_id, int content_type, int flags, int item_count, void *callback_ptr, HANDLE *enum_handle_ptr)
{
    int r = 0;
    if (user_index == 0xFF && content_type == 1 && preload_finished_savegame == 1)
    {
        RB3E_DEBUG("Game starting savegame enumeration, returning content preload", NULL);
        *enum_handle_ptr = (HANDLE)CONTENT_PRELOAD_SAVEGAME_HANDLE;
        r = 0;
    }
    else if (user_index == 0xFF && content_type == 2 && preload_finished_marketplace == 1)
    {
        RB3E_DEBUG("Game starting marketplace enumeration, returning content preload", NULL);
        *enum_handle_ptr = (HANDLE)CONTENT_PRELOAD_MARKETPLACE_HANDLE;
        r = 0;
    }
    else
    {
        r = GameXContentCreateCrossTitleEnumerator(user_index, device_id, content_type, flags, item_count, callback_ptr, enum_handle_ptr);
    }
    RB3E_DEBUG("Game enumerating user index %i, content type %i, return code = %i", user_index, content_type, r);
    return r;
}

int GameXEnumerateCrossTitleHook(HANDLE enum_handle, XCONTENT_CROSS_TITLE_DATA *cross_title_data, int size_cross_title_data, int *items_returned, void *overlapped)
{
    if (enum_handle == (HANDLE)CONTENT_PRELOAD_SAVEGAME_HANDLE && preload_finished_savegame == 1 && overlapped != NULL)
    {
        if (contents_read_savegame >= loaded_contents_savegame)
        {
            // we're done loading, no more items
            RB3E_DEBUG("Game finished savegame enumeration, resetting content preload", NULL);
            preload_finished_savegame = 0; // if the game ever tries refreshing again, let it do a fresh one
            return 0x103;
        }
        else
        {
            // copy the data from our enumeration array into the game.
            memcpy(cross_title_data, &enumerated_savegame[contents_read_savegame], sizeof(XCONTENT_CROSS_TITLE_DATA));
            // RB3E_DEBUG("Enumerating savegame item %i (filename %.42s)", contents_read_savegame, cross_title_data->szFileName);
            if (items_returned != NULL)
                *items_returned = 1;
            contents_read_savegame++;
            if (overlapped != NULL)
                *(int *)overlapped = CONTENT_PROGRESS_OVERLAPPED_MAGIC;
        }
        // the game expects IO pending because it's trying to do an overlapped operation
        return 0x3E5;
    }
    else if (enum_handle == (HANDLE)CONTENT_PRELOAD_MARKETPLACE_HANDLE && preload_finished_marketplace == 1)
    {
        if (contents_read_marketplace >= loaded_contents_marketplace)
        {
            // we're done loading, no more items
            RB3E_DEBUG("Game finished marketplace enumeration, resetting content preload", NULL);
            preload_finished_marketplace = 0; // if the game ever tries refreshing again, let it do a fresh one
            return 0x103;
        }
        else
        {
            // copy the data from our enumeration array into the game.
            memcpy(cross_title_data, &enumerated_marketplace[contents_read_marketplace], sizeof(XCONTENT_CROSS_TITLE_DATA));
            // RB3E_DEBUG("Enumerating marketplace item %i (filename %.42s)", contents_read_savegame, cross_title_data->szFileName);
            if (items_returned != NULL)
                *items_returned = 1;
            contents_read_marketplace++;
            if (overlapped != NULL)
                *(int *)overlapped = CONTENT_PROGRESS_OVERLAPPED_MAGIC;
        }
        // the game expects IO pending because it's trying to do an overlapped operation
        return 0x3E5;
    }
    // it's not for us, so don't mess with it
    return GameXEnumerateCrossTitle(enum_handle, cross_title_data, size_cross_title_data, items_returned, overlapped);
}

int OverlappedGetContentResultPatch(void *overlapped, int *result, int wait)
{
    if (overlapped != NULL && *(int *)overlapped == CONTENT_PROGRESS_OVERLAPPED_MAGIC)
    {
        *result = 1;
        return 0;
    }
    return XGetOverlappedResult(overlapped, result, wait);
}

void InitContentPreload()
{
    POKE_BL(0x82520c68, OverlappedGetContentResultPatch);
    HookFunction(0x8283EF70, &GameXContentCreateCrossTitleEnumerator, &GameXContentCreateCrossTitleEnumeratorHook);
    HookFunction(0x8283F340, &GameXEnumerateCrossTitle, &GameXEnumerateCrossTitleHook);
    RB3E_CreateThread(ContentPreloadThread, NULL, 0x4000);
}

#endif
