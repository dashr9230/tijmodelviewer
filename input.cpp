
#include "main.h"

extern RsGlobalType& RsGlobal;

RsEventStatus RsKeyboardEventHandler(RsEvent event, void *param)
{
    if (RsGlobal.keyboard.used)
    {
        return RsGlobal.keyboard.inputEventHandler(event, param);
    }

    return rsEVENTNOTPROCESSED;
}

RsEventStatus RsMouseEventHandler(RsEvent event, void *param)
{
#ifdef RWMOUSE
    /* snoop the mouse move event to update the mouse position for
     * a skeleton drawn cursor
     */
    if (event == rsMOUSEMOVE)
    {
        rsMouseAddDelta(&((RsMouseStatus *) param)->delta);
        rsMouseGetPos(&((RsMouseStatus *) param)->pos);
    }

    if ((event == rsLEFTBUTTONDOWN) || (event == rsRIGHTBUTTONDOWN))
    {
        rsMouseGetPos(&((RsMouseStatus *) param)->pos);
    }
#endif

    if (RsGlobal.mouse.used)
    {
        return RsGlobal.mouse.inputEventHandler(event, param);
    }

    return rsEVENTNOTPROCESSED;
}