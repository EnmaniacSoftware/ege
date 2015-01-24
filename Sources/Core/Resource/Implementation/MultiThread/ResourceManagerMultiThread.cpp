#include "Core/Resource/Interface/MultiThread/ResourceManagerMultiThread.h"
#include "Core/Resource/Implementation/MultiThread/ResourceManagerWorkThread.h"
#include "Core/Resource/Implementation/MultiThread/ResourceManagerGroupLoadedRequest.h"
#include "Core/Resource/Implementation/MultiThread/ResourceManagerGroupUnloadedRequest.h"
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
ResourceManagerMultiThread::ResourceManagerMultiThread(Engine& engine, IResourceLoader& loader) 
: ResourceManager(engine, loader)
{
  // create work thread
  m_workThread = ege_new ResourceManagerWorkThread(this);

  // create access mutex
  m_mutex = ege_new Mutex(EGEMutex::Recursive);

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
      MutexLocker lock(m_mutex);
      MutexLocker lock2(m_emitRequstsMutex);

      // emit one by one
      for (EmissionRequestList::const_iterator it = m_emissionRequests.begin(); it != m_emissionRequests.end(); ++it)
      {
        const ResourceManagerRequest* request = *it;

        switch (request->type())
        {
          case EGroupLoaded:
      
            // call base class implementation this time
            ResourceManager::onGroupLoaded(static_cast<const ResourceManagerGroupLoadedRequest*>(request)->group(),
                                           static_cast<const ResourceManagerGroupLoadedRequest*>(request)->result());
            break;

          case EGroupUnloaded:
      
            // call base class implementation this time
            ResourceManager::onGroupUnloaded(static_cast<const ResourceManagerGroupUnloadedRequest*>(request)->group(),
                                             static_cast<const ResourceManagerGroupUnloadedRequest*>(request)->result());
            break;

          /*case RT_PROGRESS:

            //egeCritical() << "Processed" << request.count << "out of" << request.total;

           // emit d_func()->processingStatusUpdated(request.count, request.total);
          //  break;
          */
          default:

            egeWarning(KResourceManagerDebugName) << "Unknown request type. Skipping.";
            break;
        }

        // delete request
        EGE_DELETE(request);
      }

      m_emissionRequests.clear();
    }

    // check if anything to process
    if ( ! m_processList.empty())
    {
      // wake up worker thread
      m_commandsToProcess->wakeOne();
    }
  }
  else if (EModuleStateShuttingDown == state())
  {
    if (m_workThread->isFinished())
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
    else
    {
      // wake it up
      m_commandsToProcess->wakeOne();
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceManagerMultiThread::loadGroup(const String& name)
{
  MutexLocker locker(m_mutex);

  // check if already in pending list
  for (ProcessingBatchList::iterator it = m_processList.begin(); it != m_processList.end(); ++it)
  {
    ProcessingBatch& batch = *it;

    if (batch.groups.back() == name)
    {
      // check if scheduled for unloading and not started yet
      if ( ! batch.load && (0 == batch.startTime.microseconds()))
      {
        // remove from pool
        m_processList.erase(it);
  
        // TAGE - should notification (fake) be sent ?
        return EGE_SUCCESS;
      }
      else
      {
        // already scheduled
        egeWarning(KResourceManagerDebugName) << "Group" << name << "already scheduled for loading!";
        return EGE_ERROR_ALREADY_EXISTS;
      }
    }
  }

  // add to pool
  ProcessingBatch batch;
  batch.load = true;

  // setup processing batch
  if ( ! initializeProcessingBatch(batch, name))
  {
    // error!
    egeWarning(KResourceManagerDebugName) << "Could not setup processing batch for group" << name;
    return EGE_ERROR;
  }

  m_processList.push_back(batch);

  // update statistics
  m_totalResourcesToProcess += batch.resourcesCount;

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerMultiThread::unloadGroup(const String& name)
{
  MutexLocker locker(m_mutex);

  // check if already in pending list
  for (ProcessingBatchList::iterator it = m_processList.begin(); it != m_processList.end(); ++it)
  {
    ProcessingBatch& batch = *it;

    if (batch.groups.back() == name)
    {
      // check if scheduled for unloading and not started yet
      if (batch.load && (0 == batch.startTime.microseconds()))
      {
        // remove from pool
        m_processList.erase(it);

        // TAGE - should notification (fake) be sent ?
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
  batch.load = false;

  // setup processing batch
  if ( ! initializeProcessingBatch(batch, name) || ! finalizeProcessingBatch(batch))
  {
    // error!
    egeWarning(KResourceManagerDebugName) << "Could not setup processing batch for group" << name;
    return;
  }

  m_processList.push_back(batch);

  // update statistics
  m_totalResourcesToProcess += batch.resourcesCount;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerMultiThread::threadUpdate()
{
  m_mutex->lock();

  // check if nothing to process
  if (m_processList.empty() && (EModuleStateRunning == state()))
  {
    // wait for data
    // NOTE: this unlocks mutex
    printf("Waiting...\n");
    m_commandsToProcess->wait(m_mutex);
    printf("Resuming...\n");

    // mutex is locked here
  }

  // check if nothing to process
  if ( ! m_processList.empty() && (EModuleStateRunning == state()))
  {
    processBatch();
  }

  m_mutex->unlock();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerMultiThread::onShutdown()
{
  //printf("Shutting down\n");

  if ((EModuleStateClosed != state()) && (EModuleStateShuttingDown != state()))
  {
    printf("Really shutting down\n");

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
  //EmissionRequest request;

  //request.type  = RT_PROGRESS;
  //request.count = count;
  //request.total = total;

  //MutexLocker lock(m_emitRequstsMutex);
  //m_emissionRequests.push_back(request);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerMultiThread::onGroupLoaded(const PResourceGroup& group, EGEResult result)
{
  // NOTE: mutex is locked

  ResourceManagerGroupLoadedRequest* request = ege_new ResourceManagerGroupLoadedRequest(group, result);

  MutexLocker lock(m_emitRequstsMutex);
  m_emissionRequests.push_back(request);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerMultiThread::onGroupUnloaded(const PResourceGroup& group, EGEResult result)
{
  // NOTE: mutex is locked

  ResourceManagerGroupUnloadedRequest* request = ege_new ResourceManagerGroupUnloadedRequest(group, result);

  MutexLocker lock(m_emitRequstsMutex);
  m_emissionRequests.push_back(request);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerMultiThread::processBatch()
{
  // NOTE: mutex is locked

  ProcessingBatch& batch = m_processList.front();

  // check if batch has NOT been started yet
  if (batch.resources.empty() && (0 == batch.startTime.microseconds()))
  {
    // finalize batch
    if ( ! finalizeProcessingBatch(batch))
    {
      // error!
      egeCritical(KResourceManagerDebugName) << "Could not finalize processing batch for group" << batch.groups.last("");
      
      // remove from pool
      m_processList.pop_front();

      // do not process any further
      return;
    }
  }

  // call base class
  ResourceManager::processBatch();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END