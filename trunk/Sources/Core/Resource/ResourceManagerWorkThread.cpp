#include "Core/Application/Application.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/ResourceManagerWorkThread.h"
#include <EGEDebug.h>

EGE_NAMESPACE

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
    resourceManager->m_mutex->lock();
    if (resourceManager->m_commands.empty())
    {
      // wait for any commands
      resourceManager->m_commandsToProcess->wait(resourceManager->m_mutex);
    }
    resourceManager->m_mutex->unlock();
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
