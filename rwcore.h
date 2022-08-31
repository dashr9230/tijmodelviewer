
#pragma once

#define PSGLOBAL(var) (((psGlobalType *)(RsGlobal.ps))->var)

typedef char RwChar;
typedef signed char RwInt8;
typedef unsigned char RwUInt8;
typedef short RwInt16;
typedef unsigned short RwUInt16;
typedef long RwFixed;
typedef int RwInt32;
typedef unsigned int RwUInt32;
typedef RwInt32 RwBool;
typedef float RwReal;
typedef __int64 RwInt64;
typedef unsigned __int64 RwUInt64;

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
	rsINITIALIZE = 15,
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

typedef RsEventStatus (*RsInputEventHandler)(RsEvent event, void *param);

struct RwV2d
{
    RwReal x;
    RwReal y;
};
typedef struct RwV2d RwV2d;

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
    const RwChar *appName;
    RwInt32 maximumWidth;
    RwInt32 maximumHeight;
    RwBool  quit;

    void   *ps; /* platform specific data */

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

struct RpLight
{
    // TODO: RpLight
};

typedef struct RpLight RpLight;

RsEventStatus RsEventHandler(RsEvent event, void *param);
BOOL InitApplication(HANDLE instance);
HWND InitInstance(HANDLE instance);
RwBool RsAlwaysOnTop(RwBool alwaysOnTop);
void RsMouseSetPos(RwV2d *pos);
RwBool AttachInputDevices(void);
RwBool RsInputDeviceAttach(RsInputDeviceType inputDevice, RsInputEventHandler inputEventHandler);
void RsMouseSetVisibility(RwBool visible);
RwBool Initialize3D(void *param);
void Terminate3D();
RwBool RsRwInitialize(void *displayID);
void RsRwTerminate();
RtCharset *RtCharsetCreate(const RwRGBA * foreGround, const RwRGBA * backGround);
RwBool RwRasterDestroy(RwRaster* raster);
RpWorld* LoadWorld(RwChar* bspFile);
RpLight* CreateLight(RpWorld* world);
void RemoveLight(RpLight* light);
RwUInt32 RsTimer(void);
