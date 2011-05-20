#ifndef EGE_CORE_DIR_H
#define EGE_CORE_DIR_H

#include "EGE.h"
#include "EGEString.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Dir
{
  public:

    /* Returns given path with converted sperators. */
    static EGEString FromNativeSeparators(const EGEString& path);
    /* Decomposes full path into seperate path and file name. */
    static void DecomposePath(const EGEString& fullPath, EGEString& path, EGEString& fileName);
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DIR_H