#ifndef EGE_CORE_WAITCONDITION_PRIVATE_H
#define EGE_CORE_WAITCONDITION_PRIVATE_H

/*! PThreads implementation of WaitCondition. */

#include <EGE.h>
#include <pthread.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class WaitCondition;
class Mutex;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class WaitConditionPrivate
{
  public:

    WaitConditionPrivate(WaitCondition* base);
   ~WaitConditionPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Releases the locked mutex and waits on the wait condition. 
     * @note The mutex must be initially locked by the calling thread. If mutex is not in a locked state, this function returns immediately. 
     */
    bool wait(Mutex* mutex);
    /* Wakes one thread waiting on the wait condition. 
     * @note The thread that is woken up depends on the operating system's scheduling policies, and cannot be controlled or predicted.
     * @note Raw pointer is used here so Mutex referece counter is not increased as this will block.     
     */
    void wakeOne();
    /* Wakes all threads waiting on the wait condition. 
     * @note The order in which the threads are woken up depends on the operating system's scheduling policies and cannot be controlled or predicted.
     */
    void wakeAll();

  private:

    /*! WaitCondition object. */
    pthread_cond_t m_condition;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_WAITCONDITION_PRIVATE_H