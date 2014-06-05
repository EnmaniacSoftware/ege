#include "iOS/Services/Interface/DeviceServicesIOS.h"
#include "Core/Services/Interface/SpecialURLs.h"
#include "EGEDebug.h"
#import <UIKit/UIKit.h>
#import <Security/Security.h>

/*! Secure storage implementation based on the following articale:
 *  http://useyourloaf.com/blog/2010/03/29/simple-iphone-keychain-access.html
 */

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KConfidentialValuesPrefix = "ege.deviceservices.confidentialstore.";

static const NSString* KEGEITunesAppIdKey = @"ege-itunes-app-id";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function used to create dictionary to manipulate secure storage.
 *  @param  keyValue  Name of the key for dictionary is to be created.
 *  @return Created dictionary.
 */
static NSMutableDictionary* CreateSearchDictionary(NSString* keyValue)
{
  NSMutableDictionary* searchDictionary = [[NSMutableDictionary alloc] init];
  
  // set class of keychain
  [searchDictionary setObject: (id) kSecClassGenericPassword forKey: (id) kSecClass];
  
  // set key value name
  NSData* keyValueData = [keyValue dataUsingEncoding: NSUTF8StringEncoding];
  [searchDictionary setObject: keyValueData forKey: (id) kSecAttrGeneric];
  
  // set rest of attibutes
  NSString* serviceAttrbute = [NSString stringWithFormat: @"%s", KConfidentialValuesPrefix];
  NSString* keyAttributeData = [NSString stringWithFormat:@"%@%@", serviceAttrbute, keyValue];
  
  [searchDictionary setObject: keyAttributeData forKey: (id) kSecAttrAccount];
  [searchDictionary setObject: serviceAttrbute forKey: (id) kSecAttrService];
  
  return [searchDictionary autorelease];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function used to search and retrieve dictionary value from secure storage.
 *  @param  keyValue  Name of the key for dictionary is to be retrieved.
 *  @return Buffer containing data associated with the given key. NIL is returned in case of an error, key has not been found, or no data was associated.
 */
static NSData* SearchDictionary(NSString* keyValue)
{
  NSMutableDictionary* searchDictionary = CreateSearchDictionary(keyValue);
  EGE_ASSERT(nil != searchDictionary);
  
  // specify that only one result is expected
  [searchDictionary setObject: (id) kSecMatchLimitOne forKey: (id) kSecMatchLimit];
  
  // make sure data buffer will be available directly
  [searchDictionary setObject: (id) kCFBooleanTrue forKey: (id) kSecReturnData];
  
  NSData* valueData = nil;
  SecItemCopyMatching((CFDictionaryRef) searchDictionary, (CFTypeRef*) &valueData);
  [valueData autorelease];
  
  return valueData;
}
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
  
  // convert key name
  NSString* nsName  = [NSString stringWithCString: (KConfidentialValuesPrefix + name).c_str() encoding: NSASCIIStringEncoding];
  NSData* valueData = [[[NSData alloc] initWithBytes: value.c_str() length: value.size()] autorelease];
  
  if ((nil == nsName) || (nil == valueData))
  {
    // error!
    result = EGE_ERROR_NO_MEMORY;
  }
  else
  {
    // create new search dictionary
    NSMutableDictionary* searchDictionary = CreateSearchDictionary(nsName);
    if (nil == searchDictionary)
    {
      // error!
      result = EGE_ERROR_NO_MEMORY;
    }
    else
    {
      // add data buffer to dictionary
      [searchDictionary setObject: valueData forKey: (id) kSecValueData];
      
      // add to key chain
      result = (errSecSuccess == SecItemAdd((CFDictionaryRef) searchDictionary, NULL)) ? EGE_SUCCESS : EGE_ERROR;
    }
  }
  
  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult DeviceServicesIOS::storeConfidentialValue(const String& name, const PDataBuffer value)
{
  EGEResult result = EGE_ERROR;

  // convert key name
  NSString* nsName = [NSString stringWithCString: (KConfidentialValuesPrefix + name).c_str() encoding: NSASCIIStringEncoding];
  NSData* nsValue  = [NSData dataWithBytesNoCopy:value->data(value->readOffset()) length: value->size() - value->readOffset() freeWhenDone: NO];
  
  if ((nil == nsName) || (nil == nsValue))
  {
    // error!
    result = EGE_ERROR_NO_MEMORY;
  }
  else
  {
    // create new search dictionary
    NSMutableDictionary* searchDictionary = CreateSearchDictionary(nsName);
    if (nil == searchDictionary)
    {
      // error!
      result = EGE_ERROR_NO_MEMORY;
    }
    else
    {
      // add data buffer to dictionary
      [searchDictionary setObject: nsValue forKey: (id) kSecValueData];
      
      // add to key chain
      result = (errSecSuccess == SecItemAdd((CFDictionaryRef) searchDictionary, NULL)) ? EGE_SUCCESS : EGE_ERROR;
    }
  }
  
  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult DeviceServicesIOS::retrieveConfidentialValue(const String& name, String& value)
{
  EGEResult result = EGE_ERROR_NOT_FOUND;

  // convert key name
  NSString* nsName = [NSString stringWithCString: (KConfidentialValuesPrefix + name).c_str() encoding: NSASCIIStringEncoding];
  if (nil == nsName)
  {
    // error!
    result = EGE_ERROR_NO_MEMORY;
  }
  else
  {
    // find key data in dictionary
    NSData* data = SearchDictionary(nsName);
    if (nil != data)
    {
      // convert to string
      NSString* valueDataAsString = [[[NSString alloc] initWithData: data encoding: NSUTF8StringEncoding] autorelease];
      if (nil == valueDataAsString)
      {
        // error!
        result = EGE_ERROR_NO_MEMORY;
      }
      else
      {
        // convert to EGE format
        value = [valueDataAsString cStringUsingEncoding: NSASCIIStringEncoding];
    
        // set result
        result = EGE_SUCCESS;
      }
    }
  }
  
  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult DeviceServicesIOS::retrieveConfidentialValue(const String& name, PDataBuffer& value)
{
  EGEResult result = EGE_ERROR_NOT_FOUND;
  
  // convert key name
  NSString* nsName = [NSString stringWithCString: (KConfidentialValuesPrefix + name).c_str() encoding: NSASCIIStringEncoding];
  if (nil == nsName)
  {
    // error!
    result = EGE_ERROR_NO_MEMORY;
  }
  else
  {
    // find key data in dictionary
    NSData* data  = SearchDictionary(nsName);
    if (nil != data)
    {
      // convert to EGE format
      result = ([data length] == value->write([data bytes], [data length])) ? EGE_SUCCESS : EGE_ERROR;
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
