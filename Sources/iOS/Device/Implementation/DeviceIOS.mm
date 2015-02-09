#include "Core/Device/Interface/Device.h"
#include "EGEDebug.h"
#include <sys/types.h>
#include <sys/sysctl.h>
#import <mach/mach.h>
#import <mach/mach_host.h>
#import <CoreGraphics/CGGeometry.h>
#import <Foundation/NSThread.h>
#import <UIKit/UIScreen.h>
#import <UIKit/UIDevice.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
struct DeviceInfo
{
  NSString* deviceId;
  
  DeviceType egeDevice;
};

static DeviceInfo l_iOSDeviceInfoMap[] = { 
  { @"iPhone1,1",   EDeviceiPhone},
  { @"iPhone1,2",   EDeviceiPhone3G },
  { @"iPhone2,1",   EDeviceiPhone3GS },
  { @"iPhone3,1",   EDeviceiPhone4 },
  { @"iPhone3,2",   EDeviceiPhone4 },
  { @"iPhone3,3",   EDeviceiPhone4 },
  { @"iPhone4,1",   EDeviceiPhone4S },
  { @"iPhone5,1",   EDeviceiPhone5 },
  { @"iPhone5,2",   EDeviceiPhone5 },
  { @"iPhone5,3",   EDeviceiPhone5C },
  { @"iPhone5,4",   EDeviceiPhone5C },
  { @"iPhone6,1",   EDeviceiPhone5S },
  { @"iPhone6,2",   EDeviceiPhone5S },
  { @"iPod1,1",     EDeviceiPodTouch1 },
  { @"iPod2,1",     EDeviceiPodTouch2 },
  { @"iPod3,1",     EDeviceiPodTouch3 },
  { @"iPod4,1",     EDeviceiPodTouch4 },
  { @"iPod5,1",     EDeviceiPodTouch5 },
  { @"iPad1,1",     EDeviceiPad},
  { @"iPad2,1",     EDeviceiPad2 },
  { @"iPad2,2",     EDeviceiPad2 },
  { @"iPad2,3",     EDeviceiPad2 },
  { @"iPad2,4",     EDeviceiPad2 },
  { @"iPad2,5",     EDeviceiPadMini },
  { @"iPad2,6",     EDeviceiPadMini },
  { @"iPad2,7",     EDeviceiPadMini },
  { @"iPad3,1",     EDeviceiPad3 },
  { @"iPad3,2",     EDeviceiPad3 },
  { @"iPad3,3",     EDeviceiPad3 },
  { @"iPad3,4",     EDeviceiPad4 },
  { @"iPad3,5",     EDeviceiPad4 },
  { @"iPad3,6",     EDeviceiPad4 },
  { @"iPad4,1",     EDeviceiPadAir },
  { @"iPad4,2",     EDeviceiPadAir },
  { @"iPad4,3",     EDeviceiPadAir },
  { @"iPad4,4",     EDeviceiPadMini2 },
  { @"iPad4,5",     EDeviceiPadMini2 },
  { @"iPad4,6",     EDeviceiPadMini2 },
  { @"i386",        EDeviceEmulator },
  { @"x86_64",      EDeviceEmulator }
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function determining iOS device from string value. */
static DeviceType GetIOSDevice(const String& deviceId)
{
  // convert
  NSString* deviceIdentifier = [NSString stringWithCString: deviceId.toAscii() encoding: NSASCIIStringEncoding];
    
  // go thru all iOS devices
  for (u32 i = 0; i < sizeof (l_iOSDeviceInfoMap) / sizeof (l_iOSDeviceInfoMap[0]); ++i)
  {
    const DeviceInfo& deviceInfo = l_iOSDeviceInfoMap[i];

    // check if found
    if (YES == [deviceInfo.deviceId isEqualToString: deviceIdentifier])
    {
      // check if SIMULATOR
      // NOTE: in case of native simulator we want to mimic the exact device class
      if (EDeviceEmulator == deviceInfo.egeDevice)
      {
        // determine device class based on resolution
        s32 width   = Device::SurfaceWidth();
        s32 height  = Device::SurfaceHeight();
        
        // 320x480 device (iPhone 3GS)
        if ((320 == width) && (480 == height))
        {
          return EDeviceiPhone3GS;
        }
        // 640x960 device (iPhone 4)
        else if ((640 == width) && (960 == height))
        {
          return EDeviceiPhone4;
        }
        // 640x1136 device (iPhone 5)
        else if ((640 == width) && (1136 == height))
        {
          return EDeviceiPhone5;
        }
        // 1024x768 device (iPad)
        else if ((768 == width) && (1024 == height))
        {
          return EDeviceiPad2;
        }
        // 2048x1536 device (iPad retina)
        else if ((1536 == width) && (2048 == height))
        {
          return EDeviceiPad4;
        }
        
        EGE_ASSERT_X(false, "Unsupported device!");
      }
      
      return deviceInfo.egeDevice;
    }
  }

  return EDeviceGeneric;
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
    natural_t mem_used = static_cast<natural_t>((vm_stat.active_count + vm_stat.inactive_count + vm_stat.wire_count) * pagesize);
    availableRAM = vm_stat.free_count * pagesize;
    totalRAM = mem_used + availableRAM;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DeviceOS Device::GetOS()
{
#if TARGET_IPHONE_SIMULATOR
  return EMacOS;
#else
  return EIOS;
#endif // TARGET_IPHONE_SIMULATOR
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DeviceType Device::GetDevice()
{
  // retrieve device name
  char modelName[64] = "\0";
  size_t size = sizeof (modelName) - 1;
  sysctlbyname("hw.machine", modelName, &size, NULL, 0);

  String deviceName(modelName);
  DeviceType deviceId = GetIOSDevice(deviceName);

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
    CFRelease(string);
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
