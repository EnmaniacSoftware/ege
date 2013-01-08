#if EGE_RESOURCEMANAGER_MULTI_THREAD

#include "Core/Application/Application.h"
#include "Core/Resource/MultiThread/ResourceManagerMT_p.h"
#include "Core/Resource/MultiThread/ResourceManagerWorkThread.h"
#include <EGEGraphics.h>
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManagerWorkThread::ResourceManagerWorkThread(Application* app, ResourceManagerPrivate* manager) : Thread(app),
                                                                                                          m_manager(manager)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManagerWorkThread::~ResourceManagerWorkThread()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 ResourceManagerWorkThread::run()
{
  // initialize rendering context for this thread
  app()->graphics()->initializeWorkThreadRenderingContext();

  while ( ! isStopping())
  {
    m_manager->threadUpdate();
  }

  // deinitialize rendering context for this thread
  app()->graphics()->deinitializeWorkThreadRenderingContext();

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_RESOURCEMANAGER_MULTI_THREAD