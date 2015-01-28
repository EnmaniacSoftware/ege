#ifndef EGE_CORE_THREAD_PRIVATE_H
#define EGE_CORE_THREAD_PRIVATE_H

/*! PThreads implementation of Thread. */

#include "EGE.h"
#include "EGEAtomicBool.h"
#include "EGESignal.h"
#include "EGETime.h"
#include <pthread.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Thread;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ThreadPrivate
{
  public:

    ThreadPrivate(Thread* base);
   ~ThreadPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(Thread)

    /*! Starts thread. */
    bool start();
    /*! Cancels tread. 
     *  @note This forcilbly stops the thread. Use with caution.
     */
    void cancel();

    /*! Waits for thread to be finished. */
    bool wait();

    /*! Returns TRUE if thread is currently running. */
    bool isRunning() const;
    /*! Returns TRUE if thread has finished its work. */
    bool isFinished() const;

  private:

    /*! Thread function. */
    static void* ThreadFunc(void* userData);
    /*! Thread finalization function. 
     *  @note This can be called as a clean up due to thread cancellation or as a part of regular shutdown.
     */
    static void ThreadFinalize(ThreadPrivate* thread);

  private:

    /*! Thread object. */
    pthread_t m_thread;
    /*! Cancel flag. */
    AtomicBool m_cancelled;
    /*! Running flag. */
    mutable AtomicBool m_running;
    /*! Finished flag. */
    mutable AtomicBool m_finished;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_THREAD_PRIVATE_H