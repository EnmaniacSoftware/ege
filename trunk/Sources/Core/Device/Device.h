#ifndef EGE_CORE_DEVICE_H
#define EGE_CORE_DEVICE_H

#include "EGE.h"
#include "EGEDevice.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Device
{
  public:

    /* Returns current Operating System ID. */
    static EGEDevice::EOS GetOS();
    /* Returns current device ID. */
    static EGEDevice::EDevice GetDevice();
    /* Returns physical surface width. */
    static s32 SurfaceWidth();
    /* Returns physical surface height. */
    static s32 SurfaceHeight();
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DEVICE_H