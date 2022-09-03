
#pragma once

#define MAXCAMERASPEED 200.0f
#define MAXCAMERAROTSPEED 120.0f
#define BOOSTSPEED 500.0f

#define CAMERASPEED (0.5f*MAXCAMERASPEED)
#define CAMERAROTSCL  (0.1f*MAXCAMERAROTSPEED)

RsEventStatus RsKeyboardEventHandler(RsEvent event, void *param);
RsEventStatus RsMouseEventHandler(RsEvent event, void *param);

