#include "Core/Resource/Implementation/MultiThread/ResourceManagerMultiThread.h"
#include "Core/Resource/Implementation/MultiThread/ResourceManagerWorkThread.h"
#include "Core/Resource/Interface/ResourceGroup.h"
#include "EGEResources.h"
#include "EGETimer.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* KResourceManagerMultiThreadName = "multi-thread";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceManagerMultiThread)
EGE_DEFINE_DELETE_OPERATORS(ResourceManagerMultiThread)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManagerMultiThread::ResourceManagerMultiThread(Engine& engine) 
: ResourceManager(engine)
{
  // create work thread
  m_workThread = ege_new ResourceManagerWorkThread(this);

  // create access mutex
  m_mutex = ege_new Mutex();

  // create group emit resource mutex
  m_emitRequstsMutex = ege_new Mutex();

  // create wait condition
  m_commandsToProcess = ege_new WaitCondition();

  // connect
  ege_connect(m_workThread, finished, this, ResourceManagerMultiThread::onWorkThreadFinished);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManagerMultiThread::~ResourceManagerMultiThread()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EngineModule<IResourceManager>* ResourceManagerMultiThread::Create(Engine& engine)
{
  return ege_new ResourceManagerMultiThread(engine);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceManagerMultiThread::construct()
{
  // start thread
  if ( ! m_workThread->start())
  {
    // error!
    return EGE_ERROR;
  }

  return ResourceManager::construct();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerMultiThread::update(const Time& time)
{
  EGE_UNUSED(time)

  if (EModuleStateRunning == state())
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
      
            emit groupLoadComplete(request.groupName);
            break;

          case RT_GROUP_LOAD_ERROR:

            emit groupLoadError(request.groupName);
            break;

          case RT_PROGRESS:

            //egeCritical() << "Processed" << request.count << "out of" << request.total;

           // emit d_func()->processingStatusUpdated(request.count, request.total);
            break;

          default:

            egeWarning(KResourceManagerDebugName) << "Unknown request type. Skipping.";
            break;
        }
      }

      m_emissionRequests.clear();
    }
  }
  else if ((EModuleStateShuttingDown == state()) && m_workThread->isFinished())
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
void ResourceManagerMultiThread::processCommands()
{
  // check if anything to process
  if ( ! m_scheduledList.empty())
  {
    // wake up worker thread
    m_commandsToProcess->wakeOne();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceManagerMultiThread::loadGroup(const String& name)
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
void ResourceManagerMultiThread::unloadGroup(const String& name)
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
        egeWarning(KResourceManagerDebugName) << "Group" << name << "already scheduled for loading!";
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
void ResourceManagerMultiThread::threadUpdate()
{
  // check if nothing left to process
  // NOTE: stop processing when not ready (ie closing)
  while (m_processList.empty() && (EModuleStateRunning == state()))
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
void ResourceManagerMultiThread::appendBatchesForProcessing(ProcessingBatchList& batches)
{
  // add new data to processing list
  for (ProcessingBatchList::iterator it = batches.begin(); it != batches.end(); ++it)
  {
    ProcessingBatch& batch = *it;

    // build dependencies first
    // NOTE: this contains all dependant groups and itself at the end
    String groupName = batch.groups.back();
    batch.groups.clear();
    if ( ! buildDependacyList(batch.groups, groupName))
    {
      // error!
      egeWarning(KResourceManagerDebugName) << "Could not build dependancy list for group" << groupName;
      continue;
    }

    batch.groups.push_back(groupName);

    // count resources
    for (StringList::const_iterator itGroup = batch.groups.begin(); itGroup != batch.groups.end(); ++itGroup)
    {
      // find group of given name
      PResourceGroup group = this->group(*itGroup);
      if (NULL == group)
      {
        // error!
        egeWarning(KResourceManagerDebugName) << "Group" << *itGroup << "not found!";
        continue;
      }

      // update resource count for batch
      batch.resourcesCount += group->resourceCount();
    }

    // update statistics
    m_totalResourcesToProcess += batch.resourcesCount;

    // add to pending list
    m_processList.push_back(batch);

    egeDebug(KResourceManagerDebugName) << "Group scheduled for" << ((batch.load) ? "loading:" : "unloading:") << groupName;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerMultiThread::processBatches()
{
  // add new data to processing list
  // NOTE: stop processing if not ready anymore (ie closing)
  while ( ! m_processList.empty() && (EModuleStateRunning == state()))
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
ResourceManager::ResourceProcessPolicy ResourceManagerMultiThread::resourceProcessPolicy() const
{
  return ResourceManager::RLP_GROUP;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerMultiThread::onShutdown()
{
  if ((EModuleStateClosed != state()) && (EModuleStateShuttingDown != state()))
  {
    // request stop
    m_workThread->stop(0);

    // wake up any awaiters
    m_commandsToProcess->wakeOne();
  }

  // call base class
  ResourceManager::onShutdown();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerMultiThread::onWorkThreadFinished(const PThread& thread)
{
  EGE_UNUSED(thread);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerMultiThread::addProgressRequest(u32 count, u32 total)
{
  EmissionRequest request;

  request.type  = RT_PROGRESS;
  request.count = count;
  request.total = total;

  MutexLocker lock(m_emitRequstsMutex);
  m_emissionRequests.push_back(request);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerMultiThread::addGroupLoadedRequest(const String& groupName)
{
  EmissionRequest request;

  request.type  = RT_GROUP_LOADED;
  request.groupName = groupName;

  MutexLocker lock(m_emitRequstsMutex);
  m_emissionRequests.push_back(request);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerMultiThread::addGroupLoadErrorRequest(const String& groupName)
{
  EmissionRequest request;

  request.type  = RT_GROUP_LOAD_ERROR;
  request.groupName = groupName;

  MutexLocker lock(m_emitRequstsMutex);
  m_emissionRequests.push_back(request);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END