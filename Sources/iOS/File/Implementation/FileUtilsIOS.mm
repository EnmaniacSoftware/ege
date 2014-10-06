#include "Core/File/Interface/FileUtils.h"
#include "iOS/String/Interface/StringHelper.h"
#import <Foundation/NSFileHandle.h>
#import <Foundation/NSFileManager.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FileUtils::Exists(const String& filePath)
{
  return (YES == [[NSFileManager defaultManager] fileExistsAtPath: StringHelper::Convert(filePath)]);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FileUtils::Remove(const String& filePath)
{
  return (YES == [[NSFileManager defaultManager] removeItemAtPath: StringHelper::Convert(filePath)  error: nil]);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 FileUtils::Size(const String& filePath)
{
  // first try to retrieve file info from documents folder
  NSDictionary* dict = [[NSFileManager defaultManager] attributesOfItemAtPath: StringHelper::Convert(filePath) error: nil];
  
  // get file size attribute
  s64 size = [dict fileSize];
  
  // check if not found
  if (0 == size)
  {
    // error!
    size = -1;
  }
  
  return size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END
