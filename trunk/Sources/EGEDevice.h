#ifndef EGE_DEVICE_H
#define EGE_DEVICE_H

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace EGEDevice
{
    /*! Operating systems available. */
    enum EOS
    {
      OS_UNKOWN = 0,
      OS_IPHONE,
      OS_WINDOWS
    };

    /*! Devices available. */
    enum EDevice
    {
      DEVICE_GENERIC = 0,
      DEVICE_EMULATOR,
      DEVICE_IPHONE_3G,
      DEVICE_IPHONE_3GS,
      DEVICE_IPHONE_4,
      DEVICE_IPAD,
      DEVICE_IPOD_TOUCH_1,
      DEVICE_IPOD_TOUCH_2,
      DEVICE_IPOD_TOUCH_3,
      DEVICE_IPOD_TOUCH_4
    };

    /*! Rendering capability flags. */
    enum ERenderCapability
    {
      RENDER_CAPS_APPLE_LIMITED_NPOT_TEXTURE = 0,           /*< Limited non-power of two 2D texture extension for iOS */
      RENDER_CAPS_COUNT
    };
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "Core/Device/Device.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_DEVICE_H