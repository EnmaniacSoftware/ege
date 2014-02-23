#ifndef EGE_CORE_MUTEXLOCKER_H
#define EGE_CORE_MUTEXLOCKER_H

/*! Convienience class for automated mutex locking/unlocking. */

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Mutex, PMutex)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class MutexLocker
{
  public:

    MutexLocker(PMutex mutex, bool debug = false);
   ~MutexLocker();

  private:

    /*! Mutex object for handling. */
    PMutex m_mutex;
    /*! Debug flag, */
    bool m_debug;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MUTEXLOCKER_H