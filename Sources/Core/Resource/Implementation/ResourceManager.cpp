#include "EGEResources.h"
#include "Core/Resource/Implementation/ResourceManager.h"
#include "Core/Resource/Implementation/DefaultGroup.h"
#include "EGEDirectory.h"
#include "EGEEngine.h"
#include "EGETimer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* KResourceManagerDebugName = "EGEResourceManager";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManager::ResourceManager(Engine& engine, IResourceLoader& loader) 
: m_engine(engine)
, m_resourceLoader(loader)
, m_totalResourcesToProcess(0)
, m_processedResourcesCount(0)
{
  ege_connect(&m_resourceLoader, signalGroupCreated, this, ResourceManager::onGroupCreated);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManager::~ResourceManager()
{
  // NOTE: all groups should be already removed
  EGE_ASSERT(m_groups.isEmpty());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceManager::construct()
{
  // set state
  setState(EModuleStateRunning);

  // create default resources
  if ( ! createDefaultResources())
  {
    // error!
    egeCritical(KResourceManagerDebugName) << EGE_FUNC_INFO << "Could not create default resources!";
    return EGE_ERROR;
  }

  return EngineModule::construct();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceManager::addResources(String filePath, bool autoDetect)
{
  EGEResult result = EGE_ERROR_NOT_FOUND;

  bool atLeastOneResourceAddedSucessfully = false;

  // convert separators
  filePath = Directory::FromNativeSeparators(filePath);

  egeDebug(KResourceManagerDebugName) << "Adding resources:" << filePath;

  // try to locate resource file in each data location
  // NOTE: if no AUTO-DETECTION is set we do exactly one search with a given filePath
  for (StringList::ConstIterator it = m_dataDirs.begin(); (it != m_dataDirs.end() && ((it == m_dataDirs.begin() && !autoDetect) || autoDetect)); ++it)
  {
    String fullPath = autoDetect ? Directory::Join(*it, filePath) : filePath;

    // try to add the resource
    result = m_resourceLoader.addResources(fullPath);

    // check if resource wasnt found
    if (EGE_ERROR_NOT_FOUND == result)
    {
      // try another data location
      continue;
    }
    else if (EGE_SUCCESS != result)
    {
      // some other error happend, we are done
      break;
    }

    // note this success
    atLeastOneResourceAddedSucessfully = true;
  }

  return atLeastOneResourceAddedSucessfully ? EGE_SUCCESS : result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManager::addDataDirectory(const String& path)
{
  if (!m_dataDirs.contains(path))
  {
    m_dataDirs << path;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PResourceGroup ResourceManager::group(const String& name) const
{
  PResourceGroup group;

  // go thru all groups
  for (List<PResourceGroup>::ConstIterator it = m_groups.begin(); it != m_groups.end(); ++it)
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
PResource ResourceManager::resource(const String& typeName, const String& name, const String& groupName) const
{
  // check if search is to be done exactly in the given group
  if (!groupName.isEmpty())
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
    for (GroupList::ConstIterator it = m_groups.begin(); it != m_groups.end(); ++it)
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
  // wrap default group into data buffer
  DataBuffer buffer(const_cast<void*>(reinterpret_cast<const void*>(KDefaultResourceGroup.toAscii())), KDefaultResourceGroup.length());

  // load is as XML
  XmlDocument document;
  if ((EGE_SUCCESS != document.load(buffer)) || (NULL == document.rootElement()) || ! document.rootElement().isValid())
  {
    // error!
    return false;
  }

  PResourceGroup newGroup = ege_new ResourceGroup(engine());
  if (NULL == newGroup)
  {
    // error!
    return false;
  }
  
  // locate group element
  PXmlElement groupTag = document.rootElement()->firstChild("group"/*NODE_GROUP*/);
  if ((NULL == groupTag) || ! groupTag->isValid())
  {
    // error!
    return false;
  }

  // create from XML
  if (EGE_SUCCESS != newGroup->create("", groupTag))
  {
    // error!
    return false;
  }

  // add group to the pool
  onGroupCreated(newGroup);

  return (EGE_SUCCESS == loadGroup(DEFAULT_GROUP_NAME));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManager::unloadAll()
{
  // go thru all groups
  for (GroupList::Iterator it = m_groups.begin(); it != m_groups.end(); )
  {
    PResourceGroup group = *it;

    // try to unload all resource one by one
    List<PResource> resources = group->resources("");
    for (List<PResource>::Iterator itResource = resources.begin(); itResource != resources.end(); ++itResource)
    {
      PResource resource = *itResource;

      // try to unload
      if (IResource::STATE_UNLOADED != resource->state())
      {
        resource->unload();
      }
    }

    // try to unload
    //if (EGE_ERROR_ALREADY_EXISTS == group->unload())
    {
      // remove
      it = m_groups.remove(it);
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
bool ResourceManager::buildDependacyList(StringList& list, const String& groupName) const
{
  // find group
  PResourceGroup groupResource = group(groupName);
  if (NULL == groupResource)
  {
    // error!
    egeWarning(KResourceManagerDebugName) << "Could not find group:" << groupName;
    return false;
  }

  // go thru all dependancies
  for (StringList::ConstIterator it = groupResource->dependancies().begin(); it != groupResource->dependancies().end(); ++it)
  {
    PResourceGroup groupResourceDependancy = group(*it);
    if (NULL == groupResourceDependancy)
    {
      // error!
      egeWarning(KResourceManagerDebugName) << "Could not find dependancy group:" << (*it);
      return false;
    }

    // check if NOT in pool yet
    if (!list.contains(*it))
    {
      // add to pool
      list.append(*it);
    }
    else
    {
      egeWarning(KResourceManagerDebugName) << "Dependancy group already in list:" << *it << "Circular dependancy possible.";
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
bool ResourceManager::initializeProcessingBatch(ProcessingBatch& batch, const String& groupName) const
{
  // clear up data
  batch.startTime         = 0LL;
  batch.resourcesCount    = 0;
  batch.nextResourceIndex = 0;

  // build dependencies first
  // NOTE: this contains all dependant groups only
  if ( ! buildDependacyList(batch.groups, groupName))
  {
    // error!
    egeWarning(KResourceManagerDebugName) << "Could not build dependancy list for group" << groupName;
    return false;
  }

  // add itself at the end
  batch.groups.append(groupName);

  // count resources
  for (StringList::ConstIterator it = batch.groups.begin(); it != batch.groups.end(); ++it)
  {
    // find group of given name
    PResourceGroup group = this->group(*it);
    if (NULL == group)
    {
      // error!
      egeWarning(KResourceManagerDebugName) << "Group" << groupName << "not found!";
      return false;
    }

    // update resource count for batch
    batch.resourcesCount += group->resourceCount();
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceManager::finalizeProcessingBatch(ProcessingBatch& batch) const
{
  // reserve space
  batch.resources.setCapacity(batch.resourcesCount);

  // go thru all groups
  for (StringList::ConstIterator it = batch.groups.begin(); it != batch.groups.end(); ++it)
  {
    // find group of given name
    PResourceGroup group = this->group(*it);
    if (NULL == group)
    {
      // error!
      egeWarning(KResourceManagerDebugName) << "Group" << *it << "not found!";
      return false;
    }

    // get list of all resources
    List<PResource> resources = group->resources("");

    // add them all into batch
    for (List<PResource>::ConstIterator itRes = resources.begin(); itRes != resources.end(); ++itRes)
    {
      batch.resources.append(*itRes);
    }
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManager::processBatch()
{
  EGE_ASSERT( ! m_processList.isEmpty());

  ProcessingBatch& batch = m_processList.first();

  // check if batch has been processed
  if (batch.resources.isEmpty())
  {
    // do nothing
    return;
  }

  // check if first try to load batch
  if (0 == batch.startTime.microseconds())
  {
    // set timestamp
    batch.startTime = Timer::GetMicroseconds();
  }

  // get next resource to process
  EGEResult result = EGE_SUCCESS;
  PResource resource = batch.resources.at(batch.nextResourceIndex, NULL);
  if (NULL != resource)
  {
    if (batch.load)
    {
      result = loadResource(resource);

      
      // check if not being processed
      if (EGE_WAIT != result)
      {
        // update statistics
        m_processedResourcesCount++;

        // notify
        handleResourceLoaded(resource, result, m_totalResourcesToProcess, m_processedResourcesCount);
      }
    }
    else
    {
      result = unloadResource(resource);

      // check if not being processed
      if (EGE_WAIT != result)
      {
        // update statistics
        m_processedResourcesCount++;

        // notify
        handleResourceUnloaded(resource, result, m_totalResourcesToProcess, m_processedResourcesCount);
      }
    }

    // go to next resource if not being processed
    if (EGE_WAIT != result)
    {
      egeDebug(KResourceManagerDebugName) << "Progress" << m_processedResourcesCount << "/" << m_totalResourcesToProcess;

      ++batch.nextResourceIndex;
    }
  }

  // notify group result
  // NOTE: notify in case of an error or when it is done
  //       Error occurs when 'result' is NOT one of the following:
  //       - EGE_SUCCESS which means resource has been successfully processed
  //       - EGE_WAIT which means resource is being processed but needs more time
  //       - EGE_ERROR_ALREADY_EXISTS which means resource has been found in the correct state already
  if (((EGE_SUCCESS != result) && (EGE_WAIT != result) && (EGE_ERROR_ALREADY_EXISTS != result)) || (NULL == resource))
  {
    // mark as processed
    batch.resources.clear();

    PResourceGroup group = this->group(batch.groups.last(""));
    EGE_ASSERT(NULL != group);
    EGE_ASSERT(group->name() == batch.groups.last(""));
  
    // caluclate batch processing time
    const Time processingTime = Timer::GetMicroseconds() - batch.startTime;

    // check if group is completely processed
    const bool completelyProcessed = (group->name() == batch.groups.last(""));

    // store load/unload flag
    // NOTE: this is necessary due to the fact that m_processList can get emptied below removing the object
    const bool wasLoading = batch.load;

    // NOTE: DO NOT USE 'batch' object below this line!!!

    // first remove it from internal queues
    // NOTE: do it here immediately, to allow another requests to be added during the signaling operation
    if (completelyProcessed)
    {
      // remove from process list first
      m_processList.removeFirst();

      // check if not more batches to process
      if (m_processList.isEmpty())
      {
        // clean up statistics
        m_totalResourcesToProcess = 0;
        m_processedResourcesCount = 0;
      }
    }

    // signal group completion if necessary
    if (completelyProcessed)
    {
      if (wasLoading)
      {
        egeDebug(KResourceManagerDebugName) << "Group loaded:" << group->name() << "in" << processingTime.miliseconds() << "ms.";

        // notify
        onGroupLoaded(group, (NULL == resource) ? EGE_SUCCESS : result);
      }
      else
      {
        egeDebug(KResourceManagerDebugName) << "Group unloaded:" << group->name() << "in" << processingTime.miliseconds() << "ms.";

        // notify
        onGroupUnloaded(group, (NULL == resource) ? EGE_SUCCESS : result);
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Engine& ResourceManager::engine() const
{
  return m_engine;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManager::update(const Time& time)
{
  EGE_UNUSED(time);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManager::onShutdown()
{
  if ((EModuleStateClosed != state()) && (EModuleStateShuttingDown != state()))
  {
    // set state
    setState(EModuleStateShuttingDown);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 ResourceManager::uid() const
{
  return EGE_OBJECT_UID_RESOURCE_MANAGER_MODULE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManager::onGroupLoaded(const PResourceGroup& group, EGEResult result)
{
  // signal
  emit signalGroupLoaded(group->name(), result);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManager::onGroupUnloaded(const PResourceGroup& group, EGEResult result)
{
  // signal
  emit signalGroupUnloaded(group->name(), result);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManager::handleResourceLoaded(const PResource& resource, EGEResult result, s32 totalResourceCount, s32 processedResourceCount)
{
  EGE_UNUSED(resource)
  EGE_UNUSED(result)

  // signal
  emit signalProgress(processedResourceCount, totalResourceCount);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManager::handleResourceUnloaded(const PResource& resource, EGEResult result, s32 totalResourceCount, s32 processedResourceCount)
{
  EGE_UNUSED(resource)
  EGE_UNUSED(result)

  // signal
  emit signalProgress(processedResourceCount, totalResourceCount);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManager::onGroupCreated(const PResourceGroup& group)
{
  // add into pool
  m_groups.append(group);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceManager::loadResource(PResource& resource) const
{
  EGEResult result = EGE_ERROR_ALREADY_EXISTS;

  // check if resource requires loading
  if ( ! resource->isManual() && (IResource::STATE_LOADED != resource->state()))
  {
    result = resource->load();
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceManager::unloadResource(PResource& resource) const
{
  EGEResult result = EGE_ERROR_ALREADY_EXISTS;

  // check if resource requires unloading
  if ( ! resource->isManual() && (IResource::STATE_UNLOADED != resource->state()))
  {
    // try to unload
    /*result = */resource->unload();

    result = (IResource::STATE_UNLOADED == resource->state()) ? EGE_SUCCESS : EGE_ERROR;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END