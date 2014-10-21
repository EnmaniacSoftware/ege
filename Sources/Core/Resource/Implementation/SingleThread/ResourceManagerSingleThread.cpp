#include "Core/Resource/Interface/SingleThread/ResourceManagerSingleThread.h"
#include "Core/Resource/Interface/ResourceGroup.h"
#include "EGEResources.h"
#include "EGETimer.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* KResourceManagerSingleThreadName = "single-thread";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceManagerSingleThread)
EGE_DEFINE_DELETE_OPERATORS(ResourceManagerSingleThread)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManagerSingleThread::ResourceManagerSingleThread(Engine& engine, IResourceLoader& loader) 
: ResourceManager(engine, loader)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManagerSingleThread::~ResourceManagerSingleThread()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerSingleThread::update(const Time& time)
{
  EGE_UNUSED(time)

  if ( ! m_processList.empty() && (EModuleStateRunning == state()))
  {
    ProcessingBatch& data = m_processList.front();

    PResourceGroup group = this->group(data.groups.front());

    // check if first try to load batch
    if (0 == data.startTime.microseconds())
    {
      // set timestamp
      data.startTime = Timer::GetMicroseconds();
    }

    if (data.load)
    {
      // try to load
      EGEResult result = group->load();

      // check if loaded already
      if (EGE_ERROR_ALREADY_EXISTS == result)
      {
        // process as loaded
        onGroupLoaded(group);
      }
      // check if error
      else if ((EGE_SUCCESS != result) && (EGE_WAIT != result))
      {
        // error!
        emit groupLoadError(group->name());
      }
    }
    else
    {
      // try to unload
      EGEResult result = group->unload();

      // check if unloaded already
      if (EGE_ERROR_ALREADY_EXISTS == result)
      {
        // process as unloaded
        onGroupUnloaded(group);
      }
    }
  }
  else if (EModuleStateShuttingDown == state())
  {
    // clean up
    // NOTE: this should be repeated until all groups are unloaded and removed
    unloadAll();

    if (m_groups.empty())
    {
      // done
      setState(EModuleStateClosed);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerSingleThread::processCommands()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceManagerSingleThread::loadGroup(const String& name)
{
  // check if already scheduled for processing
  for (ProcessingBatchList::iterator it = m_processList.begin(); it != m_processList.end(); ++it)
  {
    ProcessingBatch& batch = *it;

    if ((batch.groups.back() == name))
    {
      // check if scheduled for unloading
      if ( ! batch.load)
      {
        // update statistics
        m_totalResourcesToProcess -= batch.resourcesCount;

        // remove from pool
        m_processList.erase(it);
        return EGE_ERROR_ALREADY_EXISTS;
      }
      else
      {
        // already scheduled
        egeWarning(KResourceManagerDebugName) << "Group" << name << "already scheduled for loading!";
        return EGE_SUCCESS;
      }
    }
  }

  ProcessingBatch batch;
  batch.load            = true;
  batch.startTime       = 0LL;
  batch.resourcesCount  = 0;

  // build dependencies first
  // NOTE: this contains all dependant groups and itself at the end
  if ( ! buildDependacyList(batch.groups, name))
  {
    // error!
    egeWarning(KResourceManagerDebugName) << "Could not build dependancy list for group" << name;
    return EGE_ERROR;
  }

  batch.groups.push_back(name);

  // count resources
  for (StringList::const_iterator it = batch.groups.begin(); it != batch.groups.end(); ++it)
  {
    // find group of given name
    PResourceGroup group = this->group(*it);
    if (NULL == group)
    {
      // error!
      egeWarning(KResourceManagerDebugName) << "Group" << *it << "not found!";
      return EGE_ERROR;
    }

    // update resource count for batch
    batch.resourcesCount += group->resourceCount();
  }

  // add to pool
  m_processList.push_back(batch);

  // update statistics
  m_totalResourcesToProcess += batch.resourcesCount;

  egeDebug(KResourceManagerDebugName) << "Group scheduled for loading:" << name;

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerSingleThread::unloadGroup(const String& name)
{
  // check if already scheduled for processing
  for (ProcessingBatchList::iterator it = m_processList.begin(); it != m_processList.end(); ++it)
  {
    ProcessingBatch& batch = *it;

    if (batch.groups.back() == name)
    {
      // check if scheduled for loading
      if (batch.load)
      {
        // update statistics
        m_totalResourcesToProcess -= batch.resourcesCount;
       
        // remove from pool
        m_processList.erase(it);
        return;
      }
      else
      {
        // already scheduled
        egeWarning(KResourceManagerDebugName) << "Group" << name << "already scheduled for unloading!";
        return;
      }
    }
  }

  ProcessingBatch batch;
  batch.load            = false;
  batch.startTime       = 0LL;
  batch.resourcesCount  = 0;

  // build dependencies first
  // NOTE: this contains all dependant groups and itself at the end
  if ( ! buildDependacyList(batch.groups, name))
  {
    // error!
    egeWarning(KResourceManagerDebugName) << "Could not build dependancy list for group" << name;
    return;
  }

  batch.groups.push_back(name);

  // count resources
  for (StringList::const_iterator it = batch.groups.begin(); it != batch.groups.end(); ++it)
  {
    // find group of given name
    PResourceGroup group = this->group(*it);
    if (NULL == group)
    {
      // error!
      egeWarning(KResourceManagerDebugName) << "Group" << name << "not found!";
      return;
    }

    // update resource count for batch
    batch.resourcesCount += group->resourceCount();
  }

  // add to pool
  m_processList.push_back(batch);

  // update statistics
  m_totalResourcesToProcess += batch.resourcesCount;

  egeDebug(KResourceManagerDebugName) << "Group scheduled for unloading:" << name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManager::ResourceProcessPolicy ResourceManagerSingleThread::resourceProcessPolicy() const
{
  return ResourceManager::RLP_RESOURCE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END