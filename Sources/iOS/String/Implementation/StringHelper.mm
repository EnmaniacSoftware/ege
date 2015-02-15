#include "iOS/String/Interface/StringHelper.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#if TARGET_RT_BIG_ENDIAN
  const NSStringEncoding KWcharEncoding = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32BE);
#else
  const NSStringEncoding KWcharEncoding = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32LE);
#endif // TARGET_RT_BIG_ENDIAN
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
NSString* StringHelper::Convert(const String& string)
{
  NSString* value = [NSString stringWithCString: string.toAscii() encoding: NSASCIIStringEncoding];
  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
NSString* StringHelper::Convert(const Text& string)
{
  const char* data = reinterpret_cast<const char*>(string.data());
  unsigned size = string.length() * sizeof(wchar_t);
    
  NSString* result = [[[NSString alloc] initWithBytes: data length: size encoding: KWcharEncoding] autorelease];
  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String StringHelper::ConvertToString(NSString* string)
{
  return [string UTF8String];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Text StringHelper::ConvertToText(NSString* string)
{
  NSData* data = [string dataUsingEncoding: KWcharEncoding];
  
  return Text(reinterpret_cast<const Char*>([data bytes]), [data length] / sizeof(wchar_t));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END
