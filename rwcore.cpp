
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "rwcore.h"
#include "utils.h"

RwRGBA ForegroundColor = { 200, 200, 200, 255 };
RwRGBA BackgroundColor = { 64,  64,  64,   0 };

RtCharset** Charset = (RtCharset**)0x004E5BF0;
RwCamera** Camera = (RwCamera**)0x00504520;
RpWorld** World = (RpWorld**)0x00504058;
RpLight** MainLight = (RpLight**)0x00504054;
RpLight** AmbientLight = (RpLight**)0x00504050;

RwBool Initialize3D(void *param)
{
	if (!RsRwInitialize(param))
	{
		Log("Error initializing RenderWare.");

		return FALSE;
	}

	*Charset = RtCharsetCreate(&ForegroundColor, &BackgroundColor);
	if (Charset == NULL)
	{
		Log("Cannot create raster charset.");

		return FALSE;
	}

	*World = LoadWorld("levelk/track.bsp");
	if (World == NULL)
	{
		Log("Cannot create world.");

		return FALSE;
	}

	*Camera = CreateCamera();
	if (Camera == NULL)
	{
		Log("Failed to create camera.");

		return FALSE;
	}

	RpWorldAddCamera(*World, *Camera);

	if (!CreateLight(*World))
	{
		Log("Failed to create main or ambient light.");
		return FALSE;
	}

	return TRUE;
}

RsEventStatus KeyboardHandler(RsEvent event, void *param)
{
	Log("KeyboardHandler %d", event);

	return rsEVENTNOTPROCESSED;
}

RsEventStatus MouseHandler(RsEvent event, void *param)
{
	Log("MouseHandler %d", event);

	return rsEVENTNOTPROCESSED;
}

RwBool AttachInputDevices(void)
{
	RsInputDeviceAttach(rsKEYBOARD, KeyboardHandler);

	RsInputDeviceAttach(rsMOUSE, MouseHandler);

	return TRUE;
}

void Idle()
{
	//((void (__cdecl*)())0x004194A0)(); // Idle()

	RwCameraClear(*Camera, &BackgroundColor, 3);

	if (RwCameraBeginUpdate(*Camera))
	{
		RpWorldRender(*World);

		RwCameraEndUpdate(*Camera);
	}

	RsCameraShowRaster(*Camera);
}

void Terminate3D()
{
	RemoveLight(*MainLight);
	RemoveLight(*AmbientLight);

	if (*Camera)
	{
		RpWorldRemoveCamera(*World, *Camera);
		CameraDestroy(*Camera);
	}

	if (*Charset)
	{
		RwRasterDestroy(*Charset);
	}

	RsRwTerminate();
}

RsEventStatus RsEventHandler(RsEvent event, void *param)
{
	switch (event)
	{
		case rsRWINITIALIZE:
		{
			return Initialize3D(param) ? rsEVENTPROCESSED : rsEVENTERROR;
		}

		case rsIDLE:
		{
			Idle();

			return rsEVENTPROCESSED;
		}

		case rsRWTERMINATE:
		{
			Terminate3D();

			return rsEVENTPROCESSED;
		}

		default:
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

RwBool RsInputDeviceAttach(RsInputDeviceType inputDevice, RsInputEventHandler inputEventHandler)
{
	return ((RwBool (__cdecl*)(RsInputDeviceType, RsInputEventHandler))0x0041B340)(inputDevice, inputEventHandler);
}

RwBool RsAlwaysOnTop(RwBool alwaysOnTop)
{
	return ((RwBool (__cdecl*)(RwBool))0x00431AC0)(alwaysOnTop);
}

void RsMouseSetPos(RwV2d *pos)
{
	((void (__cdecl*)(RwV2d*))0x0041B310)(pos);
}

RwBool RsRwInitialize(void *displayID)
{
	return ((RwBool (__cdecl*)(void*))0x0041B6A0)(displayID);
}

void RsRwTerminate()
{
	((void (__cdecl*)())0x0041B680)();
}

RtCharset* RtCharsetCreate(const RwRGBA* foreGround, const RwRGBA* backGround)
{
	return ((RtCharset * (__cdecl*)(const RwRGBA*, const RwRGBA*))0x00498020)(foreGround, backGround);
}

RwBool RwRasterDestroy(RwRaster * raster)
{
	return ((RwBool (__cdecl*)(RwRaster*))0x00456990)(raster);
}

RwCamera* RwCameraClear(RwCamera* camera, RwRGBA* colour, RwInt32 clearMode)
{
	return ((RwCamera * (__cdecl*)(RwCamera*, RwRGBA*, RwInt32))0x00458300)(camera, colour, clearMode);
}

void RsCameraShowRaster(RwCamera* camera)
{
	((void (__cdecl*)(RwCamera*))0x0041B260)(camera);
}

RpWorld *RpWorldAddCamera(RpWorld *world, RwCamera *camera)
{
	return ((RpWorld * (__cdecl*)(RpWorld*, RwCamera*))0x004852D0)(world, camera);
}

RpWorld *RpWorldRemoveCamera(RpWorld *world, RwCamera *camera)
{
	return ((RpWorld * (__cdecl*)(RpWorld*, RwCamera*))0x00485310)(world, camera);
}

RpWorld* RpWorldRender(RpWorld* world)
{
	return ((RpWorld * (__cdecl*)(RpWorld*))0x004894A0)(world);
}

RwCamera* RwCameraBeginUpdate(RwCamera* camera)
{
	return ((RwCamera * (__cdecl*)(RwCamera*))0x004581D0)(camera);
}

RwCamera* RwCameraEndUpdate(RwCamera* camera)
{
	return ((RwCamera * (__cdecl*)(RwCamera*))0x004581B0)(camera);
}

RwCamera* CreateCamera()
{
	return ((RwCamera * (__cdecl*)())0x0041B110)();
}

void CameraDestroy(RwCamera* camera)
{
	((void (__cdecl*)(RwCamera*))0x0041C9A0)(camera);
}

RpLight* CreateLight(RpWorld* world)
{
	return ((RpLight * (__cdecl*)(RpWorld*))0x0041AFF0)(world);
}

void RemoveLight(RpLight* light)
{
	((void (__cdecl*)(RpLight*))0x0041A6C0)(light);
}

RpWorld* LoadWorld(RwChar* bspFile)
{
	return ((RpWorld * (__cdecl*)(RwChar*))0x00402000)(bspFile);
}



