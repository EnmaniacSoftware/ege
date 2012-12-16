#ifdef EGE_RESOURCE_MANAGER_MULTI_THREAD

#include "Core/Application/Application.h"
#include "Core/Resource/MultiThread/ResourceManagerMT_p.h"
#include "Core/Resource/MultiThread/ResourceManagerWorkThread.h"
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManagerWorkThread::ResourceManagerWorkThread(Application* app) : Thread(app)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManagerWorkThread::~ResourceManagerWorkThread()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Thread override. Work method. */
s32 ResourceManagerWorkThread::run()
{
  ResourceManager* resourceManager = app()->resourceManager();

  while (!isStopping())
  {
    //resourceManager->m_mutex->lock();
    //updateList();
    //resourceManager->m_mutex->unlock();

    //// process list
    //processList();

    //// wait till more data is in
    //resourceManager->m_mutex->lock();
    //if (resourceManager->m_commands.empty())
    //{
    //  // wait for any commands
    //  resourceManager->m_commandsToProcess->wait(resourceManager->m_mutex);
    //}
    //resourceManager->m_mutex->unlock();
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds group to list of pending actions. */
//void ResourceManagerWorkThread::addCommand(const ResourceManager::CommandData& command)
//{
//  if (ResourceManager::COMMAND_LOAD_GROUP == command.command)
//  {
//    m_list << command;
//  }
//  else if (ResourceManager::COMMAND_UNLOAD_GROUP == command.command)
//  {
//    m_list << command;
//  }
//  else
//  {
//    EGE_ASSERT(false && "Undefined command");
//  }
//}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------
///*! Processes the current active list of commands. */
//void ResourceManagerWorkThread::processList()
//{
//  ResourceManager* resourceManager = app()->resourceManager();
//
//  // go thru all commands
//  for (ResourceManager::CommandDataList::iterator it = m_list.begin(); it != m_list.end(); ++it)
//  {
//    ResourceManager::CommandData& commandData = *it;
//
//    // process according to command type
//    if (ResourceManager::COMMAND_LOAD_GROUP == commandData.command)
//    {
//      // process groups in back to front order
//      for (StringList::reverse_iterator it = commandData.groupNames.rbegin(); it != commandData.groupNames.rend(); ++it)
//      {
//        // find group
//        PResourceGroup groupResource = resourceManager->group(*it);
//
//        // load group
//        if ((NULL == groupResource) || (EGE_SUCCESS != groupResource->load()))
//        {
//          // error!
//          // NOTE: emit error for main group
//          emit resourceManager->groupLoadError(commandData.groupNames.front());
//        }
//        else
//        {
//          // emit completion if this is main group
//          if (*it == commandData.groupNames.front())
//          {
//            emit resourceManager->groupLoadComplete(*it);
//          }
//        }
//      }
//    }
//    else if (ResourceManager::COMMAND_UNLOAD_GROUP == commandData.command)
//    {
//      // process groups in back to front order
//      for (StringList::reverse_iterator it = commandData.groupNames.rbegin(); it != commandData.groupNames.rend(); ++it)
//      {
//        // find group
//        PResourceGroup groupResource = resourceManager->group(*it);
//
//        // unload group
//        if (NULL != groupResource)
//        {
//          groupResource->unload();
//        }
//      }
//    }
//    else
//    {
//      EGE_ASSERT("Invalid command!");
//    }
//  }
//
//  m_list.clear();
//}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------
///*! Updates command list. */
//void ResourceManagerWorkThread::updateList()
//{
//  ResourceManager* resourceManager = app()->resourceManager();
//
//  // add currently pending commands to active list
//  for (ResourceManager::CommandDataList::iterator it = resourceManager->m_commands.begin(); it != resourceManager->m_commands.end(); ++it)
//  {
//    // add to list
//    addCommand(*it);
//  }
//  resourceManager->m_commands.clear();
//}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_RESOURCE_MANAGER_MULTI_THREAD