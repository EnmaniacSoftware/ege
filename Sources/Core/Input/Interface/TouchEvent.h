#ifndef EGE_CORE_TOUCHEVENT_H
#define EGE_CORE_TOUCHEVENT_H

/*! Specific implementation of PointerEvent for touch devices. */

#include "EGE.h"
#include "Core/Input/Interface/PointerEvent.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_PREDECLARE_SMART_CLASS(TouchEvent, PTouchEvent)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class TouchEvent : public PointerEvent
{
  public:

    TouchEvent(PointerAction action, s32 x, s32 y, s32 index);
   ~TouchEvent();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Returns touch index. */
    s32 index() const;

  private:

    /*! Touch index. */
    s32 m_index;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_TOUCHEVENT_H