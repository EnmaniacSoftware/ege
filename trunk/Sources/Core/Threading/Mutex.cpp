#include <EGEMutex.h>
#include <EGEDebug.h>

#ifdef EGE_THREAD_PTHREAD
#include "Core/Threading/PThread/Mutex_p.h"
#endif // EGE_THREAD_PTHREAD

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Mutex)
EGE_DEFINE_DELETE_OPERATORS(Mutex)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Mutex::Mutex(Application* app) : Object(app, EGE_OBJECT_UID_MUTEX),
                                 m_locked(false)
{
  m_p = ege_new MutexPrivate(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Mutex::~Mutex()
{
  EGE_DELETE(m_p);

  m_locked = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool Mutex::isValid() const
{
  return (NULL != m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Locks mutex. */
bool Mutex::lock()
{
  EGE_ASSERT(isValid());

  bool result = false;
  if (NULL != m_p)
  {
    result = m_p->lock();
    if (result)
    {
      m_locked = result;
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Unlocks mutex. */
bool Mutex::unlock()
{
  EGE_ASSERT(isValid());

  bool result = false;
  if (m_p)
  {
    result = m_p->unlock();
    if (result)
    {
      m_locked = false;
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END