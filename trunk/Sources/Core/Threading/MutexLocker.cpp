#include "EGEMutex.h"
#include "EGEDebug.h"
#include "EGEThread.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MutexLocker::MutexLocker(PMutex mutex, bool debug) : m_mutex(mutex),
                                                     m_debug(debug)
{
  if (NULL != m_mutex)
  {
    if (m_debug)
    {
      Debug::PrintWithArgs("Locking %p %d at thread %d", m_mutex.object(), m_mutex->referenceCount(), Thread::CurrentId());
    }

    m_mutex->lock();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MutexLocker::~MutexLocker()
{
  if (NULL != m_mutex)
  {
    if (m_debug)
    {
      Debug::PrintWithArgs("Unlocking %p %d at thread %d", m_mutex.object(), m_mutex->referenceCount(), Thread::CurrentId());
    }

    m_mutex->unlock();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END