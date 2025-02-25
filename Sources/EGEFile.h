#ifndef EGE_FILE_H
#define EGE_FILE_H

#include "Core/Platform.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace EGEFile
{
  enum EMode
  {
    MODE_READ_ONLY = 0,
    MODE_WRITE_ONLY,
    MODE_APPEND
  };

  enum ESeekMode
  {
    SEEK_MODE_BEGIN = 0,
    SEEK_MODE_CURRENT,
    SEEK_MODE_END
  };
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Core/File/File.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_FILE_H