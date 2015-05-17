#include "Core/Resource/Interface/MultiThread/ResourceManagerMultiThread.h"
#include "Core/Resource/Implementation/MultiThread/ResourceManagerWorkThread.h"
#include "EGEDevice.h"
#include "EGEGraphics.h"

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

    // allow other threads do some work
    Device::Sleep(1);
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END
