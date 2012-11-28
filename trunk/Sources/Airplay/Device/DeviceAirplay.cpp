#include "Core/Device/Device.h"
#include "s3e.h"
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function determining device from string value. */
static EGEDevice::Device GetDeviceFromString()
{
  const char* stringId = s3eDeviceGetString(S3E_DEVICE_ID);

  if (0 == strcmp(stringId, "iPad2,5"))
  {
    return EGEDevice::DEVICE_IPAD_MINI;
  }
  else if (0 == strcmp(stringId, "iPhone5,1"))
  {
    return EGEDevice::DEVICE_IPHONE_5;
  }
  

  return EGEDevice::DEVICE_GENERIC;
}
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
  int32 deviceIdentifier = s3eDeviceGetInt(S3E_DEVICE_ID);

  EGEDevice::Device deviceId = EGEDevice::DEVICE_GENERIC;

  // get OS ID
  EGEDevice::OS osId = Device::GetOS();
  switch (osId)
  {
    // for iOS
    case EGEDevice::OS_IPHONE:

      switch (deviceIdentifier)
      {
        case -565348713: deviceId = EGEDevice::DEVICE_IPHONE_3G; break;
        case -565347625: deviceId = EGEDevice::DEVICE_IPHONE_3GS; break;
        case -565346536: deviceId = EGEDevice::DEVICE_IPHONE_4; break;
        case -565345447: deviceId = EGEDevice::DEVICE_IPHONE_4S; break;
        case 1494189823: deviceId = EGEDevice::DEVICE_IPOD_TOUCH_1; break;
        case 1494190912: deviceId = EGEDevice::DEVICE_IPOD_TOUCH_2; break;
        case 1477586929: deviceId = EGEDevice::DEVICE_IPAD; break;
        case 1477588018: deviceId = EGEDevice::DEVICE_IPAD_2; break;
        case 1477589107: deviceId = EGEDevice::DEVICE_IPAD_3; break;

        default:

          // try to get it from string representation
          deviceId = GetDeviceFromString();
          break;
      }      
      break;

    // for Windows
    case EGEDevice::OS_WINDOWS:

      switch (deviceIdentifier)
      {
        case 1: deviceId = EGEDevice::DEVICE_EMULATOR; break;
      }
      break;
  }

  // check if not found
  if (EGEDevice::DEVICE_GENERIC == deviceId)
  {
    egeWarning() << "Unknown device" << deviceIdentifier;
  }
  else
  {
    egeWarning() << "Device identified" << deviceId;
  }

  return deviceId;
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