#ifdef EGE_RESOURCE_MANAGER_SINGLE_THREAD

#include "Core/Resource/SingleThread/ResourceManagerST_p.h"
#include "Core/Resource/ResourceGroup.h"
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
  // set state
  m_state = ResourceManager::STATE_READY;

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerPrivate::update(const Time& time)
{
  EGE_UNUSED(time)

  // process pending list
  // NOTE: only process when ready (ie not closing)
  if ( ! m_processList.empty() && (ResourceManager::STATE_READY == m_state))
  {
    ProcessingBatch& data = m_processList.front();

    PResource resource = data.resources.front();

    if (data.load)
    {
      // load resource
      EGEResult result = resource->load();
      if (EGE_SUCCESS == result)
      {
        // remove resource from pool
        data.resources.pop_front();

        // update statistics
        d_func()->m_processedResourcesCount++;

        // emit
        emit d_func()->processingStatusUpdated(d_func()->m_processedResourcesCount, d_func()->m_totalResourcesToProcess);

        egeCritical() << "Processed" << d_func()->m_processedResourcesCount << "out of" << d_func()->m_totalResourcesToProcess;

        // check if no more resources to load
        if (data.resources.empty())
        {
          // TAGE - hmm, better solution needed ? as we process all resources individually here
          // call ResourceGroup::load so it marks itself as loaded
          PResourceGroup group = d_func()->group(data.groupName);
          if (NULL != group)
          {
            group->load();
          }

          // emit completion
          emit d_func()->groupLoadComplete(data.groupName);
        }
      }
      else if (EGE_WAIT == result)
      {
        // still processing, do nothing
      }
      else
      {
        // remove resource from pool
        data.resources.pop_front();

        // update statistics
        d_func()->m_processedResourcesCount++;

        // emit
        emit d_func()->processingStatusUpdated(d_func()->m_processedResourcesCount, d_func()->m_totalResourcesToProcess);

        egeCritical() << "Processed" << d_func()->m_processedResourcesCount << "out of" << d_func()->m_totalResourcesToProcess;

        // error!
        emit d_func()->groupLoadError(data.groupName);
      }
    }
    else
    {
      // load resource
      resource->unload();

      // remove resource from pool
      data.resources.pop_front();

      // update statistics
      d_func()->m_processedResourcesCount++;

      // emit
      emit d_func()->processingStatusUpdated(d_func()->m_processedResourcesCount, d_func()->m_totalResourcesToProcess);

      egeCritical() << "Processed" << d_func()->m_processedResourcesCount << "out of" << d_func()->m_totalResourcesToProcess;

      // check if no more resources to load
      if (data.resources.empty())
      {
        // TAGE - hmm, better solution needed ? as we process all resources individually here
        // call ResourceGroup::unload so it marks itself as unloaded
        PResourceGroup group = d_func()->group(data.groupName);
        if (NULL != group)
        {
          group->unload();
        }
      }
    }

    // check if no more resources to load in a group
    if (data.resources.empty())
    {
      // remove batch
      m_processList.pop_front();
    
      // check if nothing to be processed
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

    if ((batch.groupName == name))
    {
      // check if scheduled for unloading
      if ( ! batch.load)
      {
        // remove from pool
        m_processList.erase(it);

        // update statistics
        d_func()->m_totalResourcesToProcess -= batch.resources.size();

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

  // build dependencies first
  // NOTE: this contains all dependant groups and itself at the end
  StringList groupsToLoad;
  if ( ! d_func()->buildDependacyList(groupsToLoad, name))
  {
    // error!
    egeWarning() << "Could not build dependancy list for group" << name;
    return EGE_ERROR;
  }

  groupsToLoad.push_back(name);

  // add to pool
  ProcessingBatch batch;
  batch.groupName = name;
  batch.load      = true;

  for (StringList::const_iterator it = groupsToLoad.begin(); it != groupsToLoad.end(); ++it)
  {
    // find group of given name
    PResourceGroup group = d_func()->group(*it);
    if (NULL == group)
    {
      // error!
      egeWarning() << "Group" << name << "not found!";
      return EGE_ERROR;
    }

    // add to pool
    batch.resources << group->resources("");
  }

  m_processList.push_back(batch);

  // update statistics
  d_func()->m_totalResourcesToProcess += batch.resources.size();

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerPrivate::unloadGroup(const String& name)
{
  // check if already scheduled for processing
  for (ProcessingBatchList::iterator it = m_processList.begin(); it != m_processList.end(); ++it)
  {
    ProcessingBatch& batch = *it;

    if ((batch.groupName == name))
    {
      // check if scheduled for loading
      if (batch.load)
      {
        // remove from pool
        m_processList.erase(it);

        // update statistics
        d_func()->m_totalResourcesToProcess -= batch.resources.size();
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

  // find group of given name
  PResourceGroup group = d_func()->group(name);
  if (NULL == group)
  {
    // error!
    egeWarning() << "Group" << name << "not found!";
    return;
  }

  // add to pool
  ProcessingBatch batch;
  batch.groupName = name;
  batch.load      = false;
  batch.resources = group->resources("");

  m_processList.push_back(batch);

  // update statistics
  d_func()->m_totalResourcesToProcess += batch.resources.size();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManager::ResourceProcessPolicy ResourceManagerPrivate::resourceProcessPolicy() const
{
  return ResourceManager::RLP_RESOURCE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerPrivate::shutDown()
{
  // remove all data
  d_func()->removeGroups();

  // we are done
  m_state = ResourceManager::STATE_CLOSED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManager::State ResourceManagerPrivate::state() const
{
  return m_state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_RESOURCE_MANAGER_SINGLE_THREAD