#include "Core/File/Interface/FileUtils.h"
#include <sys/stat.h>
#include <sys/types.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FileUtils::Exists(const String& filePath)
{
  bool exists = false;

  // try to open file
  FILE* file = NULL;
  if (0 == fopen_s(&file, filePath.c_str(), "r"))
  {
    // exists
    exists = true;

    // close file
    fclose(file);
  }
  
  return exists;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FileUtils::Remove(const String& filePath)
{
  return (0 == ::remove(filePath.c_str()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 FileUtils::Size(const String& filePath)
{
  struct _stat fileStatistics;

  s64 fileSize = -1;

  // get info about file
  int result = _stat(filePath.c_str(), &fileStatistics);
  if (0 == result)
  {
    fileSize = fileStatistics.st_size;
  }

  return fileSize;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END
