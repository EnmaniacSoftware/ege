#ifndef EDITOR_FSUTILS_H
#define EDITOR_FSUTILS_H

/*! Helper class regarding File System area.
 */

#include <QString>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class FSUtils
{
  public:

    /*! Removes invalid characters from given file name. */
    static QString ValidateFileName(const QString& fileName);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EDITOR_FSUTILS_H
