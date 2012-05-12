#include "Core/Application/Application.h"
#include "Core/Input/Pointer.h"
#include "Win32/Input/PointerWin32_p.h"
#include "Core/ListenerContainer.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(PointerPrivate)
EGE_DEFINE_DELETE_OPERATORS(PointerPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PointerPrivate::PointerPrivate(Pointer* base) : m_base(base)
{
  base->app()->eventManager()->addListener(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PointerPrivate::~PointerPrivate()
{
  base()->app()->eventManager()->removeListener(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool PointerPrivate::isValid() const
{
  return base()->app()->eventManager()->isListening(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Event reciever. */
void PointerPrivate::onEventRecieved(PEvent event)
{
  // check if quitting already
  if (base()->app()->isQuitting())
  {
    // do not propagate
    return;
  }

  switch (event->id())
  {
    case EGE_EVENT_ID_INTERNAL_POINTER_DATA:

      // emit signal
      emit base()->eventSignal(event->data());
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
