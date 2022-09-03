
#include "main.h"

RwRGBA ForegroundColor = { 200, 200, 200, 255 };
RwRGBA BackgroundColor = { 64,  64,  64,   0 };

float* FarClipPlane = (float*)0x004CF4DC;

RtCharset** Charset = (RtCharset**)0x004E5BF0;
RwCamera** Camera = (RwCamera**)0x00504520;
RpWorld** World = (RpWorld**)0x00504058;
RpLight** MainLight = (RpLight**)0x00504054;
RpLight** AmbientLight = (RpLight**)0x00504050;

RwInt32 FrameCounter = 0;
RwInt32 FramesPerSecond = 0;

extern RwReal CameraPitchRate;
extern RwReal CameraTurnRate;
extern RwReal CameraSpeed;
extern RwReal CameraStrafeSpeed;

extern RwBool boost;

RwBool CameraUpdate(RwReal deltaTime)
{
	if (CameraSpeed != 0.0f ||
		CameraPitchRate != 0.0f ||
		CameraTurnRate != 0.0f ||
		CameraStrafeSpeed != 0.0f)
	{
		RwFrame* frame = RwCameraGetFrame(*Camera);
		RwMatrix* m = RwFrameGetMatrix(frame);
		RwV3d* right = RwMatrixGetRight(m);
		RwV3d yAxis = { 0.0f, 1.0f, 0.0f };
		RwV3d pos, invPos;

		pos = *RwMatrixGetPos(m);

		RwV3dNegate(&invPos, &pos);
		RwFrameTranslate(frame, &invPos, 2);

		RwFrameRotate(frame, right, CameraPitchRate * deltaTime,
			2);
		RwFrameRotate(frame, &yAxis, CameraTurnRate * deltaTime,
			2);
	
		RwV3dIncrementScaled(&pos, RwMatrixGetAt(m),
			(CameraSpeed * 0.2f) * deltaTime);
		RwV3dIncrementScaled(&pos, RwMatrixGetRight(m),
			(CameraStrafeSpeed * 0.3f) * deltaTime);

		RwFrameTranslate(frame, &pos, 2);
	}

	return TRUE;
}

void CameraLook(RwReal turn, RwReal tilt)
{
	RwFrame* cameraFrame;
	RwV3d delta, pos, * right;

	static RwReal totalTilt = 0.0f;
	static RwV3d Yaxis = { 0.0f, 1.0f, 0.0f };

	if (totalTilt + tilt > 89.0f)
	{
		tilt = 89.0f - totalTilt;

	}
	else if (totalTilt + tilt < -89.0f)
	{
		tilt = -89.0f - totalTilt;
	}

	totalTilt += tilt;

	cameraFrame = RwCameraGetFrame(*Camera);

	pos = *RwMatrixGetPos(RwFrameGetMatrix(cameraFrame));

	RwV3dScale(&delta, &pos, -1.0f);
	RwFrameTranslate(cameraFrame, &delta, 2);

	right = RwMatrixGetRight(RwFrameGetMatrix(cameraFrame));
	RwFrameRotate(cameraFrame, right, tilt, 2);
	RwFrameRotate(cameraFrame, &Yaxis, -turn, 2);

	RwFrameTranslate(cameraFrame, &pos, 2);
}

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

	RwChar* path = NULL;
	path = RsPathnameCreate("levelk/track/track.txd");

	if (!sub_41ADF0(path))
	{
		Log("no texture dictionary found");

		return FALSE;
	}

	RsPathnameDestroy(path);

	*World = LoadWorld("levelk/track.bsp");
	if (World == NULL)
	{
		Log("Cannot create world.");

		return FALSE;
	}

	*FarClipPlane = 1000.0f;

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

void Idle()
{
	//((void (__cdecl*)())0x004194A0)(); // Idle()

	RwUInt32 thisTime;
	RwReal deltaTime;

	static RwBool firstCall = TRUE;
	static RwUInt32 lastFrameTime, lastAnimTime;

	if (firstCall)
	{
		lastFrameTime = lastAnimTime = RsTimer();

		firstCall = FALSE;
	}

	thisTime = RsTimer();

	if (thisTime > (lastFrameTime + 1000))
	{
		FramesPerSecond = FrameCounter;

		FrameCounter = 0;

		lastFrameTime = thisTime;
	}

	deltaTime = (thisTime - lastAnimTime) * 0.001f;

	CameraUpdate(deltaTime);

	lastAnimTime = thisTime;

	// Rendering part...

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

RwFrame* RwFrameTranslate(RwFrame* frame, const RwV3d* v, RwInt32 combine)
{
	return ((RwFrame * (__cdecl*)(RwFrame*, const RwV3d*, RwInt32))0x00454A20)(frame, v, combine);
}

RwFrame* RwFrameRotate(RwFrame* frame, const RwV3d* axis, RwReal angle, RwInt32 combine)
{
	return ((RwFrame * (__cdecl*)(RwFrame*, const RwV3d*, RwReal, RwInt32))0x00454AE0)(frame, axis, angle, combine);
}

RpWorld* LoadWorld(RwChar* bspFile)
{
	return ((RpWorld * (__cdecl*)(RwChar*))0x00402000)(bspFile);
}

RwInt32 winTranslateKey(WPARAM wParam, LPARAM lParam)
{
	return ((RwInt32 (__cdecl*)(WPARAM, LPARAM))0x00431A40)(wParam, lParam);
}

RsEventStatus RsKeyboardEventHandler(RsEvent event, void* param)
{
	return ((RsEventStatus (__cdecl*)(RsEvent, void*))0x0041B3B0)(event, param);
}

RsEventStatus RsMouseEventHandler(RsEvent event, void* param)
{

	return ((RsEventStatus (__cdecl*)(RsEvent, void*))0x0041B3E0)(event, param);
}

RwUInt32 RsTimer(void)
{
	return ((RwUInt32 (__cdecl*)())0x0041B1C0)();
}

RwChar* RsPathnameCreate(const RwChar* srcBuffer)
{
	return ((RwChar * (__cdecl*)(const RwChar*))0x0041B660)(srcBuffer);
}

void RsPathnameDestroy(RwChar* buffer)
{
	((void (__cdecl*)(RwChar*))0x0041B640)(buffer);
}

// Unused functions

void RsMouseSetVisibility(RwBool visible)
{
	((void (__cdecl*)(RwBool))0x0041B2F0)(visible);
}

// Testing

int sub_41ADF0(RwChar* dictName) // Applies texture to BSDs. somehow.
{
	return ((int (__cdecl*)(RwChar*))0x0041ADF0)(dictName);
}