#ifndef EGE_CORE_RESOURCEMANAGER_WORKTHREAD_H
#define EGE_CORE_RESOURCEMANAGER_WORKTHREAD_H

/*! Resource manager's working thread responsible for processing of loading/unloading queues. 
 */

#include <EGEThread.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceManagerWorkThread : public Thread
{
  public:

    ResourceManagerWorkThread(Application* app);
   ~ResourceManagerWorkThread();

  private:

    /* Thread override. Work method. */
    EGE::s32 run() override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCEMANAGER_WORKTHREAD_H