#include "Core/Resource/Interface/MultiThread/ResourceManagerMultiThread.h"
#include "Core/Resource/Implementation/MultiThread/ResourceManagerWorkThread.h"
#include "EGEGraphics.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManagerWorkThread::ResourceManagerWorkThread(ResourceManagerMultiThread* manager) 
: Thread()
, m_manager(manager)
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
