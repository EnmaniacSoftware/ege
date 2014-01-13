#ifndef WORKSPACE_PROPERTYDEFINITION_H
#define WORKSPACE_PROPERTYDEFINITION_H

/*! This class represents an definition of property for Propery Object instances.
 *  Definition describes the hierarchy and type of properties describing the Property Object.
 */

#include <QList>
#include <QString>
#include <QMultiHash>
#include <QVariant>
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
  ERect,
  EEnum,
  EFilePath         /*!< Data in pairs: [index: Path], [index+1: File Filter]. */
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Container for multi value propery types.
 *  Key value corresponds to index of value.
 *  Value corresponds to actual value for a given index.
 *  @note It is valid for an index to have mutliple values ie. text and icon.
 */
typedef QMultiHash<int, QVariant> PropertyValueContainer;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class WORKSPACEPLUGIN_API PropertyDefinition
{
  public:

    PropertyDefinition(const QString& name, PropertyType type, const PropertyValueContainer& values, int defaultValueIndex = 0, bool readOnly = false);
    PropertyDefinition(const PropertyDefinition& other);
   ~PropertyDefinition();

    /*! Returns property name. */
    const QString& name() const;
    /*! Returns property values. */
    const PropertyValueContainer& values() const;
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
    /*! Returns index of default (visible) value.
     *  @note This only applies to certain property types where more than one value is allowed ie. enumerations.
     */
    int defaultValue() const;

  private:

    /*! Property name. */
    QString m_name;
    /*! Property values. */
    PropertyValueContainer m_values;
    /*! Type of property. */
    PropertyType m_type;
    /*! Read only flag. */
    bool m_readOnly;
    /*! Default value index. */
    int m_defaultValueIndex;
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
