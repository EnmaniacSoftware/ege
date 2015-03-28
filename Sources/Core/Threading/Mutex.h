#ifndef EGE_CORE_MUTEX_H
#define EGE_CORE_MUTEX_H

/*! This class provides access serialization between threads. */

#include "EGE.h"
#include "EGEAtomicBool.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Mutex, PMutex)
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

    /*! Locks mutex. */
    bool lock();
    /*! Unlocks mutex. */
    bool unlock();

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(Mutex);

    /*! Flag indicating wheter mutex if locked. For debugging purposes. */
    AtomicBool m_locked;
    /*! Type. */
    EGEMutex::EType m_type;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MUTEX_H