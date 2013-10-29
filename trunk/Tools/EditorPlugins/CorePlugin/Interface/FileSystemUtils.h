#ifndef CORE_FILESYSTEMUTILS_H
#define CORE_FILESYSTEMUTILS_H

/*! Helper class regarding File System area.
 */

#include <QString>
#include "CorePlugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class COREPLUGIN_API FileSystemUtils
{
  public:

    /*! Removes invalid characters from given file name.
     *  @param  fileName  Name to be validated.
     *  @return Validated name. This name does not contain any invalid characters.
     */
    static QString ValidateFileName(const QString& fileName);
    /*! Joins given path elements together.
     *  @param  path1 First path to join.
     *  @param  path2 Second path to join.
     *  @return Joint path.
     */
    static QString Join(const QString& path1, const QString& path2);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // CORE_FILESYSTEMUTILS_H
