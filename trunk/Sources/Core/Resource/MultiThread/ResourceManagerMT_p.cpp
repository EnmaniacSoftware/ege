#ifdef EGE_RESOURCE_MANAGER_MULTI_THREAD

#include "Core/Resource/MultiThread/ResourceManagerMT_p.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/ResourceGroup.h"
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
/*! Processes commands. */
void ResourceManagerPrivate::processCommands()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Loads group with given name. 
 *  @param name  Group name to be loaded.
 *  @return  Returns EGE_SUCCESS if group has been scheduled for loading. EGE_ERROR_ALREADY_EXISTS if group is already loaded. Otherwise, EGE_ERROR.
 *  @note  Given group, when found, is scheduled for loading rather than loaded immediately.
 */
EGEResult ResourceManagerPrivate::loadGroup(const String& name)
{
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Unloads group with given name. */
void ResourceManagerPrivate::unloadGroup(const String& name)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_RESOURCE_MANAGER_MULTI_THREAD