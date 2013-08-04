#include "Core/Device/Device.h"
#include "EGEDebug.h"
#include <sys/types.h>
#include <sys/sysctl.h>
#import <mach/mach.h>
#import <mach/mach_host.h>
#import <CoreGraphics/CGGeometry.h>
#import <Foundation/NSThread.h>
#import <UIKit/UIScreen.h>
#import <UIKit/UIDevice.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
struct DeviceInfo
{
  NSString* deviceId;
  
  EGEDevice::Device egeDevice;
};

static DeviceInfo l_iOSDeviceInfoMap[] = { 
  { @"iPhone1,1",   EGEDevice::DEVICE_IPHONE },
  { @"iPhone1,2",   EGEDevice::DEVICE_IPHONE_3G },
  { @"iPhone2,1",   EGEDevice::DEVICE_IPHONE_3GS },
  { @"iPhone3,1",   EGEDevice::DEVICE_IPHONE_4 },
  { @"iPhone3,2",   EGEDevice::DEVICE_IPHONE_4 },
  { @"iPhone3,3",   EGEDevice::DEVICE_IPHONE_4 },
  { @"iPhone4,1",   EGEDevice::DEVICE_IPHONE_4S },
  { @"iPhone5,1",   EGEDevice::DEVICE_IPHONE_5 },
  { @"iPhone5,2",   EGEDevice::DEVICE_IPHONE_5 },
  { @"iPod1,1",     EGEDevice::DEVICE_IPOD_TOUCH_1 },
  { @"iPod2,1",     EGEDevice::DEVICE_IPOD_TOUCH_2 },
  { @"iPod3,1",     EGEDevice::DEVICE_IPOD_TOUCH_3 },
  { @"iPod4,1",     EGEDevice::DEVICE_IPOD_TOUCH_4 },
  { @"iPod5,1",     EGEDevice::DEVICE_IPOD_TOUCH_5 },
  { @"iPad1,1",     EGEDevice::DEVICE_IPAD },
  { @"iPad2,1",     EGEDevice::DEVICE_IPAD_2 },
  { @"iPad2,2",     EGEDevice::DEVICE_IPAD_2 },
  { @"iPad2,3",     EGEDevice::DEVICE_IPAD_2 },
  { @"iPad2,4",     EGEDevice::DEVICE_IPAD_2 },
  { @"iPad2,5",     EGEDevice::DEVICE_IPAD_MINI },
  { @"iPad2,6",     EGEDevice::DEVICE_IPAD_MINI },
  { @"iPad2,7",     EGEDevice::DEVICE_IPAD_MINI },
  { @"iPad3,1",     EGEDevice::DEVICE_IPAD_3 },
  { @"iPad3,2",     EGEDevice::DEVICE_IPAD_3 },
  { @"iPad3,3",     EGEDevice::DEVICE_IPAD_3 },
  { @"iPad3,4",     EGEDevice::DEVICE_IPAD_4 },
  { @"i386",        EGEDevice::DEVICE_EMULATOR },
  { @"x86_64",      EGEDevice::DEVICE_EMULATOR }
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function determining iOS device from string value. */
static EGEDevice::Device GetIOSDevice(const String& deviceId)
{
  // convert
  NSString* deviceIdentifier = [NSString stringWithCString: deviceId.c_str() encoding: NSASCIIStringEncoding];
    
  // go thru all iOS devices
  for (u32 i = 0; i < sizeof (l_iOSDeviceInfoMap) / sizeof (l_iOSDeviceInfoMap[0]); ++i)
  {
    const DeviceInfo& deviceInfo = l_iOSDeviceInfoMap[i];

    // check if found
    if (YES == [deviceInfo.deviceId isEqualToString: deviceIdentifier])
    {
      // check if SIMULATOR
      // NOTE: in case of native simulator we want to mimic the exact device class
      if (EGEDevice::DEVICE_EMULATOR == deviceInfo.egeDevice)
      {
        // determine device class based on resolution
        s32 width   = Device::SurfaceWidth();
        s32 height  = Device::SurfaceHeight();
        
        // 320x480 device (iPhone 3GS)
        if ((320 == width) && (480 == height))
        {
          return EGEDevice::DEVICE_IPHONE_3GS;
        }
        // 640x960 device (iPhone 4)
        else if ((640 == width) && (960 == height))
        {
          return EGEDevice::DEVICE_IPHONE_4;
        }
        // 640x1136 device (iPhone 5)
        else if ((640 == width) && (1136 == height))
        {
          return EGEDevice::DEVICE_IPHONE_5;
        }
        // 1024x768 device (iPad)
        else if ((1024 == width) && (768 == height))
        {
          return EGEDevice::DEVICE_IPAD_2;
        }
        // 2048x1536 device (iPad retina)
        else if ((2048 == width) && (1536 == height))
        {
          return EGEDevice::DEVICE_IPAD_4;
        }
        
        EGE_ASSERT(false && "Unsupported!");
      }
      
      return deviceInfo.egeDevice;
    }
  }

  return EGEDevice::DEVICE_GENERIC;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function calculating memory statistics. */
void GetMemoryStatistics(u64& availableRAM, u64& totalRAM)
{
  mach_port_t host_port;
  mach_msg_type_number_t host_size;
  
  host_port = mach_host_self();
  host_size = sizeof (vm_statistics_data_t) / sizeof (integer_t);
  
  // get memory page size
  vm_size_t pagesize;
  host_page_size(host_port, &pagesize);

  // retrieve virtual memory data
  vm_statistics_data_t vm_stat;
  if (KERN_SUCCESS != host_statistics(host_port, HOST_VM_INFO, (host_info_t) &vm_stat, &host_size))
  {
    // error!
    egeWarning(KDeviceDebugName) << "Could not retreve memory statistics";
    
    availableRAM = 0;
    totalRAM = 0;
  }
  else
  {
    natural_t mem_used = (vm_stat.active_count + vm_stat.inactive_count + vm_stat.wire_count) * pagesize;
    availableRAM = vm_stat.free_count * pagesize;
    totalRAM = mem_used + availableRAM;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEDevice::OS Device::GetOS()
{
#if TARGET_IPHONE_SIMULATOR
  return EGEDevice::OS_MACOS;
#else
  return EGEDevice::OS_IOS;
#endif // TARGET_IPHONE_SIMULATOR
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEDevice::Device Device::GetDevice()
{
  // retrieve device name
  char modelName[64] = "\0";
  size_t size = sizeof (modelName) - 1;
  sysctlbyname("hw.machine", modelName, &size, NULL, 0);

  String deviceName(modelName);
  EGEDevice::Device deviceId = GetIOSDevice(deviceName);

  egeDebug(KDeviceDebugName) << "Device ID:" << deviceName << deviceId;
  
  return deviceId;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 Device::SurfaceWidth()
{
  float32 scale = 1.0f;
 
  // get main screen
  UIScreen* screen = [UIScreen mainScreen];
  
  // get screen bounds
  CGRect screenBounds = [screen bounds];
  
  // check if scle selector is present
  if ([screen respondsToSelector: @selector(scale)])
  {
    // get scale factor
    scale = screen.scale;
  }
  
  return static_cast<s32>(screenBounds.size.width * scale);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 Device::SurfaceHeight()
{
  float32 scale = 1.0f;
  
  // get main screen
  UIScreen* screen = [UIScreen mainScreen];
  
  // get screen bounds
  CGRect screenBounds = [screen bounds];
  
  // check if scle selector is present
  if ([screen respondsToSelector: @selector(scale)])
  {
    // get scale factor
    scale = screen.scale;
  }
  
  return static_cast<s32>(screenBounds.size.height * scale);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 Device::AudioOutputFrequency()
{
  return 0;//s3eSoundGetInt(S3E_SOUND_OUTPUT_FREQ);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Device::Sleep(u32 ms)
{
  [NSThread sleepForTimeInterval: ms * 0.001];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u64 Device::AvailableMemory()
{
  u64 availableRAM;
  u64 totalRAM;
  
  GetMemoryStatistics(availableRAM, totalRAM);
  
  return availableRAM;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u64 Device::TotalMemory()
{
  u64 availableRAM;
  u64 totalRAM;
  
  GetMemoryStatistics(availableRAM, totalRAM);
  
  return totalRAM;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String Device::GetUniqueId()
{
  NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
  
  NSString* UUIDString = @"";

  // check if no UUID generated yet
  if ( ! [defaults valueForKey:@"UUID"])
  {
    // generate
    CFUUIDRef theUUID = CFUUIDCreate(NULL);
    CFStringRef string = CFUUIDCreateString(NULL, theUUID);
    CFRelease(theUUID);
    
    // store
    UUIDString = [NSString stringWithFormat: @"%@", string];
    [defaults setObject: UUIDString forKey: @"UUID"];
  }
  else
  {
    // retrieve
    UUIDString = [defaults valueForKey: @"UUID"];
  }
  
  // convert to EGE format
  String uniqueId = [UUIDString cStringUsingEncoding: NSASCIIStringEncoding];
  return uniqueId;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END