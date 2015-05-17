#include "EGEWaitCondition.h"
#include "EGEMutex.h"
#include "EGEDebug.h"

#ifdef EGE_THREAD_PTHREAD
  #include "Core/Threading/PThread/WaitCondition_p.h"
#endif // EGE_THREAD_PTHREAD

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(WaitCondition)
EGE_DEFINE_DELETE_OPERATORS(WaitCondition)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
WaitCondition::WaitCondition() 
: Object(EGE_OBJECT_UID_WAIT_CONDITION)
{
  m_p = ege_new WaitConditionPrivate(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
WaitCondition::~WaitCondition()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool WaitCondition::isValid() const
{
  return (NULL != m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool WaitCondition::wait(Mutex* mutex)
{
  EGE_ASSERT(isValid());
  EGE_ASSERT((NULL != mutex) && mutex->isValid());
  if (m_p)
  {
    return m_p->wait(mutex);
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void WaitCondition::wakeOne()
{
  EGE_ASSERT(isValid());
  if (m_p)
  {
    m_p->wakeOne();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void WaitCondition::wakeAll()
{
  EGE_ASSERT(isValid());
  if (m_p)
  {
    m_p->wakeAll();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END