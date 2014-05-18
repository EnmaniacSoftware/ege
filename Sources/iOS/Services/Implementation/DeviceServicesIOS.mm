#include "iOS/Services/Interface/DeviceServicesIOS.h"
#include "EGEDebug.h"
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
  
  // check if any application can handle URL
  BOOL result = [[UIApplication sharedApplication] canOpenURL: [NSURL URLWithString: nsUrl]];
  if (YES == result)
  {
    // try to open it
    result = [[UIApplication sharedApplication] openURL: [NSURL URLWithString: nsUrl]];
  }
  
  return (YES == result);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult DeviceServicesIOS::storeConfidentialValue(const String& name, const String& value)
{
  EGEResult result = EGE_ERROR;
  
  NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
  EGE_ASSERT(nil != defaults);

  // convert name and value
  NSString* nsName  = [NSString stringWithCString: (KConfidentialValuesPrefix + name).c_str() encoding: NSASCIIStringEncoding];
  NSString* nsValue = [NSString stringWithCString: value.c_str() encoding: NSASCIIStringEncoding];

  EGE_ASSERT(nil != nsName);
  EGE_ASSERT(nil != nsValue);
  
  // store
  if ((nil != nsName) && (nil != nsValue))
  {
    [defaults setObject: nsValue forKey: nsName];
  
    result = EGE_SUCCESS;
  }
  
  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult DeviceServicesIOS::storeConfidentialValue(const String& name, const PDataBuffer value)
{
  EGEResult result = EGE_ERROR;

  NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
  EGE_ASSERT(nil != defaults);
  
  // convert name and value
  NSString* nsName = [NSString stringWithCString: (KConfidentialValuesPrefix + name).c_str() encoding: NSASCIIStringEncoding];
  NSData* nsValue  = [NSData dataWithBytesNoCopy:value->data(value->readOffset()) length: value->size() - value->readOffset() freeWhenDone: NO];
  
  EGE_ASSERT(nil != nsName);
  EGE_ASSERT(nil != nsValue);
  
  // store
  if ((nil != nsName) && (nil != nsValue))
  {
    [defaults setObject: nsValue forKey: nsName];
  
    result = EGE_SUCCESS;
  }
  
  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult DeviceServicesIOS::retrieveConfidentialValue(const String& name, String& value)
{
  EGEResult result = EGE_ERROR_NOT_FOUND;

  NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
  EGE_ASSERT(nil != defaults);
  
  // convert name
  NSString* nsName = [NSString stringWithCString: (KConfidentialValuesPrefix + name).c_str() encoding: NSASCIIStringEncoding];
  EGE_ASSERT(nil != nsName);
  
  // retrieve
  if (nil != nsName)
  {
    NSString* nsValue = [defaults stringForKey: nsName];
    EGE_ASSERT(nil != nsValue);
    
    if (nil != nsValue)
    {
      // convert to EGE format
      value = [nsValue cStringUsingEncoding: NSASCIIStringEncoding];
  
      // success
      result = EGE_SUCCESS;
    }
  }
  
  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult DeviceServicesIOS::retrieveConfidentialValue(const String& name, PDataBuffer& value)
{
  EGEResult result = EGE_ERROR_NOT_FOUND;
  
  NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
  EGE_ASSERT(nil != defaults);
  
  // convert name
  NSString* nsName = [NSString stringWithCString: (KConfidentialValuesPrefix + name).c_str() encoding: NSASCIIStringEncoding];
  EGE_ASSERT(nil != nsName);
  
  // retrieve
  if (nil != nsName)
  {
    NSData* nsValue = [defaults objectForKey: nsName];
    if (nil != nsValue)
    {
      result = EGE_SUCCESS;
      
      // convert to EGE format
      if ([nsValue length] != value->write([nsValue bytes], [nsValue length]))
      {
        // error!
        result = EGE_ERROR;
      }
    }
  }
  
  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
