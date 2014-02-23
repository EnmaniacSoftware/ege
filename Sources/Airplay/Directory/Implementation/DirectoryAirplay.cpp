#include "EGEDirectory.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String Directory::FromNativeSeparators(const String& path)
{
  // no conversion necessary as native matches the frameworks separators
  return path;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String Directory::Path(DirectoryType directory)
{
  EGE_UNUSED(directory);

  // TAGE - implement
  return String(".");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END