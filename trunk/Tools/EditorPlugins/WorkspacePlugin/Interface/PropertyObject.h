#ifndef WORKSPACE_PROPERTYOBJECT_H
#define WORKSPACE_PROPERTYOBJECT_H

/*! This class represents an object which can be characterized by a number of properties.
 *  As such it can be displayed and manipulated from Properties Window interface.
 */

#include <QObject>
#include <QList>
#include "PropertyDefinition.h"
#include "WorkspacePlugin_global.h"

namespace NPropertyObject
{
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class WORKSPACEPLUGIN_API PropertyObject : public QObject
{
  Q_OBJECT

  public:

    PropertyObject();
   ~PropertyObject();

    /*! Returns list of property definitions describing the object. */
    virtual QList<PropertyDefinition> propertiesDefinition() const = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
}

#endif // WORKSPACE_PROPERTYOBJECT_H
