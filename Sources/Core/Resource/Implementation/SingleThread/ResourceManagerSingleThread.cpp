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
    processBatch();
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
        return EGE_ERROR_ALREADY_EXISTS;
      }
    }
  }

  ProcessingBatch batch;
  batch.load = true;

  // setup processing batch
  if ( ! initializeProcessingBatch(batch, name) || ! finalizeProcessingBatch(batch))
  {
    // error!
    egeWarning(KResourceManagerDebugName) << "Could not setup processing batch for group" << name;
    return EGE_ERROR;
  }

  // add to pool
  m_processList.push_back(batch);

  // update statistics
  m_totalResourcesToProcess += batch.resourcesCount;

  egeDebug(KResourceManagerDebugName) << "Group scheduled for loading:" << name << "Total resources:" << batch.resourcesCount;

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
  batch.load = false;

  // setup processing batch
  if ( ! initializeProcessingBatch(batch, name) || ! finalizeProcessingBatch(batch))
  {
    // error!
    egeWarning(KResourceManagerDebugName) << "Could not setup processing batch for group" << name;
    return;
  }

  // add to pool
  m_processList.push_back(batch);

  // update statistics
  m_totalResourcesToProcess += batch.resourcesCount;

  egeDebug(KResourceManagerDebugName) << "Group scheduled for unloading:" << name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END