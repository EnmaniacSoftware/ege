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

    /*! Removes invalid characters from given file name. */
    static QString ValidateFileName(const QString& fileName);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // CORE_FILESYSTEMUTILS_H
