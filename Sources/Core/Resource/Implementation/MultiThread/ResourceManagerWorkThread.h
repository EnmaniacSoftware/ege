#ifndef EGE_CORE_RESOURCEMANAGER_WORKTHREAD_H
#define EGE_CORE_RESOURCEMANAGER_WORKTHREAD_H

#if EGE_RESOURCEMANAGER_MULTI_THREAD

/*! Resource manager's working thread responsible for processing of loading/unloading queues. 
 */

#include "EGEThread.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceManagerWorkThread : public Thread
{
  public:

    ResourceManagerWorkThread(ResourceManagerPrivate* manager);
   ~ResourceManagerWorkThread();

  private:

    /*! @see Thread::run */
    EGE::s32 run() override;

  private:

    /*! Resource manager instance. */
    ResourceManagerPrivate* m_manager;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_RESOURCEMANAGER_MULTI_THREAD

#endif // EGE_CORE_RESOURCEMANAGER_WORKTHREAD_H