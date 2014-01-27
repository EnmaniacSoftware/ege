#ifndef WORKSPACE_PROPERTYVALUEHELPER_H
#define WORKSPACE_PROPERTYVALUEHELPER_H

/*! Helper class to ease creation of PropertyDefinition values. */

#include "PropertyDefinition.h"
#include "WorkspacePlugin_global.h"

namespace NPropertyObject
{
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class WORKSPACEPLUGIN_API PropertyValueHelper
{
  public:

    /*! Creates value object for EInt type of property.
     *  @param  value Default value.
     *  @param  min   Minimal allowed value.
     *  @param  max   Maximal allowed value.
     *  @return Created value object.
     */
    static PropertyValueContainer CreateIntegerValue(int value, int min, int max);
    /*! Creates value object for EFilePath type property.
     *  @param  name          Default value.
     *  @param  filter        File filter.
     *  @param  fileMustExist Flag indicating if it is valid to point to non-existing files.
     *  @return Created value object.
     */
    static PropertyValueContainer CreateFilePathValue(const QString& name, const QString& filter, bool fileMustExist = true);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
}

#endif // WORKSPACE_PROPERTYVALUEHELPER_H
