#ifndef EGE_CORE_MOUSEEVENT_H
#define EGE_CORE_MOUSEEVENT_H

/*! Specific implementation of PointerEvent for mouse-alike devices. */

#include "EGE.h"
#include "Core/Input/Interface/KeyModifiers.h"
#include "Core/Input/Interface/PointerEvent.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Available buttons. */
enum MouseButton
{
  EMouseButtonNone = 0,
  EMouseButtonLeft,
  EMouseButtonRight,
  EMouseButtonWheelUp,
  EMouseButtonWheelDown
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_PREDECLARE_SMART_CLASS(MouseEvent, PMouseEvent)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class MouseEvent : public PointerEvent
{
  public:

    MouseEvent(PointerAction action, MouseButton button, KeyboardModifiers modifiers, s32 x, s32 y);
   ~MouseEvent();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Returns button type. */
    MouseButton button() const;
    /*! Returns keyboard modifiers. */
    const KeyboardModifiers& modifiers() const;

  private:

    /*! Button type. */
    MouseButton m_button;
    /*! Keyboard modifier. */
    KeyboardModifiers m_modifiers;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MOUSEEVENT_H