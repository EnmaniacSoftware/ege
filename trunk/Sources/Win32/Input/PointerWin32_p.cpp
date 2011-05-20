#include "Core/Application/Application.h"
#include "Core/Input/Pointer.h"
#include "Win32/Input/PointerWin32_p.h"

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
  switch (event->uid())
  {
    case EGE_EVENT_UID_INTERNAL_POINTER_DATA:

      // propagate as valid event
      base()->app()->eventManager()->send(EGE_EVENT_UID_CORE_POINTER_DATA, event->data());
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
