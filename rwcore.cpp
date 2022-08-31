
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "rwcore.h"
#include "utils.h"

static RwRGBA ForegroundColor = { 200, 200, 200, 255 };
static RwRGBA BackgroundColor = { 64,  64,  64,   0 };

static RpWorld* World = (RpWorld*)0x00504058;
static RpLight* MainLight = (RpLight*)0x00504054;
static RpLight* AmbientLight = (RpLight*)0x00504050;
static RtCharset* Charset = NULL;

// TODO: rsRWINITIALIZE, rsIDLE
RsEventStatus RsEventHandler(RsEvent event, void *param)
{
	switch (event)
	{
		case rsINPUTDEVICEATTACH:
		{
			AttachInputDevices();

			return rsEVENTPROCESSED;
		}

		case rsRWINITIALIZE:
		{
			Initialize3D(param);

			return rsEVENTPROCESSED;
		}

		case rsIDLE:
		{
			return rsEVENTPROCESSED;
		}

		case rsRWTERMINATE:
		{
			Terminate3D();

			return rsEVENTPROCESSED;
		}

		case rsCAMERASIZE:
		case rsCOMMANDLINE:
		case rsFILELOAD:
		case rsINITDEBUG:
		case rsPLUGINATTACH:
		case rsREGISTERIMAGELOADER:
		case rsSELECTDEVICE:
		case rsINITIALIZE:
		case rsTERMINATE:
		case rsPREINITCOMMANDLINE:
		{
			return ((RsEventStatus (__cdecl*)(RsEvent, void*))0x0041B410)(event, param);
		}
	}

	return rsEVENTERROR;
}

BOOL InitApplication(HANDLE instance)
{
	return ((BOOL (__cdecl*)(HANDLE))0x00431F30)(instance);
}

HWND InitInstance(HANDLE instance)
{
	return ((HWND (__cdecl*)(HANDLE))0x00431FA0)(instance);
}

RwBool RsAlwaysOnTop(RwBool alwaysOnTop)
{
	return ((RwBool (__cdecl*)(RwBool))0x00431AC0)(alwaysOnTop);
}

void RsMouseSetPos(RwV2d *pos)
{
	((void (__cdecl*)(RwV2d*))0x0041B310)(pos);
}

RsEventStatus KeyboardHandler(RsEvent event, void *param)
{
	DebugLog("KeyboardHandler %d\n", event);

	return rsEVENTNOTPROCESSED;
}

RsEventStatus MouseHandler(RsEvent event, void *param)
{
	DebugLog("MouseHandler %d\n", event);

	return rsEVENTNOTPROCESSED;
}

RwBool AttachInputDevices(void)
{
	RsInputDeviceAttach(rsKEYBOARD, KeyboardHandler);

	RsInputDeviceAttach(rsMOUSE, MouseHandler);

	return TRUE;
}

RwBool RsInputDeviceAttach(RsInputDeviceType inputDevice,
	RsInputEventHandler inputEventHandler)
{
	return ((RwBool(__cdecl*)(RsInputDeviceType, RsInputEventHandler))0x0041B340)(inputDevice, inputEventHandler);
}

void RsMouseSetVisibility(RwBool visible)
{
	((void (__cdecl*)(RwBool))0x0041B2F0)(visible);
}

RwBool Initialize3D(void *param)
{
	if( !RsRwInitialize(param) )
    {
		DebugLog("Error initializing RenderWare.\n");

        return FALSE;
    }

	Charset = RtCharsetCreate(&ForegroundColor, &BackgroundColor);
	if (Charset == NULL)
	{
		DebugLog("Cannot create raster charset.");

		return FALSE;
	}

	World = LoadWorld((char*)"levelk/track.bsp");
	if (World == NULL)
	{
		DebugLog("Cannot create world.");

		return FALSE;
	}

	if (!CreateLight(World))
	{
		DebugLog("Failed to create main or ambient light.");

		return FALSE;
	}
}

void Terminate3D()
{
	//RemoveLight(MainLight);
	//RemoveLight(AmbientLight);

	if (Charset)
	{
		RwRasterDestroy(Charset);
	}

	RsRwTerminate();
}

RwBool RsRwInitialize(void *displayID)
{
	return ((RwBool (__cdecl*)(void*))0x0041B6A0)(displayID);
}

RtCharset* RtCharsetCreate(const RwRGBA* foreGround, const RwRGBA* backGround)
{
	return ((RtCharset * (__cdecl*)(const RwRGBA*, const RwRGBA*))0x00498020)(foreGround, backGround);
}

RwBool RwRasterDestroy(RwRaster * raster)
{
	return ((RwBool (__cdecl*)(RwRaster*))0x00456990)(raster);
}

RpWorld* LoadWorld(RwChar* bspFile)
{
	return ((RpWorld * (__cdecl*)(RwChar*))0x00402000)(bspFile);
}

RpLight* CreateLight(RpWorld* world)
{
	return ((RpLight * (__cdecl*)(RpWorld*))0x0041AFF0)(world);
}

void RemoveLight(RpLight* light)
{
	((void (__cdecl*)(RpLight*))0x0041A6C0)(light);
}

void RsRwTerminate()
{
	((void (__cdecl*)())0x0041B680)();
}

RwUInt32 RsTimer(void)
{
	return ((RwUInt32 (__cdecl*)())0x0041B1C0)();
}
