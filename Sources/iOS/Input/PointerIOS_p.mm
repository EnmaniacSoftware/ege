#include "Core/Application/Application.h"
#include "Core/Input/Pointer.h"
#include "iOS/Input/PointerIOS_p.h"
#include "Core/ListenerContainer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(PointerPrivate)
EGE_DEFINE_DELETE_OPERATORS(PointerPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PointerPrivate::PointerPrivate(Pointer* base) : m_d(base)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PointerPrivate::~PointerPrivate()
{
  d_func()->app()->eventManager()->removeListener(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult PointerPrivate::construct()
{
  // subscribe for notifications
  if ( ! d_func()->app()->eventManager()->addListener(this))
  {
    // error!
    return EGE_ERROR;
  }
  
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PointerPrivate::onEventRecieved(PEvent event)
{
  // check if quitting already
  if (d_func()->app()->isQuitting())
  {
    // do not propagate
    return;
  }
  
  switch (event->id())
  {
    case EGE_EVENT_ID_INTERNAL_POINTER_DATA:
      
      // emit signal
      emit d_func()->eventSignal(event->data());
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END