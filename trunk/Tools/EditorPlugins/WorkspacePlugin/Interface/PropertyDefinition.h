#ifndef WORKSPACE_PROPERTYDEFINITION_H
#define WORKSPACE_PROPERTYDEFINITION_H

/*! This class represents an definition of property for Propery Object instances.
 *  Definition describes the hierarchy and type of properties describing the Property Object.
 */

#include <QList>
#include <QString>
#include "WorkspacePlugin_global.h"

namespace NPropertyObject
{
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Available property types. */
enum PropertyType
{
  EGroup = 0,
  EString,
  EBool,
  EInt,
  ESize,
  ERect
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class WORKSPACEPLUGIN_API PropertyDefinition
{
  public:

    PropertyDefinition(const QString& name, PropertyType type, const QString& value = "", bool readOnly = false);
    PropertyDefinition(const PropertyDefinition& other);
   ~PropertyDefinition();

    /*! Returns property name. */
    const QString& name() const;
    /*! Returns property value. */
    const QString& value() const;
    /*! Returns type of property. */
    PropertyType type() const;
    /*! Returns TRUE if property is read only. */
    bool isReadOnly() const;
    /*! Returns parent property definition. */
    PropertyDefinition* parent() const;

    /*! Adds child property.
     *  @param  property  Property definition to be added to child list.
     *  @return TRUE if successfully added. Otherwise, FALSE.
     */
    bool addChildProperty(const PropertyDefinition& property);
    /*! Returns list of child properties. */
    const QList<PropertyDefinition>& children() const;

  private:

    /*! Property name. */
    QString m_name;
    /*! Property value. */
    QString m_value;
    /*! Type of property. */
    PropertyType m_type;
    /*! Read only flag. */
    bool m_readOnly;
    /*! Parent property.
     *  @note Can be NULL for parent GROUP property.
     */
    PropertyDefinition* m_parent;
    /*! Child properties. */
    QList<PropertyDefinition> m_children;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
}

#endif // WORKSPACE_PROPERTYDEFINITION_H
