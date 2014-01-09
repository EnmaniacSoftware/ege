#include "PropertyDefinition.h"

using namespace NPropertyObject;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PropertyDefinition::PropertyDefinition(const QString& name, PropertyType type, const QString& value, bool readOnly) : m_name(name),
                                                                                                                      m_value(value),
                                                                                                                      m_type(type),
                                                                                                                      m_readOnly(readOnly),
                                                                                                                      m_parent(NULL)
{

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PropertyDefinition::PropertyDefinition(const PropertyDefinition& other) : m_name(other.name()),
                                                                          m_value(other.value()),
                                                                          m_type(other.type()),
                                                                          m_readOnly(other.isReadOnly()),
                                                                          m_parent(other.parent()),
                                                                          m_children(other.children())
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PropertyDefinition::~PropertyDefinition()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const QString&PropertyDefinition::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const QString&PropertyDefinition::value() const
{
  return m_value;
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
const QList<PropertyDefinition>& PropertyDefinition::children() const
{
  return m_children;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
