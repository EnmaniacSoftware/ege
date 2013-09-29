#include "EGEDirectory.h"
#include <Windows.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String Directory::FromNativeSeparators(const String& path)
{
  String result = path;

  size_t pos = 0;
  while (std::string::npos != (pos = result.find("\\", pos)))
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
  TCHAR szPath[MAX_PATH];
  DWORD dwLength = GetCurrentDirectoryA(MAX_PATH, szPath);

  // convert to framework layout
  return FromNativeSeparators(String(szPath));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END