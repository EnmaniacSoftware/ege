#include "Core/Resource/ResourceGroup.h"
#include "Core/Resource/ResourceManager.h"
#include <EGETimer.h>
#include <EGEDebug.h>
#include <EGELog.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceGroup)
EGE_DEFINE_DELETE_OPERATORS(ResourceGroup)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define NODE_DEPENDANCY "dependancy"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceGroup::ResourceGroup(Application* app, ResourceManager* manager, const String& name) : Object(app), 
                                                                                               m_manager(manager), 
                                                                                               m_name(name), 
                                                                                               m_loaded(false), 
                                                                                               m_overridable(false)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceGroup::~ResourceGroup()
{
  // NOTE: at this stage group should be already unloaded
  EGE_ASSERT( ! isLoaded());

  destroy();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceGroup::create(const String& path,  const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  bool error = false;

  // get data
  m_name        = tag->attribute("name");
  m_overridable = tag->attribute("overridable", false);
  m_path        = path;

  // check if wrong data
  if (m_name.empty() || error)
  {
    // error!
    egeWarning() << "Failed for name:" << m_name;
    return EGE_ERROR_BAD_PARAM;
  }

  // go thru all children
  PXmlElement child = tag->firstChild();
  while (child->isValid())
  {
    // check if dependancy entry
    if (NODE_DEPENDANCY == child->name())
    {
      // add dependancy
      if (EGE_SUCCESS != (result = addDependancy(child)))
      {
        // error!
        break;
      }
    }
    else
    {
      // create resource instance
      PResource resource = manager()->createResource(child->name(), this);
      if (resource)
      {
        // initialize from XML
        if (EGE_SUCCESS != (result = resource->create(path, child)))
        {
          // error!
          break;
        }

        // add into pool
        if (EGE_SUCCESS != (result = addResource(resource)))
        {
          // error!
          break;
        }
      }
    }

    // go to next child
    child = child->nextChild();
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceGroup::load()
{
  EGEResult result = EGE_SUCCESS;

  if (name() == "atlas-misc-menus")
  {
    int a = 1;
  }

  // check if NOT already loaded
  if ( ! isLoaded())
  {
    // This flag is turned on if at least one of the resources reported busy after load
    bool resourceBusy = false;

    // go thru all resources
    for (ResourcesMap::iterator it = m_resources.begin(); it != m_resources.end(); ++it)
    {
      PResource resource = it->second;

      // check if non-manual and needs to be loaded
      if ( ! resource->isManual() && (IResource::STATE_LOADED != resource->state()))
      {
        // load resource
        result = resource->load();
        if ((EGE_SUCCESS != result) && (EGE_WAIT != result))
        {
          // error!
          egeWarning() << "Load failed:" << resource->name();
          break;
        }

        // check if loaded
        if (EGE_SUCCESS == result)
        {
          egeLog() << resource->name() << "of group" << name();

          // signal
          emit resourceLoaded(resource);
        }
        // check if resource is busy
        else if (EGE_WAIT == result)
        {
          // we will need another interation
          resourceBusy = true;
        }

        // check processing policy
        if (ResourceManager::RLP_RESOURCE == m_manager->resourceProcessPolicy())
        {
          // yield for now
          return EGE_WAIT;
        }
      }
    }

    // check if error occured
    if ((EGE_SUCCESS != result) && (EGE_WAIT != result))
    {
      // unload entire group
      unload();
    }
    else
    {
      // check if all resources reported they are loaded
      if ( ! resourceBusy)
      {
        // signal
        emit resourceGroupLoaded(*this);

        // set flag
        m_loaded = true;
      }
      else
      {
        // need to wait
        result = EGE_WAIT;
      }
    }
  }
  else
  {
    // already loaded
    result = EGE_ERROR_ALREADY_EXISTS;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceGroup::unload()
{
  EGEResult result = EGE_SUCCESS;

  if (name() == "main-menu-heart")
  {
    int a = 1;
  }

  // check if loaded
  if (isLoaded())
  {
    // go thru all resources
    for (ResourcesMap::const_iterator it = m_resources.begin(); it != m_resources.end(); ++it)
    {
      PResource resource = it->second;

      // check if non-manual
      if ( ! resource->isManual())
      {
        // unload it
        resource->unload();

        // check if loaded
        if (IResource::STATE_UNLOADED == resource->state())
        {
          // signal
          emit resourceUnloaded(resource);
        }
      }
    }

    // reset flag
    m_loaded = false;

    // signal
    emit resourceGroupUnloaded(*this);
  }
  else
  {
    // already unloaded
    result = EGE_ERROR_ALREADY_EXISTS;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PResource ResourceGroup::resource(const String& typeName, const String& name) const
{
  PResource resource;
  
  // get all resources of a given type
  std::pair<ResourcesMap::const_iterator, ResourcesMap::const_iterator> range = m_resources.equal_range(typeName);

  // go thru all resources of a given type
  for (ResourcesMap::const_iterator it = range.first; it != range.second; ++it)
  {
    // check if resource of a given name found
    if (it->second->name() == name)
    {
      // found
      resource = it->second;
      break;
    }
  }

  return resource;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
List<PResource> ResourceGroup::resources(const String& typeName) const
{
  List<PResource> list;

  // check if any type requested
  if ( ! typeName.empty())
  {
    // get all resources of a given type
    std::pair<ResourcesMap::const_iterator, ResourcesMap::const_iterator> range = m_resources.equal_range(typeName);

    // go thru all resources of a given type
    for (ResourcesMap::const_iterator it = range.first; it != range.second; ++it)
    {
      // add to list
      list.push_back(it->second);
    }
  }
  else
  {
    // add all resources
    for (ResourcesMap::const_iterator it = m_resources.begin(); it != m_resources.end(); ++it)
    {
      // add to list
      list.push_back(it->second);
    }
  }

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 ResourceGroup::resourceCount() const
{
  return static_cast<u32>(m_resources.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceGroup::destroy()
{
  // go thru all resources
  for (ResourcesMap::iterator it = m_resources.begin(); it != m_resources.end();)
  {
    // destroy it
    it->second = NULL;

    // remove from pool
    m_resources.erase(it++);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceGroup::addResource(const PResource& resource)
{
  // add to pool
  m_resources.insert(resource->typeName(), resource);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceGroup::addDependancy(const PXmlElement& tag)
{
  // get data
  String name = tag->attribute("name");

  // check if wrong data
  if (name.empty())
  {
    // error!
    return EGE_ERROR_BAD_PARAM;
  }

  // check if such dependancy already exists
  if (m_dependancies.contains(name))
  {
    egeWarning() << "Dependancy" << name << "already exists.";
  }
  else
  {
    // add dependancy
    m_dependancies << name;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceGroup::overrideBy(const PResourceGroup& group)
{
  // check if not overridable
  if ( ! m_overridable)
  {
    // cannot proceed
    return EGE_ERROR_NOT_SUPPORTED;
  }

  // check if groups point to the same resource file
  if (m_path == group->m_path)
  {
    // cannot proceed
    return EGE_ERROR_ALREADY_EXISTS;
  }

  // process resources
  for (ResourcesMap::const_iterator it = group->m_resources.begin(); it != group->m_resources.end(); ++it)
  {
    const PResource& incomingResource = it->second;

    // look for the resource of the same type and name
    PResource resource;
    for (ResourcesMap::iterator itRes = m_resources.begin(); itRes != m_resources.end(); ++itRes)
    {
      resource = itRes->second;
      if ((resource->name() == incomingResource->name()) && (resource->typeName() == incomingResource->typeName()))
      {
        // override
        egeWarning() << "Overriding resource" << incomingResource->name();
        itRes->second = incomingResource;
        break;
      }
      else
      {
        resource = NULL;
      }
    }

    // add new resource
    if (NULL == resource)
    {
      EGEResult result;
      if (EGE_SUCCESS != (result = addResource(incomingResource)))
      {
        // error!
        return result;
      }
    }
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& ResourceGroup::path() const
{
  return m_path;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END