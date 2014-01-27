#include "PropertyDefinition.h"

using namespace NPropertyObject;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PropertyDefinition::PropertyDefinition() : m_type(EInvalid),
                                           m_readOnly(false),
                                           m_defaultValueIndex(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PropertyDefinition::PropertyDefinition(const QString& name, PropertyType type, const PropertyValueContainer& values, int defaultValueIndex, bool readOnly)
  : m_name(name),
    m_values(values),
    m_type(type),
    m_readOnly(readOnly),
    m_defaultValueIndex(defaultValueIndex)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PropertyDefinition::PropertyDefinition(const PropertyDefinition& other) : m_name(other.name()),
                                                                          m_values(other.values()),
                                                                          m_type(other.type()),
                                                                          m_readOnly(other.isReadOnly()),
                                                                          m_defaultValueIndex(other.defaultValue()),
                                                                          m_children(other.children())
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PropertyDefinition::~PropertyDefinition()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PropertyDefinition::isValid() const
{
  return (EInvalid != type());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const QString& PropertyDefinition::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const PropertyValueContainer& PropertyDefinition::values() const
{
  return m_values;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PropertyType PropertyDefinition::type() const
{
  return m_type;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PropertyDefinition::isReadOnly() const
{
  return m_readOnly;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PropertyDefinition::addChildProperty(const PropertyDefinition& property)
{
  bool result = false;

  // check if property with such name already exists
  PropertyDefinition child = findChildProperty(property.name());
  if ( ! child.isValid())
  {
    // add to pool
    m_children.push_back(property);

    // store result
    result = true;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PropertyDefinition::replaceChildProperty(const QString& propertyName, const PropertyDefinition& newProperty)
{
  bool result = false;

  // go thru all children
  for (int i = 0; i < m_children.size() && ! result; ++i)
  {
    // check if property found
    if (m_children.at(i).name() == propertyName)
    {
      // replace
      m_children[i] = newProperty;

      // set result
      result = true;
    }
    else
    {
      // try to replace in child properties
      result = m_children[i].replaceChildProperty(propertyName, newProperty);
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PropertyDefinition PropertyDefinition::findChildProperty(const QString& name) const
{
  PropertyDefinition property;

  // go thru all children
  for (int i = 0; i < m_children.size() && ! property.isValid(); ++i)
  {
    const PropertyDefinition& child = m_children.at(i);

    // check if found
    if (child.name() == name)
    {
      // found
      property = child;
    }
    else
    {
      // investigate children of the child
      property = child.findChildProperty(name);
    }
  }

  return property;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const QList<PropertyDefinition>& PropertyDefinition::children() const
{
  return m_children;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
int PropertyDefinition::defaultValue() const
{
  return m_defaultValueIndex;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
