#include "EGEDirectory.h"
#include <Windows.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String Directory::FromNativeSeparators(const String& path)
{
  String result = path;

  s32 pos = 0;
  while (0 <= (pos = result.indexOf("\\", pos)))
  {
    result.replace(pos, 1, PathSeparator());
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String Directory::Path(DirectoryType directory)
{
  EGE_UNUSED(directory);

  // get current directory
  CHAR szPath[MAX_PATH];
  DWORD dwLength = GetCurrentDirectoryA(MAX_PATH, szPath);

  // convert to framework layout
  return FromNativeSeparators(String(szPath));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END