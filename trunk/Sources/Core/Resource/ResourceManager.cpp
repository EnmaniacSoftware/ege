#include "EGEResources.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/ResourceGroup.h"
#include "Core/Resource/ResourceTexture.h"
#include "Core/Resource/ResourceMaterial.h"
#include "Core/Resource/ResourceData.h"
#include "Core/Resource/ResourceFont.h"
#include "Core/Resource/ResourceTextureImage.h"
#include "Core/Resource/ResourceSpritesheet.h"
#include "Core/Resource/ResourceSpriteAnimation.h"
#include "Core/Resource/ResourceCurve.h"
#include "Core/Resource/ResourceParticleEmitter.h"
#include "Core/Resource/ResourceParticleAffector.h"
#include "Core/Resource/ResourceText.h"
#include "Core/Resource/ResourceSound.h"
#include "Core/Resource/ResourceWidget.h"
#include "Core/Resource/ResourceImagedAnimation.h"
#include "Core/Resource/ResourceSequencer.h"
#include "Core/Resource/ResourceShader.h"
#include "Core/Resource/ResourceProgram.h"
#include "Core/Graphics/Graphics.h"
#include "Core/Graphics/Render/RenderSystem.h"
#include "Core/Graphics/Font.h"
#include "Core/Debug/DebugFont.h"
#include "Core/Application/Application.h"
#include "Core/Event/Event.h"
#include "Core/Event/EventIDs.h"
#include "Core/Event/EventManager.h"
#include "EGEXml.h"
#include "EGEDir.h"

#if EGE_RESOURCEMANAGER_SINGLE_THREAD
#include "Core/Resource/SingleThread/ResourceManagerST_p.h"
#elif EGE_RESOURCEMANAGER_MULTI_THREAD
#include "Core/Resource/MultiThread/ResourceManagerMT_p.h"
#endif // EGE_RESOURCE_MANAGER_SINGLE_THREAD

EGE_NAMESPACE_BEGIN

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
                                                    { RESOURCE_NAME_SPRITE_ANIMATION, ResourceSpriteAnimation::Create },
                                                    { RESOURCE_NAME_CURVE, ResourceCurve::Create },
                                                    { RESOURCE_NAME_PARTICLE_EMITTER, ResourceParticleEmitter::Create },
                                                    { RESOURCE_NAME_PARTICLE_AFFECTOR, ResourceParticleAffector::Create },
                                                    { RESOURCE_NAME_TEXT, ResourceText::Create },
                                                    { RESOURCE_NAME_SOUND, ResourceSound::Create },
                                                    { RESOURCE_NAME_WIDGET, ResourceWidget::Create },
                                                    { RESOURCE_NAME_IMAGED_ANIMATION, ResourceImagedAnimation::Create },
                                                    { RESOURCE_NAME_SEQUENCE, ResourceSequencer::Create },
                                                    { RESOURCE_NAME_SHADER, ResourceShader::Create },
                                                    { RESOURCE_NAME_PROGRAM, ResourceProgram::Create }
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManager::ResourceManager(Application* app) : Object(app),
                                                     m_p(NULL),
                                                     m_totalResourcesToProcess(0),
                                                     m_processedResourcesCount(0)
{
  ege_connect(app, frameEnd, this, ResourceManager::onFrameEnd);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManager::~ResourceManager()
{
  ege_disconnect(app(), frameEnd, this, ResourceManager::onFrameEnd);

  // NOTE: all groups should be already removed
  EGE_ASSERT(m_groups.empty());

  EGE_DELETE(m_p);

  app()->eventManager()->removeListener(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceManager::construct()
{
  EGEResult result = EGE_SUCCESS;

  // allocate private
  m_p = ege_new ResourceManagerPrivate(this);
  if (NULL == m_p)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // construct private
  if (EGE_SUCCESS != (result = m_p->construct()))
  {
    // error!
    return result;
  }

  // register build-in resource types
  for (u32 i = 0; i < sizeof (l_resourcesToRegister) / sizeof (BuiltInResource); ++i)
  {
    const BuiltInResource& resource = l_resourcesToRegister[i];

    if (EGE_SUCCESS != (result = registerResource(resource.name, resource.pfCreateFunc)))
    {
      // error!
      egeCritical() << EGE_FUNC_INFO << "Could not register resource";
      return result;
    }
  }

  // create default resources
  if ( ! createDefaultResources())
  {
    // error!
    egeCritical() << EGE_FUNC_INFO << "Could not create default resources!";
    return EGE_ERROR;
  }

  // subscribe for event notifications
  if ( ! app()->eventManager()->addListener(this))
  {
    // error!
    egeCritical() << EGE_FUNC_INFO << "Could not register for notifications!";
    return EGE_ERROR;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceManager::registerResource(const String& typeName, egeResourceCreateFunc createFunc)
{
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
bool ResourceManager::isResourceRegistered(const String& typeName) const
{
  // check if resource with such a name exists already
  Map<String, ResourceRegistryEntry>::const_iterator it = m_registeredResources.find(typeName);
  return it != m_registeredResources.end();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PResource ResourceManager::createResource(const String& name, ResourceGroup* group)
{
  PResource resource;

  // check if resource with such a name exists already
  Map<String, ResourceRegistryEntry>::iterator it = m_registeredResources.find(name);
  if (it != m_registeredResources.end())
  {
    // create resource
    resource = it->second.m_createFunc(app(), group);
  }

  return resource;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceManager::addResources(String filePath, bool autoDetect)
{
  EGEResult result = EGE_SUCCESS;

  bool atLeastOneResourceAddedSucessfully = false;

  // convert separators
  filePath = Dir::FromNativeSeparators(filePath);

  egeDebug() << "Adding resources:" << filePath;

  // try to locate resource file in each data location
  // NOTE: if no AUTO-DETECTION is set we do exactly one search with a given filePath
  for (StringList::const_iterator it = m_dataDirs.begin(); (it != m_dataDirs.end() && ((it == m_dataDirs.begin() && !autoDetect) || autoDetect)); ++it)
  {
    String fullPath = autoDetect ? (*it + "/" + filePath) : filePath;

    XmlDocument xml;
    if (EGE_SUCCESS != (result = xml.load(fullPath)))
    {
      // try another data location
      continue;
    }

    // get main node
    PXmlElement resourcesNode = xml.firstChild(NODE_RESOURCES);
    if ((NULL == resourcesNode) || ! resourcesNode->isValid())
    {
      // error!
      egeWarning() << "Resource file" << fullPath << "has no" << NODE_RESOURCES << "tag";
      result = EGE_ERROR;
      break;
    }

    // process RESOURCES tag
    String path;
    String file;
    Dir::DecomposePath(fullPath, path, file);
    if (EGE_SUCCESS != (result = processResourcesTag(path, resourcesNode)))
    {
      // error!
      break;
    }

    // note this success
    atLeastOneResourceAddedSucessfully = true;
  }

  return atLeastOneResourceAddedSucessfully ? EGE_SUCCESS : result;
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
void ResourceManager::addDataDirectory(const String& path)
{
  if (!m_dataDirs.contains(path))
  {
    m_dataDirs << path;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
      result = processInclude(filePath, child);
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
EGEResult ResourceManager::addGroup(const String& filePath, const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  PResourceGroup newGroup = ege_new ResourceGroup(app(), this);
  if (NULL == newGroup)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }
  
  // create from XML
  result = newGroup->create(filePath, tag);
  if (EGE_SUCCESS == result)
  {
    egeDebug() << newGroup->name();

    // check if such group DOES NOT exists
    PResourceGroup existingGroup = group(newGroup->name());
    if (NULL == existingGroup)
    {
      // connect
      ege_connect(newGroup, resourceLoaded, p_func(), ResourceManagerPrivate::onResourceLoaded);
      ege_connect(newGroup, resourceUnloaded, p_func(), ResourceManagerPrivate::onResourceUnloaded);
      ege_connect(newGroup, resourceGroupLoaded, p_func(), ResourceManagerPrivate::onGroupLoaded);
      ege_connect(newGroup, resourceGroupUnloaded, p_func(), ResourceManagerPrivate::onGroupUnloaded);
  
      // add into pool
      m_groups.push_back(newGroup);
    }
    else
    {
      // try to override
      result = existingGroup->overrideBy(newGroup);
      if (EGE_ERROR_NOT_SUPPORTED == result)
      {
        // error!
        egeWarning() << "Attempt to override non-overridable group" << existingGroup->name();
      }
      else if (EGE_ERROR_ALREADY_EXISTS == result)
      {
        // NOTE: we quitely omit group duplicates so it is valid to ie. INCLUDE the same group multiple times
        egeWarning() << "Group" << newGroup->name() << "already exists. Skipping.";
        result = EGE_SUCCESS;
      }
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
EGEResult ResourceManager::loadGroup(const String& name)
{
  // check if can NOT accept loading
  if (STATE_READY != state())
  {
    return EGE_ERROR;
  }

  return p_func()->loadGroup(name);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManager::unloadGroup(const String& name)
{
  p_func()->unloadGroup(name);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
    for (GroupList::const_iterator it = m_groups.begin(); it != m_groups.end(); ++it)
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
bool ResourceManager::createDefaultResources()
{
  // create and add default resource group
  PResourceGroup group = ege_new ResourceGroup(app(), this, DEFAULT_GROUP_NAME);
  if (NULL == group)
  {
    // error!
    return false;
  }

  // wrap texture data into buffer and create texture from it
  DataBuffer textureData(DebugFontData, DEBUG_FONT_LEN);

  // create debug font texture
  PTexture2D texture = app()->graphics()->hardwareResourceProvider()->createTexture2D("debug-font", textureData);
  if (NULL == texture)
  {
    // error!
    return false;
  }

  // create texture resource and manually assign texture to it
  PResourceTexture textureResource = ResourceTexture::Create(app(), group, texture->name(), texture);
  if (NULL == textureResource)
  {
    // error!
    return false;
  }

  if (EGE_SUCCESS != group->addResource(textureResource))
  {
    // error!
    return false;
  }

  // create font material and assign texture to it
  PMaterial material = ege_new Material(app());
  if (NULL == material)
  {
    // error!
    return false;
  }

  // add empty render pass
  RenderPass* renderPass = material->addPass(NULL);
  if (NULL == renderPass)
  {
    // error!
    return false;
  }

  // create texture image
  PTextureImage textureImage = ege_new TextureImage(texture);
  if (EGE_SUCCESS != renderPass->addTexture(textureImage))
  {
    // error!
    return false;
  }

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

    glyphs.insert(static_cast<Char>(i), data);
  }

  // create font from glyphs
  PFont font = ege_new Font(app(), 16, glyphs);
  if (NULL == font)
  {
    // error!
    return false;
  }

  // assign font material
  font->setMaterial(material);

  // create font resource and manually assign font to it
  PResourceFont fontResource = ResourceFont::Create(app(), group, texture->name(), font);
  if (NULL == fontResource)
  {
    // error!
    return false;
  }

  if (EGE_SUCCESS != group->addResource(fontResource))
  {
    // error!
    return false;
  }

  // add to pool
  m_groups.push_back(group);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManager::destroyDefaultResources()
{
  PResourceGroup defaultGroup = group(DEFAULT_GROUP_NAME);
  if (NULL != defaultGroup)
  {
    List<PResource> resources = defaultGroup->resources("");

    // go thru all resources
    for (List<PResource>::iterator it = resources.begin(); it != resources.end(); ++it)
    {
      PResource resource = *it;

      if (RESOURCE_NAME_TEXTURE == resource->typeName())
      {
        PResourceTexture textureResource = resource;
        app()->graphics()->hardwareResourceProvider()->destroyTexture2D(textureResource->texture());
      }
    }

    // remove from pool
    m_groups.remove(defaultGroup);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManager::unloadAll()
{
  destroyDefaultResources();

  // go thru all groups
  for (GroupList::iterator it = m_groups.begin(); it != m_groups.end(); )
  {
    PResourceGroup group = *it;

    //if (group->name() == DEFAULT_GROUP_NAME)
    //{
    //  int a = 1;
    //}

    // disconnect
    ege_disconnect(group, resourceLoaded, p_func(), ResourceManagerPrivate::onResourceLoaded);
    ege_disconnect(group, resourceUnloaded, p_func(), ResourceManagerPrivate::onResourceUnloaded);
    ege_disconnect(group, resourceGroupLoaded, p_func(), ResourceManagerPrivate::onGroupLoaded);
    ege_disconnect(group, resourceGroupUnloaded, p_func(), ResourceManagerPrivate::onGroupUnloaded);

    // try to unload all resource one by one
    // NOTE: we need to go thru all resources instead of letting the group to unload itself cause it is possible (due to implementation)
    //       that some of the resources were loaded from outside (ie not thru PResourceGroup::load) ie when some other group refers 
    //       the resources from other group
    
    List<PResource> resources = group->resources("");
    for (List<PResource>::iterator itResource = resources.begin(); itResource != resources.end(); ++itResource)
    {
      PResource resource = *itResource;

      // try to unload
      resource->unload();
    }

    // NOTE: just to reset load flag
    group->unload();

    // try to unload
    //if (EGE_ERROR_ALREADY_EXISTS == group->unload())
    {
      // remove
      it = m_groups.erase(it);
      //continue;
    }

    // go to next
    //++it;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PResourceMaterial ResourceManager::materialResource(const String& name, const String& groupName) const
{
  return resource(RESOURCE_NAME_MATERIAL, name, groupName);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PResourceText ResourceManager::textResource(const String& name, const String& groupName) const
{
  return resource(RESOURCE_NAME_TEXT, name, groupName);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PResourceSound ResourceManager::soundResource(const String& name, const String& groupName) const
{
  return resource(RESOURCE_NAME_SOUND, name, groupName);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceManager::processInclude(const String& filePath, const PXmlElement& tag)
{
  bool error = false;

  // get data
  String path     = tag->attribute("path");
  bool autoDetect = tag->attribute("auto-detect", "false").toBool(&error);

  // check if obligatory data is wrong
  if (path.empty() || error)
  {
    // error!
    return EGE_ERROR;
  }

  egeDebug() << "Including" << path;

  // check if not autodetecting
  // NOTE: in this case we assume path is with respect to current directory
  if (!autoDetect)
  {
    // compose absolute path
    path = filePath + "/" + path;
  }

  // add new resource
  return addResources(path, autoDetect);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManager::update(const Time& time)
{
  p_func()->update(time);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceManager::buildDependacyList(StringList& list, const String& groupName) const
{
  // find group
  PResourceGroup groupResource = group(groupName);
  if (NULL == groupResource)
  {
    // error!
    egeWarning() << "Could not find group:" << groupName;
    return false;
  }

  // go thru all dependancies
  for (StringList::const_iterator it = groupResource->dependancies().begin(); it != groupResource->dependancies().end(); ++it)
  {
    PResourceGroup groupResourceDependancy = group(*it);
    if (NULL == groupResourceDependancy)
    {
      // error!
      egeWarning() << "Could not find dependancy group:" << (*it);
      return false;
    }

    // check if NOT in pool yet
    if (!list.contains(*it))
    {
      // add to pool
      list.push_back(*it);
    }
    else
    {
      egeWarning() << "Dependancy group already in list:" << *it << "Circular dependancy possible.";
      continue;
    }

    // process its dependancies
    if (!buildDependacyList(list, *it))
    {
      // error!
      return false;
    }
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManager::onEventRecieved(PEvent event)
{
  switch (event->id())
  {
    case EGE_EVENT_ID_CORE_QUIT_REQUEST:

      if ((STATE_CLOSING != p_func()->state()) && (STATE_CLOSED != p_func()->state()))
      {
        // do shouting down
        shutDown();
      }
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManager::shutDown()
{
  p_func()->shutDown();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManager::processCommands()
{
  p_func()->processCommands();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManager::ResourceProcessPolicy ResourceManager::resourceProcessPolicy() const
{
  return p_func()->resourceProcessPolicy();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManager::State ResourceManager::state() const
{
  return p_func()->state();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManager::onFrameEnd()
{
  if (STATE_READY == p_func()->state())
  {
    processCommands();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END