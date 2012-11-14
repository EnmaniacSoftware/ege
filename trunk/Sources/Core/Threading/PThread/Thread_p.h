#ifndef EGE_CORE_THREAD_PRIVATE_H
#define EGE_CORE_THREAD_PRIVATE_H

/*! PThreads implementation of Thread. */

#include <EGE.h>
#include <EGESignal.h>
#include <EGETime.h>
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

    /* Starts thread. */
    bool start();
    /* Cancels tread. 
     * @note This forcilbly stops the thread. Use with caution.
     */
    void cancel();

    /* Waits for thread to be finished. */
    bool wait();

    /* Returns TRUE if thread is currently running. */
    bool isRunning() const;
    /* Returns TRUE if thread has finished its work. */
    bool isFinished() const;

  private:

    /* Thread function. */
    static void* ThreadFunc(void* userData);

  private:

    /*! Thread object. */
    pthread_t m_thread;
    /*! Running flag. */
    volatile bool m_running;
    /*! Finished flag. */
    bool m_finished;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_THREAD_PRIVATE_H