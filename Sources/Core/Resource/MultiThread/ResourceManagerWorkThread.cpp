#if EGE_RESOURCEMANAGER_MULTI_THREAD

#include "Core/Application/Application.h"
#include "Core/Resource/MultiThread/ResourceManagerMT_p.h"
#include "Core/Resource/MultiThread/ResourceManagerWorkThread.h"
#include "EGEGraphics.h"
#include "EGEDebug.h"

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
  while ( ! isStopping())
  {
    m_manager->threadUpdate();
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_RESOURCEMANAGER_MULTI_THREAD