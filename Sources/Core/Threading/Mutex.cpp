#include "EGEMutex.h"
#include "EGEDebug.h"

#ifdef EGE_THREAD_PTHREAD
  #include "Core/Threading/PThread/Mutex_p.h"
#endif // EGE_THREAD_PTHREAD

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsRecursive(EGEMutex::EType type)
{
  return (EGEMutex::Recursive == type);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Mutex)
EGE_DEFINE_DELETE_OPERATORS(Mutex)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Mutex::Mutex(EGEMutex::EType type) 
: Object(EGE_OBJECT_UID_MUTEX)
, m_locked(false)
, m_type(type)
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
bool Mutex::lock()
{
  EGE_ASSERT(isValid() && ((! IsRecursive(m_type) && ! m_locked.load()) || IsRecursive(m_type)));

  bool result = false;
  if (NULL != m_p)
  {
    result = m_p->lock();
    m_locked.store(result);
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Mutex::unlock()
{
  EGE_ASSERT(isValid() && (( ! IsRecursive(m_type) && m_locked.load()) || IsRecursive(m_type)));

  bool result = false;
  if (m_p)
  {
    result = m_p->unlock();
    m_locked.store( ! result);
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END