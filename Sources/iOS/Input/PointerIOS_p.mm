#include "iOS/Input/PointerIOS_p.h"
#include "Core/Input/Pointer.h"
#include "Core/Engine/Interface/EngineInternal.h"
#include "Core/Engine/Interface/EngineInstance.h"
#include "EGEEngine.h"

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
  d_func()->engine().eventManager()->removeListener(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult PointerPrivate::construct()
{
  // subscribe for notifications
  if ( ! d_func()->engine().eventManager()->addListener(this))
  {
    // error!
    return EGE_ERROR;
  }
  
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PointerPrivate::onEventRecieved(PEvent event)
{
  EngineInternal& engineInternal = static_cast<EngineInternal&>(reinterpret_cast<EngineInstance&>(d_func()->engine()));
  
  // check if quitting already
  if (engineInternal.isShuttingDown())
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