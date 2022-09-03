
#pragma once

#ifndef MACRO_START
#define MACRO_START do
#endif

#ifndef MACRO_STOP
#define MACRO_STOP while(0)
#endif

#define PSGLOBAL(var) (((psGlobalType *)(RsGlobal.ps))->var)

#define rwObjectGetParent(object)           (((const RwObject *)(object))->parent)
#define RwCameraGetFrameMacro(_camera)                          \
    ((RwFrame *)rwObjectGetParent((_camera)))
#define RwCameraGetFrame(_camera)                               \
    RwCameraGetFrameMacro(_camera)

#define RwFrameGetMatrixMacro(_f)   (&(_f)->modelling)
#define RwFrameGetMatrix(_f)    RwFrameGetMatrixMacro(_f)

#define RwMatrixGetRight(m)    (&(m)->right)
#define RwMatrixGetUp(m)       (&(m)->up)
#define RwMatrixGetAt(m)       (&(m)->at)
#define RwMatrixGetPos(m)      (&(m)->pos)

#define RwV3dNegateMacro(o, a)                                  \
MACRO_START                                                     \
{                                                               \
    (o)->x = -(a)->x;                                           \
    (o)->y = -(a)->y;                                           \
    (o)->z = -(a)->z;                                           \
}                                                               \
MACRO_STOP

#define RwV3dIncrementScaledMacro(o, a, s)                      \
MACRO_START                                                     \
{                                                               \
    (o)->x += (((a)->x) * ( (s)));                              \
    (o)->y += (((a)->y) * ( (s)));                              \
    (o)->z += (((a)->z) * ( (s)));                              \
}                                                               \
MACRO_STOP

#define RwV3dScaleMacro(o, a, s)                                \
MACRO_START                                                     \
{                                                               \
    (o)->x = (((a)->x) * ( (s)));                               \
    (o)->y = (((a)->y) * ( (s)));                               \
    (o)->z = (((a)->z) * ( (s)));                               \
}                                                               \
MACRO_STOP

#define RwV3dNegate(o, a)               RwV3dNegateMacro(o, a)
#define RwV3dIncrementScaled(o, a, s)   RwV3dIncrementScaledMacro(o, a, s)
#define RwV3dScale(o, a, s)             RwV3dScaleMacro(o, a, s)

typedef char RwChar;
typedef signed char RwInt8;
typedef unsigned char RwUInt8;
typedef short RwInt16;
typedef unsigned short RwUInt16;
typedef int RwInt32;
typedef unsigned int RwUInt32;
typedef __int64 RwInt64;
typedef unsigned __int64 RwUInt64;
typedef RwInt32 RwBool;
typedef long RwFixed;
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
    rsLEFTBUTTONDOWN,
    rsLEFTBUTTONUP,
    rsMOUSEMOVE,
    rsPLUGINATTACH,
    rsREGISTERIMAGELOADER,
    rsRIGHTBUTTONUP = 11,
    rsRWINITIALIZE = 12,
    rsRWTERMINATE,
    rsSELECTDEVICE,
    rsINITIALIZE,
    rsTERMINATE,
    rsIDLE,
    rsKEYDOWN,
    rsKEYUP,
    rsQUITAPP,
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

enum RsKeyCodes
{
    rsESC       = 128,

    rsF1        = 129,
    rsF2        = 130,
    rsF3        = 131,
    rsF4        = 132,
    rsF5        = 133,
    rsF6        = 134,
    rsF7        = 135,
    rsF8        = 136,
    rsF9        = 137,
    rsF10       = 138,
    rsF11       = 139,
    rsF12       = 140,

    rsINS       = 141,
    rsDEL       = 142,
    rsHOME      = 143,
    rsEND       = 144,
    rsPGUP      = 145,
    rsPGDN      = 146,

    rsUP        = 147,
    rsDOWN      = 148,
    rsLEFT      = 149,
    rsRIGHT     = 150,

    rsPADINS    = 151,
    rsPADDEL    = 152,
    rsPADHOME   = 153,
    rsPADEND    = 154,
    rsPADPGUP   = 155,
    rsPADPGDN   = 156,

    rsPADUP     = 157,
    rsPADDOWN   = 158,
    rsPADLEFT   = 159,
    rsPADRIGHT  = 160,

    rsNUMLOCK   = 161,
    rsDIVIDE    = 162,
    rsTIMES     = 163,
    rsMINUS     = 164,
    rsPLUS      = 165,
    rsPADENTER  = 166,
    rsPAD5      = 167,

    rsBACKSP    = 168,
    rsTAB       = 169,
    rsCAPSLK    = 170,
    rsENTER     = 171,
    rsLSHIFT    = 172,
    rsRSHIFT    = 173,
    rsLCTRL     = 174,
    rsRCTRL     = 175,
    rsLALT      = 176,
    rsRALT      = 177,

    rsNULL       = 255
};
typedef enum RsKeyCodes RsKeyCodes;

struct RwV2d
{
    RwReal x;
    RwReal y;
};
typedef struct RwV2d RwV2d;

struct RwV3d
{
    RwReal x;
    RwReal y;
    RwReal z;
};
typedef struct RwV3d RwV3d;

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

struct RwMatrixTag
{
    RwV3d               right;
    RwUInt32            flags;
    RwV3d               up;
    RwUInt32            pad1;
    RwV3d               at;
    RwUInt32            pad2;
    RwV3d               pos;
    RwUInt32            pad3;
};
typedef struct RwMatrixTag RwMatrix;

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

struct RsKeyStatus
{
    RwInt32 keyScanCode;
    RwInt32 keyCharCode;
};
typedef struct RsKeyStatus RsKeyStatus;

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

struct RwFrame
{
    char _pad[16];
    RwMatrix modelling;

    // TODO: RwFrame
};
typedef struct RwFrame RwFrame;

struct RwObject
{
    RwUInt8 type;
    RwUInt8 subType;
    RwUInt8 flags;
    RwUInt8 privateFlags;
    void* parent;
};
typedef struct RwObject RwObject;

struct RsMouseStatus
{
    // currently no evidence for offsets of these, so it's a TODO
    RwV2d   pos;
    RwV2d   delta;
    RwBool  shift;
    RwBool  control;
};
typedef struct RsMouseStatus RsMouseStatus;

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

RwFrame* RwFrameTranslate(RwFrame* frame, const RwV3d* v, RwInt32 combine);
RwFrame* RwFrameRotate(RwFrame* frame, const RwV3d* axis, RwReal angle, RwInt32 combine);

RpWorld* LoadWorld(RwChar* bspFile);

RwInt32 winTranslateKey(WPARAM wParam, LPARAM lParam);

RwBool CameraUpdate(RwReal deltaTime);
void CameraLook(RwReal turn, RwReal tilt);

RsEventStatus RsKeyboardEventHandler(RsEvent event, void* param);
RsEventStatus RsMouseEventHandler(RsEvent event, void* param);

RwUInt32 RsTimer(void);

RwChar* RsPathnameCreate(const RwChar* srcBuffer);
void RsPathnameDestroy(RwChar* buffer);

// Unused
void RsMouseSetVisibility(RwBool visible);

// Testing

int sub_41ADF0(RwChar* dictName);
