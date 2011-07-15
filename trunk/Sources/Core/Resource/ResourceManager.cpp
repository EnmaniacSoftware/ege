#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/ResourceGroup.h"
#include "Core/Resource/ResourceTexture.h"
#include "Core/Resource/ResourceMaterial.h"
#include "Core/Resource/ResourceData.h"
#include "Core/Resource/ResourceFont.h"
#include "Core/Graphics/Font.h" 
#include <EGEXml.h>
#include <EGEDir.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(ResourceManager)
EGE_DEFINE_DELETE_OPERATORS(ResourceManager)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#define NODE_RESOURCES "resources"
#define NODE_GROUP     "group"
#define NODE_INCLUDE   "include"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManager::ResourceManager(Application* app) : Object(app)
{
  // register build-in resource types
  registerResource("texture", ResourceTexture::Create);
  registerResource("material", ResourceMaterial::Create);
  registerResource("data", ResourceData::Create);
  registerResource("font", ResourceFont::Create);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManager::~ResourceManager()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool ResourceManager::isValid() const
{
  return isResourceRegistered("texture") && isResourceRegistered("material") && isResourceRegistered("data") && isResourceRegistered("font");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Registeres custom resource type. */
EGEResult ResourceManager::registerResource(const String& typeName, egeResourceCreateFunc createFunc)
{
  EGEResult result = EGE_SUCCESS;
  
  // check if resource with such a name exists already
  Map<String, ResourceRegistryEntry>::iterator it = m_registeredResources.find(typeName);
  if (it != m_registeredResources.end())
  {
    // error!
    return EGE_ERROR_ALREADY_EXISTS;
  }

  // register
  ResourceRegistryEntry entry;
  entry.m_createFunc  = createFunc;

  m_registeredResources.insert(typeName, entry);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if given resource type is registered. */
bool ResourceManager::isResourceRegistered(const String& typeName) const
{
  // check if resource with such a name exists already
  Map<String, ResourceRegistryEntry>::const_iterator it = m_registeredResources.find(typeName);
  return it != m_registeredResources.end();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of resource of the type given by name. */
PResource ResourceManager::createResource(const String& name)
{
  PResource resource;

  // check if resource with such a name exists already
  Map<String, ResourceRegistryEntry>::iterator it = m_registeredResources.find(name);
  if (it != m_registeredResources.end())
  {
    // create resource
    resource = it->second.m_createFunc(app(), this);
  }

  return resource;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds resources from given file to repository. */
EGEResult ResourceManager::addResources(String fileName)
{
  EGEResult result = EGE_SUCCESS;

  // convert separators
  fileName = Dir::FromNativeSeparators(fileName);

  XmlDocument xml;
  if (EGE_SUCCESS != (result = xml.load(makeFullPath(fileName))))
  {
    // error!
    return result;
  }

  // get main node
  PXmlElement resourcesNode = xml.firstChild(NODE_RESOURCES);
  if (!resourcesNode->isValid())
  {
    // error!
    return EGE_ERROR;
  }

  // process RESOURCES tag
  String path;
  String file;
  Dir::DecomposePath(fileName, path, file);
  result = processResourcesTag(path, resourcesNode);

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds resources from given buffer. */
//EGEResult ResourceManager::addResources(const PDataBuffer& buffer)
//{
//  EGEResult result = EGE_SUCCESS;
//
//  XmlDocument xml;
//  if (EGE_SUCCESS != (result = xml.load(buffer)))
//  {
//    // error!
//    return result;
//  }
//
//  // get main node
//  PXmlElement resourcesNode = xml.firstChild(NODE_RESOURCES);
//  if (!resourcesNode->isValid())
//  {
//    // error!
//    return EGE_ERROR;
//  }
//
//  // process RESOURCES tag
//  result = processResourcesTag(resourcesNode);
//
//  return result;
//}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets root directory for resources */
void ResourceManager::setRootDirectory(const String& rootDir)
{
  m_rootDir = rootDir;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Composes full path to given resource */
String ResourceManager::makeFullPath(const String& localPath) const
{
  if (!m_rootDir.empty())
  {
    if (localPath.empty())
    {
      return m_rootDir;
    }

    return m_rootDir + "/" + localPath;
  }

  return localPath;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/** Processes the RESOURCES tag.
*  
*   \param  filePath  relative (with respect to resource root directory) path to resouce file.
*   \param  tag       resource element to process. 
*/
EGEResult ResourceManager::processResourcesTag(const String& filePath, const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  // go thru all children
  PXmlElement child = tag->firstChild();
  while (child->isValid())
  {
    // get child name
    const String childName = child->name();

    // process GROUP tag
    if (NODE_GROUP == childName)
    {
      result = addGroup(filePath, child);
    }
    // process INCLUDE tag
    else if (NODE_INCLUDE == childName)
    {
      String path = child->attribute("path");

      // check if obligatory data is wrong
      if (path.empty())
      {
        // erro!
        break;
      }

      // compose absolute path
      path = filePath + "/" + path;

      // add new resource
      result = addResources(path);
    }

    // check if error occured
    if (EGE_SUCCESS != result)
    {
      // we r done
      break;
    }

    // go to next child
    child = child->nextChild();
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/** Add new group from XML data.
*  
*   \param  filePath  relative (with respect to resource root directory) path to resouce file containing the group definition.
*   \param  tag       group element to process. 
*/
EGEResult ResourceManager::addGroup(const String& filePath, const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  PResourceGroup newGroup = ege_new ResourceGroup(app(), this, filePath);
  if (NULL == newGroup)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }
  
  // create from XML
  result = newGroup->create(tag);
  if (EGE_SUCCESS == result)
  {
    // check if such group DOES NOT exists
    // NOTE: we quitely omit group duplicates so it is valid to ie. INCLUDE the same group multiple times
    if (NULL == group(newGroup->name()))
    {
      // add into pool
      m_groups.push_back(newGroup);
    }
    else
    {
      EGE_PRINT(String::Format("Group %s already exists.", newGroup->name().toAscii()));
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Gets group of the given name. */
PResourceGroup ResourceManager::group(const String& name) const
{
  PResourceGroup group;

  // go thru all groups
  for (List<PResourceGroup>::const_iterator it = m_groups.begin(); it != m_groups.end(); ++it)
  {
    // check if proper group has been found
    if ((*it)->name() == name)
    {
      // we r done
      group = *it;
      break;
    }
  }

  return group;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Loads group with given name. */
EGEResult ResourceManager::loadGroup(const String& name)
{
  // get group of given name
  PResourceGroup theGroup = group(name);
  if (NULL != theGroup)
  {
    // load it
    return theGroup->load();
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Unloads group with given name. */
void ResourceManager::unloadGroup(const String& name)
{
  // get group of given name
  PResourceGroup theGroup = group(name);
  if (NULL != theGroup)
  {
    // unload it
    theGroup->unload();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns resource of a given type and name. Optionally, from given group only. */
PResource ResourceManager::resource(const String& typeName, const String& name, const String& groupName) const
{
  // check if search is to be done exactly in the given group
  if (!groupName.empty())
  {
    PResourceGroup groupRes = group(groupName);
    if (groupRes)
    {
      return groupRes->resource(typeName, name);
    }
  }
  else
  {
    // go thru all groups
    for (List<PResourceGroup>::const_iterator it = m_groups.begin(); it != m_groups.end(); ++it)
    {
      PResource resource = (*it)->resource(typeName, name);
      if (resource)
      {
        // found
        return resource;
      }
    }
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
