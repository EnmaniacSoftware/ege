#include "EGEDirectory.h"
#include "EGEDebug.h"
#import <Foundation/NSFileHandle.h>
#import <Foundation/NSFileManager.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Path to application folder. */
static String l_applicationFolderPath;
/*! Path to documents folder. */
static String l_documentsFolderPath;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String Directory::FromNativeSeparators(const String& path)
{
  // no conversion necessary as native matches the frameworks separators
  return path;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String Directory::Path(DirectoryType directory)
{
  String out;

  // proccess according to type
  switch (directory)
  {
    case EApplication:

      if (l_applicationFolderPath.empty())
      {
        // get application directory
        NSString* path = [[NSBundle mainBundle] bundlePath];
  
        l_applicationFolderPath = [path cStringUsingEncoding: NSASCIIStringEncoding];
      }

      out = l_applicationFolderPath;
      break;

    case EDocuments:

      if (l_documentsFolderPath.empty())
      {
        // get the documents directory
        NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString* path = [paths objectAtIndex: 0];
  
        l_documentsFolderPath = [path cStringUsingEncoding: NSASCIIStringEncoding];
      }

      out = l_documentsFolderPath;
      break;

    default:

      EGE_ASSERT_X(false, "Unsupported directory type!");
      break;
  };

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END