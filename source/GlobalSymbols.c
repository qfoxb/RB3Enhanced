#include <string.h>
#include "rb3/Symbol.h"
#include "GlobalSymbols.h"

GlobalSymbols globalSymbols;

void InitGlobalSymbols()
{
    memset(&globalSymbols, 0, sizeof(globalSymbols));

    SymbolConstruct(&globalSymbols.print_debug, "print_debug");
    SymbolConstruct(&globalSymbols.rb3e_change_music_speed, "rb3e_change_music_speed");
    SymbolConstruct(&globalSymbols.rb3e_change_track_speed, "rb3e_change_track_speed");
    SymbolConstruct(&globalSymbols.rb3e_get_music_speed, "rb3e_get_music_speed");
    SymbolConstruct(&globalSymbols.rb3e_get_track_speed, "rb3e_get_track_speed");
    SymbolConstruct(&globalSymbols.rb3e_set_venue, "rb3e_set_venue");
    SymbolConstruct(&globalSymbols.rb3e_is_emulator, "rb3e_is_emulator");
    SymbolConstruct(&globalSymbols.rb3e_relaunch_game, "rb3e_relaunch_game");

    SymbolConstruct(&globalSymbols.blackBackground, "mod_black_background");
    SymbolConstruct(&globalSymbols.colorShuffle, "mod_color_shuffle");
    SymbolConstruct(&globalSymbols.forceHopos, "mod_force_hopos");
    SymbolConstruct(&globalSymbols.mirrorMode, "mod_mirror_mode");
#ifdef RB3E_WII
    SymbolConstruct(&globalSymbols.useCrossplayNetcode, "mod_use_crossplay_netcode");
#endif

    SymbolConstruct(&globalSymbols.greenGem, "gem_green.wid");
    SymbolConstruct(&globalSymbols.redGem, "gem_red.wid");
    SymbolConstruct(&globalSymbols.yellowGem, "gem_yellow.wid");
    SymbolConstruct(&globalSymbols.blueGem, "gem_blue.wid");
    SymbolConstruct(&globalSymbols.orangeGem, "gem_orange.wid");

    SymbolConstruct(&globalSymbols.redDrumGem, "drum_red.wid");
    SymbolConstruct(&globalSymbols.yellowDrumGem, "drum_yellow.wid");
    SymbolConstruct(&globalSymbols.blueDrumGem, "drum_blue.wid");
    SymbolConstruct(&globalSymbols.greenDrumGem, "drum_green.wid");

    SymbolConstruct(&globalSymbols.redCymbalGem, "cymbal_gem_red.wid");
    SymbolConstruct(&globalSymbols.yellowCymbalGem, "cymbal_gem_yellow.wid");
    SymbolConstruct(&globalSymbols.blueCymbalGem, "cymbal_gem_blue.wid");
    SymbolConstruct(&globalSymbols.greenCymbalGem, "cymbal_gem_green.wid");

    SymbolConstruct(&globalSymbols.greenHopoGem, "gem_green_hopo.wid");
    SymbolConstruct(&globalSymbols.redHopoGem, "gem_red_hopo.wid");
    SymbolConstruct(&globalSymbols.yellowHopoGem, "gem_yellow_hopo.wid");
    SymbolConstruct(&globalSymbols.blueHopoGem, "gem_blue_hopo.wid");
    SymbolConstruct(&globalSymbols.orangeHopoGem, "gem_orange_hopo.wid");

    SymbolConstruct(&globalSymbols.green, "green");
    SymbolConstruct(&globalSymbols.red, "red");
    SymbolConstruct(&globalSymbols.yellow, "yellow");
    SymbolConstruct(&globalSymbols.blue, "blue");
    SymbolConstruct(&globalSymbols.orange, "orange");

    SymbolConstruct(&globalSymbols.overshell_guitar, "overshell_guitar");
    SymbolConstruct(&globalSymbols.overshell_real_guitar, "overshell_real_guitar");
    SymbolConstruct(&globalSymbols.overshell_bass, "overshell_bass");
    SymbolConstruct(&globalSymbols.overshell_real_bass, "overshell_real_bass");
    SymbolConstruct(&globalSymbols.overshell_keys, "overshell_keys");
    SymbolConstruct(&globalSymbols.overshell_real_keys, "overshell_real_keys");
    SymbolConstruct(&globalSymbols.overshell_drums, "overshell_drums");
    SymbolConstruct(&globalSymbols.overshell_drums_pro, "overshell_drums_pro");
    SymbolConstruct(&globalSymbols.overshell_vocal_solo, "overshell_vocal_solo");
    SymbolConstruct(&globalSymbols.overshell_vocal_harmony, "overshell_vocal_harmony");
}
