#include "iOS/Input/Interface/PointerIOS_p.h"
#include "Core/Input/Interface/Pointer.h"
#include "EGEEngine.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(PointerPrivate)
EGE_DEFINE_DELETE_OPERATORS(PointerPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PointerPrivate::PointerPrivate(Pointer* base)
: m_d(base)
{
  d_func()->engine().eventManager()->registerListener(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PointerPrivate::~PointerPrivate()
{
  d_func()->engine().eventManager()->unregisterListener(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PointerPrivate::onEventRecieved(PEvent event)
{
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