#ifdef EGE_RESOURCE_MANAGER_SINGLE_THREAD

#include "Core/Resource/SingleThread/ResourceManagerST_p.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/ResourceGroup.h"
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceManagerPrivate)
EGE_DEFINE_DELETE_OPERATORS(ResourceManagerPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManagerPrivate::ResourceManagerPrivate(ResourceManager* base) : m_d(base)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManagerPrivate::~ResourceManagerPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Constructs object. */
EGEResult ResourceManagerPrivate::construct()
{
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates manager. */
void ResourceManagerPrivate::update(const Time& time)
{
  EGE_UNUSED(time)

  // process pending list
  if ( ! m_processList.empty())
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
/*! Processes commands. */
void ResourceManagerPrivate::processCommands()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Loads group with given name. 
 *  @param name  Group name to be loaded.
 *  @return  Returns EGE_SUCCESS if group has been scheduled for loading. EGE_ERROR_ALREADY_EXISTS if group is already loaded. Otherwise, EGE_ERROR.
 *  @note  Given group, when found, is scheduled for loading rather than loaded immediately.
 */
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

  //// find group of given name
  //PResourceGroup theGroup = d_func()->group(name);
  //if (NULL != theGroup)
  //{
  //  // check if given group is scheduled for unloading/loading already
  //  CommandDataList::iterator it;
  //  for (it = m_commands.begin(); it != m_commands.end(); ++it)
  //  {
  //    CommandData& commandData = *it;
  //    if (((COMMAND_UNLOAD_GROUP == commandData.command) || (COMMAND_LOAD_GROUP == commandData.command)) && (commandData.groupNames.front() == name))
  //    {
  //      // found, done
  //      break;
  //    }
  //  }

  //  // check if loaded already
  //  if (theGroup->isLoaded())
  //  {
  //    // check if it awaits unloading
  //    if ((it != m_commands.end()) && (COMMAND_UNLOAD_GROUP == (*it).command))
  //    {
  //      // remove it from command list
  //      m_commands.erase(it);
  //    }

  //    // cannot be loaded
  //    egeWarning() << name << "already loaded.";

  //    // unlock resources
  //    p_func()->unlockResources();

  //    return EGE_ERROR_ALREADY_EXISTS;
  //  }
  //  
  //  // check if it is awaiting loading already
  //  if ((it != m_commands.end()) && (COMMAND_LOAD_GROUP == (*it).command))
  //  {
  //    // do nothing
  //    egeWarning() << "Group" << name << "already scheduled. Skipping.";

  //    // unlock resources
  //    p_func()->unlockResources();
  //    return EGE_SUCCESS;
  //  }

  //  // add group for loading
  //  CommandData commandData;

  //  commandData.command = COMMAND_LOAD_GROUP;
  //  commandData.groupNames.push_back(name);

  //  // add dependancies
  //  if (buildDependacyList(commandData.groupNames, name))
  //  {
  //    // add to pool
  //    m_commands.push_back(commandData);
  //  }
  //  else
  //  {
  //    // error!

  //    // unlock resources
  //    p_func()->unlockResources();

  //    return EGE_ERROR;
  //  }

  //  // unlock resources
  //  p_func()->unlockResources();

  //  egeDebug() << name << "scheduled for loading.";
  //  return EGE_SUCCESS;
  //}
 
  //egeWarning() << name << "not found!";

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Unloads group with given name. */
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

#endif // EGE_RESOURCE_MANAGER_SINGLE_THREAD