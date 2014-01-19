#include "PropertyDefinition.h"

using namespace NPropertyObject;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PropertyDefinition::PropertyDefinition(const QString& name, PropertyType type, const PropertyValueContainer& values, int defaultValueIndex, bool readOnly)
  : m_name(name),
    m_values(values),
    m_type(type),
    m_readOnly(readOnly),
    m_defaultValueIndex(defaultValueIndex),
    m_parent(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PropertyDefinition::PropertyDefinition(const PropertyDefinition& other) : m_name(other.name()),
                                                                          m_values(other.values()),
                                                                          m_type(other.type()),
                                                                          m_readOnly(other.isReadOnly()),
                                                                          m_defaultValueIndex(other.defaultValue()),
                                                                          m_parent(other.parent()),
                                                                          m_children(other.children())
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PropertyDefinition::~PropertyDefinition()
{
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
PropertyValueContainer& PropertyDefinition::values()
{
  return m_values;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PropertyType PropertyDefinition::type() const
{
  return m_type;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PropertyDefinition::setReadOnlyEnabled(bool set)
{
  m_readOnly = set;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PropertyDefinition::isReadOnly() const
{
  return m_readOnly;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PropertyDefinition* PropertyDefinition::parent() const
{
  return m_parent;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PropertyDefinition::addChildProperty(const PropertyDefinition& property)
{
  bool result = true;

  // check if property with a given name exists already
  foreach (const PropertyDefinition& child, m_children)
  {
    if (child.name() == property.name())
    {
      // cannot add
      result = false;
      break;
    }
  }

  // check if no error so far
  if (result)
  {
    // add to pool
    m_children.push_back(property);

    // update parent
    m_children.last().m_parent = this;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PropertyDefinition* PropertyDefinition::findChildProperty(const QString& name)
{
  PropertyDefinition* property = NULL;

  // go thru all children
  for (int i = 0; i < m_children.size(); ++i)
  {
    PropertyDefinition* child = &m_children[i];

    // check if found
    if (child->name() == name)
    {
      // found
      property = child;
    }
    else
    {
      // investigate children of the child
      property = child->findChildProperty(name);
    }

    // check if found
    if (NULL != property)
    {
      // done
      break;
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
