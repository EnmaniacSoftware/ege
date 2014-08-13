#ifndef EGE_CORE_WAITCONDITION_H
#define EGE_CORE_WAITCONDITION_H

/*! This class provides a condition variable for synchronizing threads. */

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(WaitCondition, PWaitCondition)
EGE_DECLARE_SMART_CLASS(Mutex, PMutex)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class WaitCondition : public Object
{
  public:

    WaitCondition();
   ~WaitCondition();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Returns TRUE if object is valid. */
    bool isValid() const;

    /*! Releases the locked mutex and waits on the wait condition. 
     *  @note The mutex must be initially locked by the calling thread. If mutex is not in a locked state, this function returns immediately.
     *  @note Raw pointer is used here so Mutex referece counter is not increased as this will block.
     */
    bool wait(Mutex* mutex);
    /*! Wakes one thread waiting on the wait condition. 
     *  @note The thread that is woken up depends on the operating system's scheduling policies, and cannot be controlled or predicted.
     */
    void wakeOne();
    /*! Wakes all threads waiting on the wait condition. 
     *  @note The order in which the threads are woken up depends on the operating system's scheduling policies and cannot be controlled or predicted.
     */
    void wakeAll();

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(WaitCondition);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_WAITCONDITION_H