#include <EGEWaitCondition.h>
#include <EGEDebug.h>

#ifdef EGE_THREAD_PTHREAD
#include "Core/Threading/PThread/WaitCondition_p.h"
#endif // EGE_THREAD_PTHREAD

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(WaitCondition)
EGE_DEFINE_DELETE_OPERATORS(WaitCondition)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
WaitCondition::WaitCondition(Application* app) : Object(app, EGE_OBJECT_UID_WAIT_CONDITION)
{
  m_p = ege_new WaitConditionPrivate(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
WaitCondition::~WaitCondition()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool WaitCondition::isValid() const
{
  return (NULL != m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Locks mutex. */
bool WaitCondition::lock()
{
  EGE_ASSERT(isValid());
  if (m_p)
  {
    return m_p->lock();
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Unlocks mutex. */
bool WaitCondition::unlock()
{
  EGE_ASSERT(isValid());
  if (m_p)
  {
    return m_p->unlock();
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
