#include "EGEMutex.h"
#include "EGEDebug.h"
#include "EGEThread.h"

#ifdef EGE_THREAD_PTHREAD
  #include "Core/Threading/PThread/Mutex_p.h"
#endif // EGE_THREAD_PTHREAD

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Mutex)
EGE_DEFINE_DELETE_OPERATORS(Mutex)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Mutex::Mutex(EGEMutex::EType type) 
: Object(EGE_OBJECT_UID_MUTEX)
, m_lockCount(0)
, m_type(type)
, m_owner(NULL)
{
  m_p = ege_new MutexPrivate(this, type);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Mutex::~Mutex()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Mutex::isValid() const
{
  return (NULL != m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Mutex::isRecursive() const
{
  return (EGEMutex::Recursive == m_type);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Mutex::lock()
{
  EGE_ASSERT(isValid());

  bool result = false;
  if (NULL != m_p)
  {
    if (m_owner.load() == Thread::CurrentId())
    {
      if (isRecursive())
      {
        ++m_lockCount;
        return true;
      }

      EGE_ASSERT_X(false, "Deadlock detected!");
    }

    result = m_p->lock();

    // store owner and set initial counter
    if (result)
    {
      m_owner.store(Thread::CurrentId());
      m_lockCount = 1;
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Mutex::unlock()
{
  EGE_ASSERT(isValid());
  EGE_ASSERT_X(m_owner.load() == Thread::CurrentId(), "Cannot unlock mutex from different than owning thread!");

  bool result = false;
  if (m_p)
  {
    result = true;

    if (0 < m_lockCount)
    {
      --m_lockCount;
    }
    
    if (0 == m_lockCount)
    {
      m_owner.store(NULL);

      result = m_p->unlock();
      EGE_ASSERT_X(result, "Unlocking mutex failed!");
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END