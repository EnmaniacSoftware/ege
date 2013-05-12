#ifndef EGE_CORE_MUTEX_PRIVATE_H
#define EGE_CORE_MUTEX_PRIVATE_H

/*! PThreads implementation of Mutex. */

#include "EGE.h"
#include "EGEMutex.h"
#include <pthread.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class MutexPrivate
{
  /*! For accessing private data. */
  friend class WaitConditionPrivate;

  public:

    MutexPrivate(Mutex* base, EGEMutex::EType type);
   ~MutexPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Locks mutex. */
    bool lock();
    /*! Unlocks mutex. */
    bool unlock();

  private:

    /*! Mutex object. */
    pthread_mutex_t m_mutex;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MUTEX_PRIVATE_H