#ifndef EGE_CORE_MUTEX_H
#define EGE_CORE_MUTEX_H

/*! This class provides access serialization between threads. */

#include "EGE.h"
#include "EGEAtomicInt.h"
#include "EGEAtomicPointer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_PREDECLARE_SMART_CLASS(Mutex, PMutex)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Mutex : public Object
{
  /*! For accessing private implementation. */
  friend class WaitConditionPrivate;

  public:

    Mutex(EGEMutex::EType type = EGEMutex::Normal);
   ~Mutex();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Returns TRUE if object is valid. */
    bool isValid() const;
    /*! Returns TRUE if mutex is recursive. */
    bool isRecursive() const;

    /*! Locks mutex. */
    bool lock();
    /*! Unlocks mutex. */
    bool unlock();

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(Mutex);

    /*! Lock counter. */
    s32 m_lockCount;
    /*! Type. */
    EGEMutex::EType m_type;
    /*! Thread owning the mutex. NULL if mutex is not locked. */
    AtomicPointer<void> m_owner;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MUTEX_H