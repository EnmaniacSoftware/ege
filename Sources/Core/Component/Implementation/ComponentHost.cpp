#include "Core/Component/Interface/ComponentHost.h"
#include "Core/Component/Interface/Component.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ComponentHost::ComponentHost()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ComponentHost::~ComponentHost()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ComponentHost::addComponent(const PComponent& component)
{
  EGEResult result = EGE_SUCCESS;

  // retrieve all attcahed components of the given type
  List<PComponent> values = m_pool.values(component->uid());

  // go thru all components and check if any has the same name
  for (List<PComponent>::ConstIterator it = values.begin(); it != values.end(); ++it)  
  {
    const PComponent& value = *it;

    // check if component of a given name found
    if (value->name() == component->name())
    {
      // found
      result = EGE_ERROR_ALREADY_EXISTS;
      break;
    }
  }

  // check if ok to add
  if (EGE_SUCCESS == result)
  {
    // add to pool
    m_pool.insert(component->uid(), component);
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
List<PComponent> ComponentHost::components(u32 type) const
{
  return m_pool.values(type);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PComponent ComponentHost::component(u32 type) const
{
  return m_pool.values(type).first(NULL);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PComponent ComponentHost::component(const String& name) const
{
  PComponent component;

  // go thru all registered components
  for (MultiMap<u32, PComponent>::ConstIterator it = m_pool.begin(); (it != m_pool.end()) && (NULL == component); ++it)
  {
    const PComponent& current = it->second;
    if (current->name() == name)
    {
      // found
      component = current;
    }
  }

  return component;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ComponentHost::removeComponent(const PComponent& component)
{
  m_pool.removeByValue(component);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END