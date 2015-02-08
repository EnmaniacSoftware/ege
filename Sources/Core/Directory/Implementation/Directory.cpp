#include "Core/Directory/Interface/Directory.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KPathSeparator = "/";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Directory::DecomposePath(const String& fullPath, String& path, String& fileName)
{
  path.clear();

  fileName = fullPath;

  // find last separator
  s32 pos = fullPath.lastIndexOf(PathSeparator());
  if (0 <= pos)
  {
    // store everything before separator as path
    path = fullPath.subString(0, pos);

    // store everything after separator as file name
    fileName = fullPath.subString(pos + 1);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String Directory::Join(const String& path1, const String& path2)
{
  String out = path1;

  // check if first path ends with separator already
  if (path1.endsWith(PathSeparator()))
  {
    // just add second path
    out += path2;
  }
  else
  {
    // add separator and second path
    out += (PathSeparator() + path2);
  }

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String Directory::PathSeparator()
{
  return String(KPathSeparator);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END