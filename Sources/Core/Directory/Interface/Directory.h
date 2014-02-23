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
    enum DirectoryType
    {
      EApplication = 0,     /*!< Application root directory. Usually not writable. */
      EDocuments,           /*!< Application documents directory. Writable. */
    };

  public:

    /*! Returns given path with converted sperators. */
    static String FromNativeSeparators(const String& path);
    /*! Decomposes full path into seperate path and file name. */
    static void DecomposePath(const String& fullPath, String& path, String& fileName);
    /*! Joins given path elements together.
     *  @param  path1 First path to join.
     *  @param  path2 Second path to join.
     *  @return Joint path.
     */
    static String Join(const String& path1, const String& path2);
    /*! Returns full path to one of the predefined directories. 
     *  @param  directory Directory type to which path is to be determined.
     *  @return Path to directory.
     */
    static String Path(DirectoryType directory);
    /*! Returns path separator. 
     *  @note This is platform independed separator used in entire framework.
     */
    static String PathSeparator();
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DIRECTORY_DIRECTORY_H