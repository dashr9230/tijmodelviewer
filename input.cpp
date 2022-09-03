
#include "main.h"

RwBool Forwards = FALSE;
RwBool Backwards = FALSE;
RwBool StrafeLeft = FALSE;
RwBool StrafeRight = FALSE;

RwReal CameraPitchRate = 0.0f;
RwReal CameraTurnRate = 0.0f;
RwReal CameraSpeed = 0.0f;
RwReal CameraStrafeSpeed = 0.0f;

RwBool spin = FALSE;
RwBool boost = FALSE;

extern RsGlobalType& RsGlobal;

RsEventStatus HandleKeyDown(RsKeyStatus* keyStatus)
{
	switch (keyStatus->keyCharCode)
	{
	case rsUP:
	case 119: // W
	{
		CameraSpeed = boost ? (CAMERASPEED + BOOSTSPEED) : CAMERASPEED;
		Forwards = TRUE;

		return rsEVENTPROCESSED;
	}

	case rsDOWN:
	case 115: // S
	{
		CameraSpeed = boost ? -(CAMERASPEED + BOOSTSPEED) : -CAMERASPEED;
		Backwards = TRUE;

		return rsEVENTPROCESSED;
	}

	case rsLEFT:
	case 97: // A
	{
		CameraStrafeSpeed = boost ? (CAMERASPEED + BOOSTSPEED) : CAMERASPEED;
		StrafeLeft = TRUE;
		return rsEVENTPROCESSED;
	}

	case rsRIGHT:
	case 100: // D
	{
		CameraStrafeSpeed = boost ? -(CAMERASPEED + BOOSTSPEED) : -CAMERASPEED;
		StrafeRight = TRUE;
		return rsEVENTPROCESSED;
	}

	case rsLSHIFT:
	case rsRSHIFT:
	{
		boost = ~boost;

		return rsEVENTPROCESSED;
	}

	default:
	{
		return rsEVENTNOTPROCESSED;
	}
	}
}

RsEventStatus HandleKeyUp(RsKeyStatus* keyStatus)
{
	switch (keyStatus->keyCharCode)
	{
	case rsUP:
	case 119: // W
	{
		Forwards = FALSE;
		CameraSpeed = Backwards ? -CAMERASPEED : 0.0f;

		return rsEVENTPROCESSED;
	}

	case rsDOWN:
	case 115: // S
	{
		Backwards = FALSE;
		CameraSpeed = Forwards ? CAMERASPEED : 0.0f;

		return rsEVENTPROCESSED;
	}

	case rsLEFT:
	case 97: // A
	{
		StrafeLeft = FALSE;
		CameraStrafeSpeed = StrafeRight ? -CAMERASPEED : 0.0f;
		return rsEVENTPROCESSED;
	}

	case rsRIGHT:
	case 100: // D
	{
		StrafeRight = FALSE;
		CameraStrafeSpeed = StrafeLeft ? CAMERASPEED : 0.0f;

		return rsEVENTPROCESSED;
	}

	default:
	{
		return rsEVENTNOTPROCESSED;
	}
	}
}

RsEventStatus KeyboardHandler(RsEvent event, void* param)
{
    //Log("KeyboardHandler %d", param);

    switch (event)
    {
    case rsKEYDOWN:
    {
        return HandleKeyDown((RsKeyStatus*)param);
    }

    case rsKEYUP:
    {
        return HandleKeyUp((RsKeyStatus*)param);
    }

    default:
    {
        return rsEVENTNOTPROCESSED;
    }
    }

    return rsEVENTNOTPROCESSED;
}

RsEventStatus HandleLeftButtonDown(RsMouseStatus* mouseStatus)
{
    spin = TRUE;

    return rsEVENTPROCESSED;
}

RsEventStatus HandleLeftButtonUp(RsMouseStatus* mouseStatus)
{
    spin = FALSE;

    return rsEVENTPROCESSED;
}

RsEventStatus HandleMouseMove(RsMouseStatus* mouseStatus)
{
    if (spin)
    {
        CameraLook(mouseStatus->delta.x, mouseStatus->delta.y);
    }

    return rsEVENTPROCESSED;
}

RsEventStatus MouseHandler(RsEvent event, void* param)
{
	//Log("MouseHandler %d", event);

	switch (event)
	{
	case rsLEFTBUTTONDOWN:
	{
		return HandleLeftButtonDown((RsMouseStatus*)param);
	}

	case rsLEFTBUTTONUP:
	{
		return HandleLeftButtonUp((RsMouseStatus*)param);
	}

	case rsMOUSEMOVE:
	{
		return HandleMouseMove((RsMouseStatus*)param);
	}

	default:
	{
		return rsEVENTNOTPROCESSED;
	}
	}

	return rsEVENTNOTPROCESSED;
}

RwBool AttachInputDevices(void)
{
	RsInputDeviceAttach(rsKEYBOARD, KeyboardHandler);

	RsInputDeviceAttach(rsMOUSE, MouseHandler);

	return TRUE;
}
