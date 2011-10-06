#include <EGEResources.h>
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/ResourceGroup.h"
#include "Core/Resource/ResourceTexture.h"
#include "Core/Resource/ResourceMaterial.h"
#include "Core/Resource/ResourceData.h"
#include "Core/Resource/ResourceFont.h"
#include "Core/Resource/ResourceTextureImage.h"
#include "Core/Resource/ResourceSpritesheet.h"
#include "Core/Resource/ResourceSprite.h"
#include "Core/Resource/ResourceCurve.h"
#include "Core/Resource/ResourceParticleEmitter.h"
#include "Core/Resource/ResourceParticleAffector.h"
#include "Core/Graphics/Font.h"
#include "Core/Debug/DebugFont.h"
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
struct BuiltInResource
{
  const char* name;
  egeResourceCreateFunc pfCreateFunc;
};

static BuiltInResource l_resourcesToRegister[] = {  { RESOURCE_NAME_TEXTURE, ResourceTexture::Create},
                                                    { RESOURCE_NAME_TEXTURE_IMAGE, ResourceTextureImage::Create},
                                                    { RESOURCE_NAME_MATERIAL, ResourceMaterial::Create },
                                                    { RESOURCE_NAME_FONT, ResourceFont::Create },
                                                    { RESOURCE_NAME_DATA, ResourceData::Create },
                                                    { RESOURCE_NAME_SPRITE_SHEET, ResourceSpritesheet::Create },
                                                    { RESOURCE_NAME_SPRITE, ResourceSprite::Create },
                                                    { RESOURCE_NAME_CURVE, ResourceCurve::Create },
                                                    { RESOURCE_NAME_PARTICLE_EMITTER, ResourceParticleEmitter::Create },
                                                    { RESOURCE_NAME_PARTICLE_AFFECTOR, ResourceParticleAffector::Create }
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManager::ResourceManager(Application* app) : Object(app)
{
  // register build-in resource types
  for (u32 i = 0; i < sizeof (l_resourcesToRegister) / sizeof (BuiltInResource); ++i)
  {
    const BuiltInResource& resource = l_resourcesToRegister[i];

    registerResource(resource.name, resource.pfCreateFunc);
  }

  createDefaultResources();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManager::~ResourceManager()
{
  removeGroups();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool ResourceManager::isValid() const
{
  // check if built-in resources are registered correctly
  for (u32 i = 0; i < sizeof (l_resourcesToRegister) / sizeof (BuiltInResource); ++i)
  {
    const BuiltInResource& resource = l_resourcesToRegister[i];

    if (!isResourceRegistered(resource.name))
    {
      // error!
      return false;
    }
  }

  return NULL != group(DEFAULT_GROUP_NAME);
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
    // check if such group DOES NOT   exists
    // NOTE: we quitely omit group duplicates so it is valid to ie. INCLUDE the same group multiple times
    if (NULL == group(newGroup->name()))
    {
      // add into pool
      m_groups.push_back(newGroup);
    }
    else
    {
      EGE_PRINT("Group %s already exists.", newGroup->name().toAscii());
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
    EGE_PRINT("ResourceManager::loadGroup: %s", name.toAscii());

    // load it
    return theGroup->load();
  }

  EGE_PRINT("ResourceManager::loadGroup: %s not found!", name.toAscii());
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
    EGE_PRINT("ResourceManager::unloadGroup: %s", name.toAscii());

    // unload it
    theGroup->unload();
  }
  else
  {
    EGE_PRINT("ResourceManager::unloadGroup: %s not found!", name.toAscii());
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
/*! Creates default resources. */
void ResourceManager::createDefaultResources()
{
  // create and add default resource group
  PResourceGroup group = ege_new ResourceGroup(app(), this, "", DEFAULT_GROUP_NAME);
  m_groups.push_back(group);

  // create debug font texture
  PTexture2D texture = ege_new Texture2D(app(), "debug-font");
  if (texture)
  {
    // create texture resource and manually assign texture to it
    PResourceTexture textureResource = ResourceTexture::Create(app(), this, texture->name(), texture);
    if (textureResource)
    {
      group->addResource(textureResource);
    }

    // wrap texture data into buffer and create texture from it
    DataBuffer textureData(DebugFontData, DEBUG_FONT_LEN);
    if (EGE_SUCCESS == texture->create(textureData))
    {
      // create font material and assign texture to it
      PMaterial material = ege_new Material(app());
      if (material)
      {
        // add empty render pass
        RenderPass* renderPass = material->addPass(NULL);
        if (renderPass)
        {
          renderPass->addTexture(texture);
          renderPass->setSrcBlendFactor(EGEGraphics::BF_ONE);
          renderPass->setDstBlendFactor(EGEGraphics::BF_ONE);

          // setup glyphs data (all 256)
          Map<Char, GlyphData> glyphs;
          const float32 cellSize = 1.0f / 16.0f;
          for (s32 i = 0; i < 256; i++)
          {
            GlyphData data;

            data.m_textureRect = Rectf((i % 16) * cellSize, (i / 16) * cellSize, cellSize, cellSize);
            data.m_width       = 16;

            glyphs.insert((Char) i, data);
          }

          // create font from glyphs
          PFont font = ege_new Font(app(), 16, glyphs);
          if (font)
          {
            // assign font material
            font->setMaterial(material);

            // create font resource and manually assign font to it
            PResourceFont fontResource = ResourceFont::Create(app(), this, texture->name(), font);
            if (fontResource)
            {
              group->addResource(fontResource);
            }
          }
        }
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes all groups. */
void ResourceManager::removeGroups()
{
  // remove all groups
  for (GroupList::iterator it = m_groups.begin(); it != m_groups.end();)
  {
    m_groups.erase(it++);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------