#include "Core/Device/Device.h"
#include "s3e.h"
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

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
        case -565345447: return EGEDevice::DEVICE_IPHONE_4S;
        case 1494189823: return EGEDevice::DEVICE_IPOD_TOUCH_1;
        case 1494190912: return EGEDevice::DEVICE_IPOD_TOUCH_2;
        case 1477586929: return EGEDevice::DEVICE_IPAD;
        case 1477588018: return EGEDevice::DEVICE_IPAD_2;
        case 1477589107: return EGEDevice::DEVICE_IPAD_3;

        default:

          egeWarning() << "Unknown iOS device" << deviceId;
          break;
      }      
      break;

    // for Windows
    case EGEDevice::OS_WINDOWS:

      switch (deviceId)
      {
        case 1: return EGEDevice::DEVICE_EMULATOR;

        default:

          egeWarning() << "Unknown Windows device" << deviceId;
          break;
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
/*! Returns audio output device frequency (in Hz). */
s32 Device::AudioOutputFrequency()
{
  return s3eSoundGetInt(S3E_SOUND_OUTPUT_FREQ);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sleeps current thread for a specified number of miliseconds. */
void Device::Sleep(u32 ms)
{
  s3eDeviceYield(ms);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns free RAM memory (in bytes). */
u64 Device::AvailableMemory()
{
  return static_cast<u64>(s3eDeviceGetInt(S3E_DEVICE_FREE_RAM));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns total RAM memory (in bytes). */
u64 Device::TotalMemory()
{
  return static_cast<u64>(s3eDeviceGetInt(S3E_DEVICE_TOTAL_RAM));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END