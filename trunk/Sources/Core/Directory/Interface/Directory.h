#ifndef EGE_CORE_DIRECTORY_DIRECTORY_H
#define EGE_CORE_DIRECTORY_DIRECTORY_H

#include "EGE.h"
#include "EGEString.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Directory
{
  public:

    /*! Predefined directories. */
    enum EDirectory
    {
    };

    /*! Returns given path with converted sperators. */
    static String FromNativeSeparators(const String& path);
    /*! Decomposes full path into seperate path and file name. */
    static void DecomposePath(const String& fullPath, String& path, String& fileName);
    /*! Returns full path to one of the predefined directories. 
     *  @param  directory Directory type to which path is to be determined.
     *  @return Path to directory.
     */
    static String Path(EDirectory directory);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DIRECTORY_DIRECTORY_H