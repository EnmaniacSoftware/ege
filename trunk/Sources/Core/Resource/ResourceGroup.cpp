#include "Core/Resource/ResourceGroup.h"
#include "Core/Resource/ResourceManager.h"
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceGroup)
EGE_DEFINE_DELETE_OPERATORS(ResourceGroup)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define NODE_DEPENDANCY "dependancy"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceGroup::ResourceGroup(Application* app, ResourceManager* manager, const String& dirPath, const String& name) : Object(app), 
                                                                                                                      m_manager(manager), 
                                                                                                                      m_name(name), 
                                                                                                                      m_loaded(false), 
                                                                                                                      m_dirPath(dirPath)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceGroup::~ResourceGroup()
{
  destroy();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Initializes group from XML. */
EGEResult ResourceGroup::create(const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  // get data
  m_name = tag->attribute("name");

  // check if wrong data
  if (m_name.empty())
  {
    // error!
    EGE_PRINT("ERROR: Failed for name: %s", m_name.toAscii());
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
      PResource resource = manager()->createResource(child->name());
      if (resource)
      {
        // initialize from XML
        if (EGE_SUCCESS != (result = resource->create(m_dirPath, child)))
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
/*! Loads the group resources. */
EGEResult ResourceGroup::load()
{
  EGEResult result = EGE_SUCCESS;

  // check if NOT already loaded
  if (!isLoaded())
  {
    //if (name() == "main-menu-screen")
    //{
    //  int a = 1;
    //}

    // go thru all resources
    for (ResourcesMap::iterator it = m_resources.begin(); it != m_resources.end(); ++it)
    {
      PResource resource = it->second;

      // check if non-manual
      if (!resource->isManual())
      {
        if (resource->name() == "sprite-ripples")
        {
          int a = 1;
        }

        // load resource
        if (EGE_SUCCESS != (result = resource->load()))
        {
          // error!
          EGE_WARNING("Load failed: %s", resource->name().toAscii());
          break;
        }
      }
    }

    // check if error occured
    if (EGE_SUCCESS != result)
    {
      // unload entire group
      unload();
    }
    else
    {
      EGE_PRINT("%s loaded.", name().toAscii());

      // set flag
      m_loaded = true;
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Unloads the group resources */
void ResourceGroup::unload()
{
  // check if loaded
  if (isLoaded())
  {
    // go thru all resources
    for (ResourcesMap::const_iterator it = m_resources.begin(); it != m_resources.end(); ++it)
    {
      PResource resource = it->second;

      // check if non-manual
      if (!resource->isManual())
      {
        // unload it
        resource->unload();
      }
    }

    EGE_PRINT("%s unloaded.", name().toAscii());

    // reset flag
    m_loaded = false;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns resource of a given type and name. */
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
/*! Returns list of all resources of the given type. */
List<PResource> ResourceGroup::resources(const String& typeName) const
{
  List<PResource> list;

  // get all resources of a given type
  std::pair<ResourcesMap::const_iterator, ResourcesMap::const_iterator> range = m_resources.equal_range(typeName);

  // go thru all resources of a given type
  for (ResourcesMap::const_iterator it = range.first; it != range.second; ++it)
  {
    // add to list
    list.push_back(it->second);
  }

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Destroys group. */
void ResourceGroup::destroy()
{
  // unload first
  unload();

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
/*! Adds given resource to group. */
EGEResult ResourceGroup::addResource(const PResource& resource)
{
  m_resources.insert(resource->typeName(), resource);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds dependancy. */
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
    EGE_WARNING("Dependancy %s already exists.", name.toAscii());
  }
  else
  {
    // add dependancy
    m_dependancies << name;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END