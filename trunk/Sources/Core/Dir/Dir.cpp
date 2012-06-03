#include "Core/Dir/Dir.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns given path with converted sperators. */
String Dir::FromNativeSeparators(const String& path)
{
  String result = path;

  size_t pos = 0;
  while (std::string::npos != (pos = result.find("\\", pos)))
  {
    result.replace(pos, 1, "/");
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Decomposes full path into seperate path and file name. */
void Dir::DecomposePath(const String& fullPath, String& path, String& fileName)
{
  path.clear();

  fileName = fullPath;

  // find last separator
  size_t pos = fullPath.find_last_of("/");
  if (std::string::npos != pos)
  {
    // store everything before separator as path
    path = fullPath.substr(0, pos);

    // store everything after separator as file name
    fileName = fullPath.substr(pos + 1);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END