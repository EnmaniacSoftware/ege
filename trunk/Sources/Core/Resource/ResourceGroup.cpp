#include "Core/Resource/ResourceGroup.h"
#include "Core/Resource/ResourceManager.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(ResourceGroup)
EGE_DEFINE_DELETE_OPERATORS(ResourceGroup)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceGroup::ResourceGroup(Application* app, ResourceManager* manager, const String& path) : Object(app), m_manager(manager), m_loaded(false), m_path(path)
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
    return EGE_ERROR_BAD_PARAM;
  }

  // go thru all children
  PXmlElement child = tag->firstChild();
  while (child->isValid())
  {
    // create resource instance
    PResource resource = manager()->createResource(child->name());
    if (resource)
    {
      // initialize from XML
      if (EGE_SUCCESS != (result = resource->create(manager()->makeFullPath(path()), child)))
      {
        // error!
        break;
      }

      // add into pool
      m_resources.insert(ResourcesMap::value_type(resource->typeName(), resource));
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
    // go thru all resources
    for (ResourcesMap::iterator it = m_resources.begin(); it != m_resources.end(); ++it)
    {
      PResource resource = it->second;

      // load resource
      if (EGE_SUCCESS != (result = resource->load()))
      {
        // error!
        break;
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
      // unload it
      it->second->unload();
    }

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
