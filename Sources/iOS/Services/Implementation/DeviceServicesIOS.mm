#include "iOS/Services/Interface/DeviceServicesIOS.h"
#include "Core/Services/Interface/SpecialURLs.h"
#include "EGEDebug.h"
#import <UIKit/UIKit.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KConfidentialValuesPrefix = "ege.deviceservices.confidentialstore.";

static const NSString* KEGEITunesAppIdKey = @"ege-itunes-app-id";
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
  // check for special URLs
  if (KSpecialURLRateApp == url)
  {
    return openApplicationRateURL();
  }

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
bool DeviceServicesIOS::openApplicationRateURL()
{
  u32 appId = 0;
  
  // retrieve iTunes app id from .plist file
  NSDictionary* infoDictionary = [[NSBundle mainBundle] infoDictionary];
  
  NSNumber* value = [infoDictionary objectForKey: KEGEITunesAppIdKey];
  if (nil != value)
  {
    appId = static_cast<u32>([value intValue]);
  }
  else
  {
    // cannot process
    return false;
  }
  
	String url;
  
#if TARGET_IPHONE_SIMULATOR
  url = String("https://itunes.apple.com/app/id%1");
#else
  // iOS 7 needs a different URL
  if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 7.0)
  {
		url = String("itms-apps://itunes.apple.com/app/id%1");
	}
  else
  {
    url = String("itms-apps://ax.itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?type=Purple+Software&id=%1");
  }
#endif // TARGET_IPHONE_SIMULATOR
  
  url = url.arg(appId);

  // convert URL
  NSString* nsUrl = [NSString stringWithCString: url.c_str() encoding: NSASCIIStringEncoding];

  // execute
  return (YES == [[UIApplication sharedApplication] openURL: [NSURL URLWithString: nsUrl]]);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
