#if EGE_RESOURCEMANAGER_MULTI_THREAD

#include "Core/Resource/MultiThread/ResourceManagerMT_p.h"
#include "Core/Resource/MultiThread/ResourceManagerWorkThread.h"
#include "Core/Resource/ResourceGroup.h"
#include <EGETimer.h>
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceManagerPrivate)
EGE_DEFINE_DELETE_OPERATORS(ResourceManagerPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManagerPrivate::ResourceManagerPrivate(ResourceManager* base) : m_d(base),
                                                                        m_state(ResourceManager::STATE_NONE)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManagerPrivate::~ResourceManagerPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceManagerPrivate::construct()
{
  // create work thread
  m_workThread = ege_new ResourceManagerWorkThread(d_func()->app(), this);
  if (NULL == m_workThread)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }
  ege_connect(m_workThread, finished, this, ResourceManagerPrivate::onWorkThreadFinished);

  // create access mutex
  m_mutex = ege_new Mutex(d_func()->app());
  if (NULL == m_mutex)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // create group emit resource mutex
  m_emitRequstsMutex = ege_new Mutex(d_func()->app());
  if (NULL == m_emitRequstsMutex)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // create wait condition
  m_commandsToProcess = ege_new WaitCondition(d_func()->app());
  if (NULL == m_commandsToProcess)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // start thread
  if ( ! m_workThread->start())
  {
    // error!
    return EGE_ERROR;
  }

  // set state
  m_state = ResourceManager::STATE_READY;

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerPrivate::update(const Time& time)
{
  EGE_UNUSED(time)

  if (ResourceManager::STATE_READY == m_state)
  {
    // check if any signals are to be emitted
    if ( ! m_emissionRequests.empty())
    {
      MutexLocker lock(m_emitRequstsMutex);

      // emit one by one
      for (EmissionRequestList::const_iterator it = m_emissionRequests.begin(); it != m_emissionRequests.end(); ++it)
      {
        const EmissionRequest& request = *it;

        switch (request.type)
        {
          case RT_GROUP_LOADED:
      
            emit d_func()->groupLoadComplete(request.groupName);
            break;

          case RT_GROUP_LOAD_ERROR:

            emit d_func()->groupLoadError(request.groupName);
            break;

          case RT_PROGRESS:

            egeCritical() << "Processed" << request.count << "out of" << request.total;

           // emit d_func()->processingStatusUpdated(request.count, request.total);
            break;

          default:

            egeWarning() << "Unknown request type. Skipping.";
            break;
        }
      }

      m_emissionRequests.clear();
    }
  }
  else if ((ResourceManager::STATE_CLOSING == m_state) && m_workThread->isFinished())
  {
    // clean up
    // NOTE: this should be repeated until all groups are unloaded and removed
    d_func()->unloadAll();

    if (d_func()->m_groups.empty())
    {
      // done
      m_state = ResourceManager::STATE_CLOSED;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerPrivate::processCommands()
{
  // check if anything to process
  if ( ! m_scheduledList.empty())
  {
    // wake up worker thread
    m_commandsToProcess->wakeOne();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceManagerPrivate::loadGroup(const String& name)
{
  // check if already scheduled for processing
  for (ProcessingBatchList::iterator it = m_scheduledList.begin(); it != m_scheduledList.end(); ++it)
  {
    ProcessingBatch& batch = *it;

    if ((batch.groups.back() == name))
    {
      // check if scheduled for unloading
      if ( ! batch.load)
      {
        // remove from pool
        m_mutex->lock();
        m_scheduledList.erase(it);
        m_mutex->unlock();

        return EGE_SUCCESS;
      }
      else
      {
        // already scheduled
        egeWarning() << "Group" << name << "already scheduled for loading!";
        return EGE_ERROR_ALREADY_EXISTS;
      }
    }
  }

  // add to pool
  ProcessingBatch batch;
  batch.groups          = StringList() << name;          
  batch.load            = true;
  batch.startTime       = 0LL;
  batch.resourcesCount  = 0;

  m_mutex->lock();
  m_scheduledList.push_back(batch);
  m_mutex->unlock();

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerPrivate::unloadGroup(const String& name)
{
  // check if already scheduled for processing
  for (ProcessingBatchList::iterator it = m_scheduledList.begin(); it != m_scheduledList.end(); ++it)
  {
    ProcessingBatch& batch = *it;

    if ((batch.groups.back() == name))
    {
      // check if scheduled for unloading
      if (batch.load)
      {
        // remove from pool
        m_mutex->lock();
        m_scheduledList.erase(it);
        m_mutex->unlock();

        return;
      }
      else
      {
        // already scheduled
        egeWarning() << "Group" << name << "already scheduled for loading!";
        return;
      }
    }
  }

  // add to pool
  ProcessingBatch batch;
  batch.groups          = StringList() << name;          
  batch.load            = false;
  batch.startTime       = 0LL;
  batch.resourcesCount  = 0;

  m_mutex->lock();
  m_scheduledList.push_back(batch);
  m_mutex->unlock();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerPrivate::threadUpdate()
{
  // check if nothing left to process
  // NOTE: stop processing when not ready (ie closing)
  while (m_pendingList.empty() && (ResourceManager::STATE_READY == m_state))
  {
    // check if no more data to process
    if (m_scheduledList.empty())
    {
      // wait for data
      m_mutex->lock();
      m_commandsToProcess->wait(m_mutex);
    }
    else
    {
      // just lock and retrieve data
      m_mutex->lock();
    }

    // copy scheduled data
    ProcessingBatchList newScheduledData(m_scheduledList);

    // clean up shared list
    m_scheduledList.clear();

    // unlock shared resource access
    m_mutex->unlock();

    // add scheduled batches to processing list
    appendBatchesForProcessing(newScheduledData);
  }

  processBatches();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerPrivate::appendBatchesForProcessing(ProcessingBatchList& batches)
{
  // add new data to processing list
  for (ProcessingBatchList::iterator it = batches.begin(); it != batches.end(); ++it)
  {
    ProcessingBatch& batch = *it;

    // build dependencies first
    // NOTE: this contains all dependant groups and itself at the end
    String groupName = batch.groups.back();
    batch.groups.clear();
    if ( ! d_func()->buildDependacyList(batch.groups, groupName))
    {
      // error!
      egeWarning() << "Could not build dependancy list for group" << groupName;
      continue;
    }

    batch.groups.push_back(groupName);

    // count resources
    for (StringList::const_iterator it = batch.groups.begin(); it != batch.groups.end(); ++it)
    {
      // find group of given name
      PResourceGroup group = d_func()->group(*it);
      if (NULL == group)
      {
        // error!
        egeWarning() << "Group" << *it << "not found!";
        continue;
      }

      // update resource count for batch
      batch.resourcesCount += group->resourceCount();
    }

    // update statistics
    d_func()->m_totalResourcesToProcess += batch.resourcesCount;

    // add to pending list
    m_pendingList.push_back(batch);

    egeDebug() << "Group scheduled for" << ((batch.load) ? "loading:" : "unloading:") << groupName;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerPrivate::processBatches()
{
  // add new data to processing list
  // NOTE: stop processing if not ready anymore (ie closing)
  while ( ! m_pendingList.empty() && (ResourceManager::STATE_READY == m_state))
  {
    ProcessingBatch& data = m_pendingList.front();

    PResourceGroup group = d_func()->group(data.groups.front());

    // check if first try to load batch
    if (0 == data.startTime.microseconds())
    {
      // set timestamp
      data.startTime = Timer::GetMicroseconds();
    }

    if (data.load)
    {
      // load resource
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
        // schedule error signal
        addGroupLoadErrorRequest(group->name());
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
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManager::ResourceProcessPolicy ResourceManagerPrivate::resourceProcessPolicy() const
{
  return ResourceManager::RLP_GROUP;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerPrivate::shutDown()
{
  // mark we are to be closed
  m_state = ResourceManager::STATE_CLOSING;
  
  // request stop
  m_workThread->stop(0);

  // wake up any awaiters
  m_commandsToProcess->wakeOne();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManager::State ResourceManagerPrivate::state() const
{
  return m_state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerPrivate::onWorkThreadFinished(const PThread& thread)
{
  EGE_UNUSED(thread);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerPrivate::addProgressRequest(u32 count, u32 total)
{
  EmissionRequest request;

  request.type  = RT_PROGRESS;
  request.count = count;
  request.total = total;

  MutexLocker lock(m_emitRequstsMutex);
  m_emissionRequests.push_back(request);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerPrivate::addGroupLoadedRequest(const String& groupName)
{
  EmissionRequest request;

  request.type  = RT_GROUP_LOADED;
  request.groupName = groupName;

  MutexLocker lock(m_emitRequstsMutex);
  m_emissionRequests.push_back(request);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerPrivate::addGroupLoadErrorRequest(const String& groupName)
{
  EmissionRequest request;

  request.type  = RT_GROUP_LOAD_ERROR;
  request.groupName = groupName;

  MutexLocker lock(m_emitRequstsMutex);
  m_emissionRequests.push_back(request);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerPrivate::onGroupLoaded(const PResourceGroup& group)
{
  ProcessingBatch& data = m_pendingList.front();

  EGE_ASSERT(group->name() == data.groups.front());
  
  // check if expected group has been loaded
  if (group->name() == data.groups.front())
  {
    egeDebug() << "Group loaded:" << group->name() << "in" << (Timer::GetMicroseconds() - data.startTime).miliseconds() << "ms.";

    // remove it from batch pool
    data.groups.pop_front();

    // check if no more groups to be processed
    if (data.groups.empty())
    {
      // remove from process list first
      m_pendingList.pop_front();

      // add request to signal
      addGroupLoadedRequest(group->name());

      // check if not more batches to process
      if (m_pendingList.empty())
      {
        // clean up statistics
        d_func()->m_totalResourcesToProcess = 0;
        d_func()->m_processedResourcesCount = 0;
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerPrivate::onGroupUnloaded(const PResourceGroup& group)
{
  ProcessingBatch& data = m_pendingList.front();

  EGE_ASSERT(group->name() == data.groups.front());

  // check if expected group has been loaded
  if (group->name() == data.groups.front())
  {
    egeDebug() << "Group unloaded:" << group->name() << "in" << (Timer::GetMicroseconds() - data.startTime).miliseconds() << "ms.";

    // remove it from batch pool
    data.groups.pop_front();

    // check if no more groups to be processed
    if (data.groups.empty())
    {
      // remove from process list first
      m_pendingList.pop_front();

      // check if not more batches to process
      if (m_pendingList.empty())
      {
        // clean up statistics
        d_func()->m_totalResourcesToProcess = 0;
        d_func()->m_processedResourcesCount = 0;
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerPrivate::onResourceLoaded(const PResource& resource)
{
  EGE_UNUSED(resource);

  // update statistics
  d_func()->m_processedResourcesCount++;

  // signal
  emit d_func()->processingStatusUpdated(d_func()->m_processedResourcesCount, d_func()->m_totalResourcesToProcess);

  egeDebug() << "Progress" << d_func()->m_processedResourcesCount << "/" << d_func()->m_totalResourcesToProcess;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerPrivate::onResourceUnloaded(const PResource& resource)
{
  EGE_UNUSED(resource);

  // update statistics
  d_func()->m_processedResourcesCount++;

  // signal
  emit d_func()->processingStatusUpdated(d_func()->m_processedResourcesCount, d_func()->m_totalResourcesToProcess);

  egeDebug() << "Progress" << d_func()->m_processedResourcesCount << "/" << d_func()->m_totalResourcesToProcess;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_RESOURCEMANAGER_MULTI_THREAD