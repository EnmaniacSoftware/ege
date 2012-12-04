#ifdef EGE_RESOURCE_MANAGER_SINGLE_THREAD

#include "Core/Resource/SingleThread/ResourceManagerST_p.h"
#include "Core/Resource/ResourceManager.h"
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceManagerPrivate)
EGE_DEFINE_DELETE_OPERATORS(ResourceManagerPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManagerPrivate::ResourceManagerPrivate(ResourceManager* base) : m_d(base)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManagerPrivate::~ResourceManagerPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Constructs object. */
EGEResult ResourceManagerPrivate::construct()
{
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates manager. */
void ResourceManagerPrivate::update(const Time& time)
{
  EGE_UNUSED(time)
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Locks resources. */
void ResourceManagerPrivate::lockResources()
{
  // do nothing
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Unlocks resources. */
void ResourceManagerPrivate::unlockResources()
{
  // do nothing
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_RESOURCE_MANAGER_SINGLE_THREAD