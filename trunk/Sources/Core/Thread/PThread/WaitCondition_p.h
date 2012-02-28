#ifndef EGE_CORE_WAITCONDITION_PRIVATE_H
#define EGE_CORE_WAITCONDITION_PRIVATE_H

/*! PThreads implementation of WaitCondition. */

#include <EGE.h>
#include <pthread.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class WaitCondition;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class WaitConditionPrivate
{
  public:

    WaitConditionPrivate(WaitCondition* base);
   ~WaitConditionPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Locks mutex. */
    bool lock();
    /* Unlocks mutex. */
    bool unlock();

  private:

    /*! WaitCondition object. */
    pthread_cond_t m_condition;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_WAITCONDITION_PRIVATE_H