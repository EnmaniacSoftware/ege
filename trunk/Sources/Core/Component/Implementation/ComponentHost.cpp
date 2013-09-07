#include "Core/Component/Interface/ComponentHost.h"
#include "Core/Component/Interface/Component.h"

EGE_NAMESPACE

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
  std::pair<ComponentPool::const_iterator, ComponentPool::const_iterator> range = m_pool.equal_range(component->uid());

  // go thru all components and check if any has the same name
  for (ComponentPool::const_iterator it = range.first; it != range.second; ++it)
  {
    // check if component of a given name found
    if (it->second->name() == component->name())
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
  List<PComponent> list;

  m_pool.values(type, list);

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
