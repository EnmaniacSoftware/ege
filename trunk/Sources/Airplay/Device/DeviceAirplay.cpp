#include "Core/Device/Device.h"
#include "s3e.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns current Operating System ID. */
EGEDevice::OS Device::GetOS()
{
  int32 osId = s3eDeviceGetInt(S3E_DEVICE_OS);
  switch (osId)
  {
    case S3E_OS_ID_IPHONE:  return EGEDevice::OS_IPHONE;
    case S3E_OS_ID_WINDOWS: return EGEDevice::OS_WINDOWS;
  }

  return EGEDevice::OS_UNKOWN;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns current device ID. */
EGEDevice::Device Device::GetDevice()
{
  int32 deviceId = s3eDeviceGetInt(S3E_DEVICE_ID);

  // get OS ID
  EGEDevice::OS osId = Device::GetOS();
  switch (osId)
  {
    // for iOS
    case EGEDevice::OS_IPHONE:

      switch (deviceId)
      {
        case -565348713: return EGEDevice::DEVICE_IPHONE_3G;
        case -565347625: return EGEDevice::DEVICE_IPHONE_3GS;
        case -565346536: return EGEDevice::DEVICE_IPHONE_4;
        case 1494189823: return EGEDevice::DEVICE_IPOD_TOUCH_1;
        case 1494190912: return EGEDevice::DEVICE_IPOD_TOUCH_2;
        case 1477586929: return EGEDevice::DEVICE_IPAD;
      }
      break;

    // for Windows
    case EGEDevice::OS_WINDOWS:

      switch (deviceId)
      {
        case 1: return EGEDevice::DEVICE_EMULATOR;
      }
      break;
  }

  return EGEDevice::DEVICE_GENERIC;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns physical surface width. */
s32 Device::SurfaceWidth()
{
  return s3eSurfaceGetInt(S3E_SURFACE_DEVICE_WIDTH);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns physical surface height. */
s32 Device::SurfaceHeight()
{
 return s3eSurfaceGetInt(S3E_SURFACE_DEVICE_HEIGHT);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

