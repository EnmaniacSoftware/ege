#include "Core/Event/Implementation/EventManager.h"
#include "Core/Event/Interface/Event.h"
#include "Core/ComplexTypes.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* KEventManagerDebugName = "EGEEventManager";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(EventManager)
EGE_DEFINE_DELETE_OPERATORS(EventManager)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EventManager::EventManager()
: m_mutex(ege_new Mutex())
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EventManager::~EventManager()
{
  m_mutex = NULL;
  m_pendingEvents.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void EventManager::update(const Time& time)
{
  EGE_UNUSED(time);

  // check if any pending events
  if ( ! m_pendingEvents.isEmpty())
  {
    EventList localList;

    // make copy of the list
    if ( ! m_mutex->lock())
    {
      // damn...
      egeCritical(KEventManagerDebugName) << EGE_FUNC_INFO << "Could not lock mutex!!!";
    }

    localList.copy(m_pendingEvents);
    m_pendingEvents.clear();
    
    if ( ! m_mutex->unlock())
    {
      // damn...
      egeCritical(KEventManagerDebugName) << EGE_FUNC_INFO << "Could not unlock mutex!!!";
    }
    
    // propagate
    EventList::ConstIterator itEnd = localList.end();
    for (EventList::ConstIterator it = localList.begin(); it != itEnd; ++it)
    {
      notify(*it);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult EventManager::send(s32 id, bool immediate)
{
  PEvent event = ege_new Event(id);
  if (NULL == event)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // check if event is to be dispatched immediately
  if (immediate)
  {
    notify(event);
  }
  else
  {
    // add to pending
    MutexLocker locker(m_mutex);
    m_pendingEvents.append(event);
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult EventManager::send(s32 id, s32 data, bool immediate)
{
  PEvent event = ege_new Event(id);
  if (NULL == event)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  PInteger eventData = ege_new Integer(data);
  if (NULL == eventData)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  event->setData(eventData);

  // check if event is to be dispatched immediately
  if (immediate)
  {
    notify(event);
  }
  else
  {
    // add to pending
    MutexLocker locker(m_mutex);
    m_pendingEvents.append(event);
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult EventManager::send(s32 id, float32 data, bool immediate)
{
  PEvent event = ege_new Event(id);
  if (NULL == event)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  PFloat eventData = ege_new Float(data);
  if (NULL == eventData)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  event->setData(eventData);

  // check if event is to be dispatched immediately
  if (immediate)
  {
    notify(event);
  }
  else
  {
    // add to pending
    MutexLocker locker(m_mutex);
    m_pendingEvents.append(event);
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult EventManager::send(s32 id, PObject data, bool immediate)
{
  PEvent event = ege_new Event(id);
  if (NULL == event)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  event->setData(data);

  // check if event is to be dispatched immediately
  if (immediate)
  {
    notify(event);
  }
  else
  {
    // add to pending
    MutexLocker locker(m_mutex);
    m_pendingEvents.append(event);
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void EventManager::notify(PEvent event)
{
  const ListenersContainer& objects = listeners();

  // go thru all listners
  ListenersContainer::const_iterator iterEnd = objects.end();
  for (ListenersContainer::const_iterator iter = objects.begin(); iter != iterEnd; ++iter)
  {
    (*iter)->onEventRecieved(event);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool EventManager::registerListener(IEventListener* listener)
{
  return addListener(listener);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void EventManager::unregisterListener(IEventListener* listener)
{
  removeListener(listener);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 EventManager::uid() const
{
  return EGE_OBJECT_UID_EVENT_MANAGER_MODULE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END