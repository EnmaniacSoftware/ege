#include "Core/Event/EventManager.h"
#include "Core/Event/Event.h"
#include "EGETypes.h"
#include "Core/ComplexTypes.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(EventManager)
EGE_DEFINE_DELETE_OPERATORS(EventManager)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EventManager::EventManager(Application* app) : Object(app)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EventManager::~EventManager()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult EventManager::send(s32 id)
{
  PEvent event = ege_new Event(app(), id);
  if (NULL == event)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // notify listeners
  notify(event);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult EventManager::send(s32 id, s32 data)
{
  PEvent event = ege_new Event(app(), id);
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

  // notify listeners
  notify(event);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult EventManager::send(s32 id, float32 data)
{
  PEvent event = ege_new Event(app(), id);
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

  // notify listeners
  notify(event);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult EventManager::send(s32 id, PObject data)
{
  PEvent event = ege_new Event(app(), id);
  if (NULL == event)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  event->setData(data);

  // notify listeners
  notify(event);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void EventManager::notify(PEvent event)
{
  const ListenersContainer& objects = listeners();

  // go thru all listners
  for (ListenersContainer::const_iterator iter = objects.begin(); iter != objects.end(); ++iter)
  {
    (*iter)->onEventRecieved(event);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END