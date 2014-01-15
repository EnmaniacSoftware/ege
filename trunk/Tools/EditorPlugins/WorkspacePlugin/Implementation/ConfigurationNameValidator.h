#ifndef WORKSPACE_CONFIGURATIONNAMEVALIDATOR_H
#define WORKSPACE_CONFIGURATIONNAMEVALIDATOR_H

#include <QString>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ConfigurationNameValidator
{
  public:

    /*! Returns validated name based on the supplied one. Removes all inappropriate characters in the process.
     *  @param  name  Name to be validated.
     *  @return Validated name.
     *  @note If given name is fully valid, returned name is exactly the same.
     */
    static QString Validate(const QString& name);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // WORKSPACE_CONFIGURATIONNAMEVALIDATOR_H
