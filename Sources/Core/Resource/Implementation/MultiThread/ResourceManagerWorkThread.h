#ifndef EGE_CORE_RESOURCE_RESOURCEMANAGER_WORKTHREAD_H
#define EGE_CORE_RESOURCE_RESOURCEMANAGER_WORKTHREAD_H

/*! Resource manager's working thread responsible for processing of loading/unloading queues. 
 */

#include "EGEThread.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceManagerWorkThread : public Thread
{
  public:

    ResourceManagerWorkThread(ResourceManagerMultiThread* manager);
   ~ResourceManagerWorkThread();

  private:

    /*! @see Thread::run */
    EGE::s32 run() override;

  private:

    /*! Resource manager instance. */
    ResourceManagerMultiThread* m_manager;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCE_RESOURCEMANAGER_WORKTHREAD_H