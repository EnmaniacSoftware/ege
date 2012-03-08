#ifndef FSUTILS_H
#define FSUTILS_H

/*! Helper class regarding File System area.
 */

#include <QString>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class FSUtils
{
  public:

    /* Removes invalid characters from given file name. */
    static QString ValidateFileName(const QString& fileName);
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // FSUTILS_H