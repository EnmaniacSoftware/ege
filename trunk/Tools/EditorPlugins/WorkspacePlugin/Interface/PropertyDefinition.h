#ifndef WORKSPACE_PROPERTYDEFINITION_H
#define WORKSPACE_PROPERTYDEFINITION_H

/*! This class represents an definition of property for Propery Object instances.
 *  Definition describes the hierarchy and type of properties describing the Property Object.
 */

#include <QList>
#include <QString>
#include <QVariant>
#include "WorkspacePlugin_global.h"

namespace NPropertyObject
{
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Available property types. */
enum PropertyType
{
  EInvalid = -1,
  EGroup,
  EString,
  EBool,
  EInt,             /*!< Data triplet: [0: int:value], [1: int:minValue], [2: int:maxValue]. */
  ESize,
  ERect,
  EEnum,            /*!< Data in pairs: [n: QString:text], [n+1: QIcon:icon]. */
  EFilePath,        /*!< Data triplet: [0: QString:Path], [1: QString:FileFilter], [2: bool:FileMustExist]. */
  EDirectoryPath
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Container for multi value propery types. */
typedef QList<QVariant> PropertyValueContainer;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class WORKSPACEPLUGIN_API PropertyDefinition
{
  public:

    PropertyDefinition(const QString& name, PropertyType type, const PropertyValueContainer& values, int defaultValueIndex = 0, bool readOnly = false);
    PropertyDefinition(const PropertyDefinition& other);
   ~PropertyDefinition();

    /*! Returns TRUE if property is valid. */
    bool isValid() const;

    /*! Returns property name. */
    const QString& name() const;
    /*! Returns property values. */
    const PropertyValueContainer& values() const;
    /*! Returns type of property. */
    PropertyType type() const;
    /*! Returns TRUE if property is read only. */
    bool isReadOnly() const;

    /*! Adds child property.
     *  @param  property  Property definition to be added to child list.
     *  @return TRUE on success.
     */
    bool addChildProperty(const PropertyDefinition& property);
    /*! Replaces child property.
     *  @param  propertyName  Name of an existing property to be replaced.
     *  @param  newProperty   New property.
     *  @return TRUE on success.
     */
    bool replaceChildProperty(const QString& propertyName, const PropertyDefinition& newProperty);
    /*! Finds child property definition.
     *  @param  name  Child property name to find.
     *  @return Pointer to found property. NULL if not found.
     *  @note Search is recursive.
     */
    PropertyDefinition findChildProperty(const QString& name) const;
    /*! Returns list of child properties. */
    const QList<PropertyDefinition>& children() const;
    /*! Returns index of default (visible) value.
     *  @note This only applies to certain property types where more than one value is allowed ie. enumerations.
     */
    int defaultValue() const;

  private:

    PropertyDefinition();

  private:

    /*! NOTE: DO NOT keep pointer to parent here. As this object is copied by value all the time, pointer will be made invalid in no time!!!! */

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
    /*! Child properties. */
    QList<PropertyDefinition> m_children;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
}

#endif // WORKSPACE_PROPERTYDEFINITION_H
