#include "iOS/Services/Interface/DeviceServicesIOS.h"
#import <UIKit/UIKit.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KConfidentialValuesPrefix = "ege.deviceservices.confidentialstore.";
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
EGEResult DeviceServicesIOS::storeConfidentialValue(const String& name, const String& value)
{
  // convert name and value
  NSString* nsName  = [NSString stringWithCString: (KConfidentialValuesPrefix + name).c_str() encoding: NSASCIIStringEncoding];
  NSString* nsValue = [NSString stringWithCString: value.c_str() encoding: NSASCIIStringEncoding];

  // store
  NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];

  [defaults setObject: nsValue forKey: nsName];

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult DeviceServicesIOS::retrieveConfidentialValue(const String& name, String& value)
{
  EGEResult result = EGE_ERROR_NOT_FOUND;

  // convert name
  NSString* nsName = [NSString stringWithCString: (KConfidentialValuesPrefix + name).c_str() encoding: NSASCIIStringEncoding];

  // retrieve
  NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
  NSString* nsValue = [defaults stringForKey: nsName])
  EGE_ASSERT(nil != nsValue);
    
  if (nil != nsValue)
  {
    // convert to EGE format
    value = [nsValue cStringUsingEncoding: NSASCIIStringEncoding];
  
    // success
    result = EGE_SUCCESS;
  }
  
  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
