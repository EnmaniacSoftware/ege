#ifndef EGE_CORE_FILEUTILS_H
#define EGE_CORE_FILEUTILS_H

/*! Utility class for file related tasks.
 */

#include "EGE.h"
#include "EGEString.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class FileUtils
{
  public:

    /*! Checks if a file exists. 
     *  @param  filePath  Path to the file to be tested.
     *  @return TRUE if file at the given path exists. Otherwise, FALSE.
     */
    static bool Exists(const String& filePath);
    /*! Removes file at given path. 
     *  @param  filePath  Path to the file to be removed.
     *  @return TRUE if file at the given path has been removed. Otherwise, FALSE.
     */
    static bool Remove(const String& filePath);
    /*! Returns file size.
     *  @param  filePath  Path to the file which size is to be determined.
     *  @return Size of the file in bytes. In case of an error, negative value is returned. 
     */
    static s64 Size(const String& filePath);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_FILEUTILS_H