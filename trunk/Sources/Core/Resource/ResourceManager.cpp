#include <EGEResources.h>
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
#include "Core/Resource/ResourceManagerWorkThread.h"
#include "Core/Resource/ResourceSequencer.h"
#include "Core/Graphics/Font.h"
#include "Core/Debug/DebugFont.h"
#include "Core/Application/Application.h"
#include "Core/Event/Event.h"
#include "Core/Event/EventIDs.h"
#include "Core/Event/EventManager.h"
#include <EGEXml.h>
#include <EGEDir.h>
#include <EGEMutex.h>

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
                                                    { RESOURCE_NAME_SPRITE_ANIMATION, ResourceSpriteAnimation::Create },
                                                    { RESOURCE_NAME_CURVE, ResourceCurve::Create },
                                                    { RESOURCE_NAME_PARTICLE_EMITTER, ResourceParticleEmitter::Create },
                                                    { RESOURCE_NAME_PARTICLE_AFFECTOR, ResourceParticleAffector::Create },
                                                    { RESOURCE_NAME_TEXT, ResourceText::Create },
                                                    { RESOURCE_NAME_SOUND, ResourceSound::Create },
                                                    { RESOURCE_NAME_WIDGET, ResourceWidget::Create },
                                                    { RESOURCE_NAME_IMAGED_ANIMATION, ResourceImagedAnimation::Create },
                                                    { RESOURCE_NAME_SEQUENCE, ResourceSequencer::Create }
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManager::ResourceManager(Application* app) : Object(app),
                                                     m_state(STATE_INITIALIZING)
{
  // register build-in resource types
  for (u32 i = 0; i < sizeof (l_resourcesToRegister) / sizeof (BuiltInResource); ++i)
  {
    const BuiltInResource& resource = l_resourcesToRegister[i];

    registerResource(resource.name, resource.pfCreateFunc);
  }

  // create work thread
  m_workThread = ege_new ResourceManagerWorkThread(app);

  // create access mutex
  m_mutex = ege_new Mutex(app);

  // create wait condition
  m_commandsToProcess = ege_new WaitCondition(app);

  // create default resources
  createDefaultResources();

  // subscribe for event notifications
  app->eventManager()->addListener(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManager::~ResourceManager()
{
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

  return (NULL != group(DEFAULT_GROUP_NAME)) && (NULL != m_workThread) && m_workThread->isValid() && (NULL != m_mutex) && m_mutex->isValid() && 
         (NULL != m_commandsToProcess) && m_commandsToProcess->isValid() && app()->eventManager()->isListening(this);
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
/*! Adds resources from given file to repository. 
 * @param filePath    Path to resource definition file which is to be added into resources.
 * @param autoDetect  If TRUE file given by filePath will be looked for within currently registered data paths. Otherwise, filePath will be treated as
 *                    absolute path.
 */
EGEResult ResourceManager::addResources(String filePath, bool autoDetect)
{
  EGEResult result = EGE_SUCCESS;

  bool atLeastOneResourceAddedSucessfully = false;

  // convert separators
  filePath = Dir::FromNativeSeparators(filePath);

  EGE_PRINT("%s", filePath.toAscii());

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
    if (!resourcesNode->isValid())
    {
      // error!
      EGE_PRINT("ERROR: Resource file %s has no %s tag", fullPath.toAscii(), NODE_RESOURCES);
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
/*! Adds data directory. */
void ResourceManager::addDataDirectory(const String& path)
{
  if (!m_dataDirs.contains(path))
  {
    m_dataDirs << path;
  }
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
    EGE_PRINT("%s", newGroup->name().toAscii());

    // check if such group DOES NOT exists
    // NOTE: we quitely omit group duplicates so it is valid to ie. INCLUDE the same group multiple times
    if (NULL == group(newGroup->name()))
    {
      // add into pool
      m_groups.push_back(newGroup);
    }
    else
    {
      EGE_WARNING("Group %s already exists.", newGroup->name().toAscii());
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
/*! Loads group with given name. 
 * @param name  Group name to be loaded.
 * @return  Returns EGE_SUCCESS if group has been scheduled for loading. EGE_ERROR_ALREADY_EXISTS if group is already loaded. Otherwise, EGE_ERROR.  
 * @note  Given group, when found, is scheduled for loading rather than loaded immediately.
 */
EGEResult ResourceManager::loadGroup(const String& name)
{
  // find group of given name
  PResourceGroup theGroup = group(name);
  if (NULL != theGroup)
  {
   // MutexLocker locker(m_mutex);

    // check if given group is scheduled for unloading/loading already
    CommandDataList::iterator it;
    for (it = m_commands.begin(); it != m_commands.end(); ++it)
    {
      CommandData& commandData = *it;
      if (((COMMAND_UNLOAD_GROUP == commandData.command) || (COMMAND_LOAD_GROUP == commandData.command)) && (commandData.groupNames.front() == name))
      {
        // found, done
        break;
      }
    }

    // check if loaded already
    if (theGroup->isLoaded())
    {
      // check if it awaits unloading
      if ((it != m_commands.end()) && (COMMAND_UNLOAD_GROUP == (*it).command))
      {
        // remove it from command list
        m_commands.erase(it);
      }

      // cannot be loaded
      EGE_PRINT("%s is already loaded.", name.toAscii());
      return EGE_ERROR_ALREADY_EXISTS;
    }
    
    // check if it is awaiting loading already
    if ((it != m_commands.end()) && (COMMAND_LOAD_GROUP == (*it).command))
    {
      // do nothing
      EGE_PRINT("Group %s already scheduled. Skipping.", name.toAscii());
      return EGE_SUCCESS;
    }

    // add group for loading
    CommandData commandData;

    commandData.command = COMMAND_LOAD_GROUP;
    commandData.groupNames.push_back(name);

    // add dependancies
    if (buildDependacyList(commandData.groupNames, name))
    {
      // add to pool
      m_commands.push_back(commandData);
    }
    else
    {
      // error!
      return EGE_ERROR;
    }

    EGE_PRINT("%s scheduled for loading.", name.toAscii());
    return EGE_SUCCESS;
  }

  EGE_PRINT("ERROR: %s not found!", name.toAscii());
  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Unloads group with given name. */
void ResourceManager::unloadGroup(const String& name)
{
  // find group of given name
  PResourceGroup theGroup = group(name);
  if (NULL != theGroup)
  {
    //MutexLocker locker(m_mutex);

    // check if given group is scheduled for unloading/loading already
    CommandDataList::iterator it;
    for (it = m_commands.begin(); it != m_commands.end(); ++it)
    {
      CommandData& commandData = *it;
      if (((COMMAND_UNLOAD_GROUP == commandData.command) || (COMMAND_LOAD_GROUP == commandData.command)) && (commandData.groupNames.front() == name))
      {
        // found, done
        break;
      }
    }

    // check if unloaded already
    if (!theGroup->isLoaded())
    {
      // check if it awaits loading
      if ((it != m_commands.end()) && (COMMAND_LOAD_GROUP == (*it).command))
      {
        // remove it from command list
        m_commands.erase(it);
      }

      // done
      return;
    }
    
    // check if it is awaiting unloading already
    if ((it != m_commands.end()) && (COMMAND_UNLOAD_GROUP == (*it).command))
    {
      // do nothing
      return;
    }

    // add group for unloading
    CommandData commandData;

    commandData.command = COMMAND_UNLOAD_GROUP;
    commandData.groupNames.push_back(name);

    // add dependancies
    if (buildDependacyList(commandData.groupNames, name))
    {
      // add to pool
      m_commands.push_back(commandData);
    }
    else
    {
      // error!
      return;
    }

    EGE_PRINT("%s scheduled for unloading.", name.toAscii());
    return;
  }

  EGE_PRINT("%s not found!", name.toAscii());
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
/*! Returns material resource of a given name. Optionally, from given group only. */
PResourceMaterial ResourceManager::materialResource(const String& name, const String& groupName) const
{
  return resource(RESOURCE_NAME_MATERIAL, name, groupName);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns text resource of a given name. Optionally, from given group only. */
PResourceText ResourceManager::textResource(const String& name, const String& groupName) const
{
  return resource(RESOURCE_NAME_TEXT, name, groupName);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns sound resource of a given name. Optionally, from given group only. */
PResourceSound ResourceManager::soundResource(const String& name, const String& groupName) const
{
  return resource(RESOURCE_NAME_SOUND, name, groupName);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Processes include command from XML data. 
 *  
 *   @param  filePath  relative (with respect to resource root directory) path to resouce file containing the group definition.
 *   @param  tag       include element to process. 
 */
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

  EGE_PRINT("Including %s", path.toAscii());

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
/*! Updates object. */
void ResourceManager::update(const Time& time)
{
  // check if initialization is done
  if (STATE_INITIALIZING == m_state)
  {
    if (isValid())
    {
      // start work thread
      //m_workThread->start();

      // change state
      m_state = STATE_READY;
    }
  }
  else if (STATE_READY == m_state)
  {
  }
  else if (STATE_CLOSING == m_state)
  {
    if (m_workThread && (m_workThread->isFinished() || !m_workThread->isRunning()))
    {
      m_workThread        = NULL;
      m_commandsToProcess = NULL;
      m_mutex             = NULL;

      // set state
      m_state = STATE_CLOSED;
    }
  }

  if (!app()->isQuitting())
  {
    // go thru all groups to be loaded
    for (CommandDataList::iterator it = m_commands.begin(); it != m_commands.end(); )
    {
      CommandData& commandData = *it;

      // process according to command type
      if (COMMAND_LOAD_GROUP == commandData.command)
      {
        // process groups in back to front order
        for (StringList::reverse_iterator it = commandData.groupNames.rbegin(); it != commandData.groupNames.rend(); ++it)
        {
          // find group
          PResourceGroup groupResource = group(*it);

          // load group
          if ((NULL == groupResource) || (EGE_SUCCESS != groupResource->load()))
          {
            // error!
            // NOTE: emit error for main group
            emit groupLoadError(commandData.groupNames.front());
          }
          else
          {
            // emit completion if this is main group
            if (*it == commandData.groupNames.front())
            {
              emit groupLoadComplete(*it);
            }
          }
        }
      }
      else if (COMMAND_UNLOAD_GROUP == commandData.command)
      {
        // process groups in back to front order
        for (StringList::reverse_iterator it = commandData.groupNames.rbegin(); it != commandData.groupNames.rend(); ++it)
        {
          // find group
          PResourceGroup groupResource = group(*it);

          // unload group
          if (NULL != groupResource)
          {
            groupResource->unload();
          }
        }
      }
      else
      {
        EGE_ASSERT("Invalid command!");
      }

      // remove from pool
      it = m_commands.erase(it);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Builds dependancy list for a given group. */
bool ResourceManager::buildDependacyList(StringList& list, const String& groupName) const
{
  // find group
  PResourceGroup groupResource = group(groupName);
  if (NULL == groupResource)
  {
    // error!
    EGE_PRINT("ERROR: Could not find group: %s", groupName.toAscii());
    return false;
  }

  // go thru all dependancies
  for (StringList::const_iterator it = groupResource->dependancies().begin(); it != groupResource->dependancies().end(); ++it)
  {
    PResourceGroup groupResourceDependancy = group(*it);
    if (NULL == groupResourceDependancy)
    {
      // error!
      EGE_PRINT("ERROR: Could not find dependancy group: %s", (*it).toAscii());
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
      EGE_PRINT("Dependancy group already in list: %s. Circular dependancy possible.", (*it).toAscii());
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
/*! EventListener override. Event reciever. */
void ResourceManager::onEventRecieved(PEvent event)
{
  switch (event->id())
  {
    case EGE_EVENT_ID_CORE_QUIT_REQUEST:

      if ((STATE_CLOSING != m_state) && (STATE_CLOSED != m_state))
      {
        // do shouting down
        shutDown();
      }
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Shuts down. */
void ResourceManager::shutDown()
{
  // change state
  m_state = STATE_CLOSING;

  // remove all groups
  removeGroups();

  // request work thread stop
  m_workThread->stop(0);

  // make sure all threads starts work
  m_commandsToProcess->wakeAll();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
