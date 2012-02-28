#include "Core/Application/Application.h"
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
  while (!isStopping())
  {
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
