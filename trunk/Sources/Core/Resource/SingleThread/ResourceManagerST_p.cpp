#if EGE_RESOURCEMANAGER_SINGLE_THREAD

#include "Core/Resource/SingleThread/ResourceManagerST_p.h"
#include "Core/Resource/ResourceGroup.h"
#include "EGETimer.h"
#include "EGEDebug.h"

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
  // set state
  m_state = ResourceManager::STATE_READY;

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerPrivate::update(const Time& time)
{
  EGE_UNUSED(time)

  if ( ! m_processList.empty() && (ResourceManager::STATE_READY == m_state))
  {
    ProcessingBatch& data = m_processList.front();

    PResourceGroup group = d_func()->group(data.groups.front());

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
        emit d_func()->groupLoadError(group->name());
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
  else if (ResourceManager::STATE_CLOSING == m_state)
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
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceManagerPrivate::loadGroup(const String& name)
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
        d_func()->m_totalResourcesToProcess -= batch.resourcesCount;

        // remove from pool
        m_processList.erase(it);
        return EGE_ERROR_ALREADY_EXISTS;
      }
      else
      {
        // already scheduled
        egeWarning() << "Group" << name << "already scheduled for loading!";
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
  if ( ! d_func()->buildDependacyList(batch.groups, name))
  {
    // error!
    egeWarning() << "Could not build dependancy list for group" << name;
    return EGE_ERROR;
  }

  batch.groups.push_back(name);

  // count resources
  for (StringList::const_iterator it = batch.groups.begin(); it != batch.groups.end(); ++it)
  {
    // find group of given name
    PResourceGroup group = d_func()->group(*it);
    if (NULL == group)
    {
      // error!
      egeWarning() << "Group" << *it << "not found!";
      return EGE_ERROR;
    }

    // update resource count for batch
    batch.resourcesCount += group->resourceCount();
  }

  // add to pool
  m_processList.push_back(batch);

  // update statistics
  d_func()->m_totalResourcesToProcess += batch.resourcesCount;

  egeDebug() << "Group scheduled for loading:" << name;

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerPrivate::unloadGroup(const String& name)
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
        d_func()->m_totalResourcesToProcess -= batch.resourcesCount;
       
        // remove from pool
        m_processList.erase(it);
        return;
      }
      else
      {
        // already scheduled
        egeWarning() << "Group" << name << "already scheduled for unloading!";
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
  if ( ! d_func()->buildDependacyList(batch.groups, name))
  {
    // error!
    egeWarning() << "Could not build dependancy list for group" << name;
    return;
  }

  batch.groups.push_back(name);

  // count resources
  for (StringList::const_iterator it = batch.groups.begin(); it != batch.groups.end(); ++it)
  {
    // find group of given name
    PResourceGroup group = d_func()->group(*it);
    if (NULL == group)
    {
      // error!
      egeWarning() << "Group" << name << "not found!";
      return;
    }

    // update resource count for batch
    batch.resourcesCount += group->resourceCount();
  }

  // add to pool
  m_processList.push_back(batch);

  // update statistics
  d_func()->m_totalResourcesToProcess += batch.resourcesCount;

  egeDebug() << "Group scheduled for unloading:" << name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManager::ResourceProcessPolicy ResourceManagerPrivate::resourceProcessPolicy() const
{
  return ResourceManager::RLP_RESOURCE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerPrivate::shutDown()
{
  // we are done
  m_state = ResourceManager::STATE_CLOSING;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManager::State ResourceManagerPrivate::state() const
{
  return m_state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerPrivate::onGroupLoaded(const PResourceGroup& group)
{
  ProcessingBatch& data = m_processList.front();

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
      m_processList.pop_front();

      // signal
      emit d_func()->groupLoadComplete(group->name());

      // check if not more batches to process
      if (m_processList.empty())
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
  ProcessingBatch& data = m_processList.front();

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
      m_processList.pop_front();

      // check if not more batches to process
      if (m_processList.empty())
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

#endif // EGE_RESOURCEMANAGER_SINGLE_THREAD