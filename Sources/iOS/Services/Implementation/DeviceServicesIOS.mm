#include "iOS/Services/Interface/DeviceServicesIOS.h"
#import <UIKit/UIKit.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DeviceServicesIOS::DeviceServicesIOS() : DeviceServices()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DeviceServicesIOS::~DeviceServicesIOS()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool DeviceServicesIOS::openUrl(const String& url)
{
  // convert URL
  NSString* nsUrl = [NSString stringWithCString: url.c_str() encoding: NSASCIIStringEncoding];
  
  // try to open it
  return (YES == [[UIApplication sharedApplication] openURL: [NSURL URLWithString: nsUrl]]);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
