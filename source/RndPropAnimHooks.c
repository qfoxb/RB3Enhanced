#include "rb3e_include.h"
#include "RndPropAnimHooks.h"

void PropAnimSetFrameHook(RndPropAnim *rndPropAnim, float frame, float time)
{
    if (strcmp(rndPropAnim->object.name, "slot_positions.anim") == 0)
    {
        // check that it is not setting the frame to 0 (which is the default slot positions for overshell)
        if (frame != 0.0f)
        {
            // set the frame to 2 so pause menus will appear when playing on the wrong instrument type
            // TODO: Get rid of this hook and figure out how to properly fix this
            PropAnimSetFrame(rndPropAnim, 2.0f, 1.0f);
            return;
        }
    }
    PropAnimSetFrame(rndPropAnim, frame, time);
}