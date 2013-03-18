#include "Core/Device/Device.h"
#include "s3e.h"
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
struct DeviceInfo
{
  const char* marmaladeDeviceId;
  
  EGEDevice::Device egeDevice;
};

static DeviceInfo l_iOSDeviceInfoMap[] = { 
  { "iPhone1,1",  EGEDevice::DEVICE_IPHONE },
  { "iPhone1,2",  EGEDevice::DEVICE_IPHONE_3G },
  { "iPhone2,1",  EGEDevice::DEVICE_IPHONE_3GS },
  { "iPhone3,1",  EGEDevice::DEVICE_IPHONE_4 },
  { "iPhone3,2",  EGEDevice::DEVICE_IPHONE_4 },
  { "iPhone3,3",  EGEDevice::DEVICE_IPHONE_4 },
  { "iPhone4,1",  EGEDevice::DEVICE_IPHONE_4S },
  { "iPhone5,1",  EGEDevice::DEVICE_IPHONE_5 },
  { "iPhone5,2",  EGEDevice::DEVICE_IPHONE_5 },
  { "iPod1,1",    EGEDevice::DEVICE_IPOD_TOUCH_1 },
  { "iPod2,1",    EGEDevice::DEVICE_IPOD_TOUCH_2 },
  { "iPod3,1",    EGEDevice::DEVICE_IPOD_TOUCH_3 },
  { "iPod4,1",    EGEDevice::DEVICE_IPOD_TOUCH_4 },
  { "iPod5,1",    EGEDevice::DEVICE_IPOD_TOUCH_5 },
  { "iPad1,1",    EGEDevice::DEVICE_IPAD },
  { "iPad2,1",    EGEDevice::DEVICE_IPAD_2 },
  { "iPad2,2",    EGEDevice::DEVICE_IPAD_2 },
  { "iPad2,3",    EGEDevice::DEVICE_IPAD_2 },
  { "iPad2,4",    EGEDevice::DEVICE_IPAD_2 },
  { "iPad2,5",    EGEDevice::DEVICE_IPAD_MINI },
  { "iPad3,1",    EGEDevice::DEVICE_IPAD_3 },
  { "iPad3,2",    EGEDevice::DEVICE_IPAD_3 },
  { "iPad3,3",    EGEDevice::DEVICE_IPAD_3 },
  { "iPad3,4",    EGEDevice::DEVICE_IPAD_4 }
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function determining iOS device from string value. */
static EGEDevice::Device GetIOSDevice(const String& deviceId)
{  
  // go thru all iOS devices
  for (u32 i = 0; i < sizeof (l_iOSDeviceInfoMap) / sizeof (l_iOSDeviceInfoMap[0]); ++i)
  {
    const DeviceInfo& deviceInfo = l_iOSDeviceInfoMap[i];

    // check if found
    if (deviceInfo.marmaladeDeviceId == deviceId)
    {
      // found
      return deviceInfo.egeDevice;
    }
  }

  return EGEDevice::DEVICE_GENERIC;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEDevice::OS Device::GetOS()
{
  int32 osId = s3eDeviceGetInt(S3E_DEVICE_OS);
  switch (osId)
  {
    case S3E_OS_ID_IPHONE:  return EGEDevice::OS_IOS;
    case S3E_OS_ID_WINDOWS: return EGEDevice::OS_WINDOWS;
  }

  return EGEDevice::OS_UNKOWN;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEDevice::Device Device::GetDevice()
{
  const String deviceName = s3eDeviceGetString(S3E_DEVICE_ID);

  EGEDevice::Device deviceId = EGEDevice::DEVICE_GENERIC;

  egeDebug() << "Device ID:" << deviceName << s3eDeviceGetInt(S3E_DEVICE_OS);

  // get OS ID
  EGEDevice::OS osId = Device::GetOS();
  switch (osId)
  {
    // for iOS
    case EGEDevice::OS_IOS:

      // try to get it from string representation
      deviceId = GetIOSDevice(deviceName);
      break;

    // for Windows
    case EGEDevice::OS_WINDOWS:

      deviceId = EGEDevice::DEVICE_EMULATOR;
      break;

    default:

      egeWarning() << "Unknown OS" << osId;
  }

  return deviceId;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 Device::SurfaceWidth()
{
  return s3eSurfaceGetInt(S3E_SURFACE_DEVICE_WIDTH);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 Device::SurfaceHeight()
{
 return s3eSurfaceGetInt(S3E_SURFACE_DEVICE_HEIGHT);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 Device::AudioOutputFrequency()
{
  return s3eSoundGetInt(S3E_SOUND_OUTPUT_FREQ);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Device::Sleep(u32 ms)
{
  s3eDeviceYield(ms);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u64 Device::AvailableMemory()
{
  return static_cast<u64>(s3eDeviceGetInt(S3E_DEVICE_MEM_FREE));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u64 Device::TotalMemory()
{
  return static_cast<u64>(s3eDeviceGetInt(S3E_DEVICE_MEM_TOTAL));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String Device::GetUniqueId()
{
  return s3eDeviceGetString(S3E_DEVICE_UNIQUE_ID);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END