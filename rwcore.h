
#pragma once

#define PSGLOBAL(var) (((psGlobalType *)(RsGlobal.ps))->var)

typedef char RwChar;
typedef unsigned char RwUInt8;
typedef int RwInt32;
typedef RwInt32 RwBool;
typedef float RwReal;

enum RsEventStatus;
enum RsEvent;

typedef RsEventStatus (*RsInputEventHandler)(RsEvent event, void *param);

enum RsEvent
{
    rsCAMERASIZE,
    rsCOMMANDLINE,
    rsFILELOAD,
    rsINITDEBUG,
    rsINPUTDEVICEATTACH,
    rsPLUGINATTACH = 8,
    rsREGISTERIMAGELOADER,
    rsRWINITIALIZE = 12,
    rsRWTERMINATE,
    rsSELECTDEVICE,
    rsINITIALIZE,
    rsTERMINATE,
    rsIDLE,
    rsQUITAPP = 20,
    rsPREINITCOMMANDLINE = 25,
    rsACTIVATE,
};
typedef enum RsEvent RsEvent;

enum RsEventStatus
{
    rsEVENTERROR,
    rsEVENTPROCESSED,
    rsEVENTNOTPROCESSED
};
typedef enum RsEventStatus RsEventStatus;

enum RsInputDeviceType
{
    rsKEYBOARD,
    rsMOUSE,
    rsPAD
};
typedef enum RsInputDeviceType RsInputDeviceType;

struct RwV2d
{
    RwReal x;
    RwReal y;
};
typedef struct RwV2d RwV2d;

struct RwRect
{
    RwInt32 x;
    RwInt32 y;
    RwInt32 w;
    RwInt32 h;
};
typedef struct RwRect RwRect;

struct RwRGBA
{
    RwUInt8 red;
    RwUInt8 green;
    RwUInt8 blue;
    RwUInt8 alpha;
};
typedef struct RwRGBA RwRGBA;

struct RsInputDevice
{
    RsInputDeviceType inputDeviceType;
    RwBool used;
    RsInputEventHandler inputEventHandler;
};
typedef struct RsInputDevice RsInputDevice;

struct RsGlobalType
{
    const RwChar* appName;
    RwInt32 maximumWidth;
    RwInt32 maximumHeight;
    RwBool  quit;

    void* ps;

    RsInputDevice keyboard;
    RsInputDevice mouse;
    RsInputDevice pad;
};
typedef struct RsGlobalType RsGlobalType;

typedef struct
{
    HWND        window;
    HINSTANCE   instance;
    RwBool      fullScreen;
    RwV2d       lastMousePos;
}
psGlobalType;

struct RwRaster
{
    // TODO: RwRaster;
};
typedef struct RwRaster RwRaster;

typedef RwRaster RtCharset;

struct RpWorld
{
    // TODO: RpWorld
};
typedef struct RpWorld RpWorld;

struct RwCamera
{
    // TODO: RwCamera
};
typedef struct RwCamera RwCamera;

struct RpLight
{
    // TODO: RpLight
};
typedef struct RpLight RpLight;

RwBool Initialize3D(void *param);
RsEventStatus KeyboardHandler(RsEvent event, void *param);
RsEventStatus MouseHandler(RsEvent event, void *param);
RwBool AttachInputDevices(void);
void Idle();
void Terminate3D();

RsEventStatus RsEventHandler(RsEvent event, void *param);

BOOL InitApplication(HANDLE instance);
HWND InitInstance(HANDLE instance);
RwBool RsInputDeviceAttach(RsInputDeviceType inputDevice, RsInputEventHandler inputEventHandler);
RwBool RsAlwaysOnTop(RwBool alwaysOnTop);
void RsMouseSetPos(RwV2d *pos);

RwBool RsRwInitialize(void *displayID);
void RsRwTerminate();

RtCharset *RtCharsetCreate(const RwRGBA * foreGround, const RwRGBA * backGround);
RwBool RwRasterDestroy(RwRaster* raster);

RwCamera* RwCameraClear(RwCamera* camera, RwRGBA* colour, RwInt32 clearMode);
void RsCameraShowRaster(RwCamera* camera);
RpWorld *RpWorldAddCamera(RpWorld *world, RwCamera *camera);
RpWorld *RpWorldRemoveCamera(RpWorld *world, RwCamera *camera);
RpWorld* RpWorldRender(RpWorld* world);
RwCamera* RwCameraBeginUpdate(RwCamera* camera);
RwCamera* RwCameraEndUpdate(RwCamera* camera);

RwCamera* CreateCamera();
void CameraDestroy(RwCamera* camera);

RpLight* CreateLight(RpWorld* world);
void RemoveLight(RpLight* light);

RpWorld* LoadWorld(RwChar* bspFile);

