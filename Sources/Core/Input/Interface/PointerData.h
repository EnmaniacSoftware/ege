#ifndef EGE_CORE_POINTERDATA_H
#define EGE_CORE_POINTERDATA_H

#include "EGE.h"
#include "EGEFlags.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Available buttons. */
enum PointerButton
{
  EPointerButtonNone = 0,
  EPointerButtonLeft,
  EPointerButtonRight,
  EPointerButtonWheelUp,
  EPointerButtonWheelDown
};

/*! Available actions. */
enum PointerAction
{
  EPointerActionButtonDown = 0,
  EPointerActionButtonUp,
  EPointerActionMove
};

/*! Available keyboard modifiers. */
enum KeyboardModifiersFlags
{
  EKeyModifierNone  = 0x00,
  EKeyModifierShift = 0x01,
  EKeyModifierAlt   = 0x02,
  EKeyModifierCtrl  = 0x04
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_FLAGS(KeyboardModifiers, KeyboardModifiersFlags)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(PointerData, PPointerData)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PointerData : public Object
{
  public:

    PointerData(PointerAction action, PointerButton button, KeyboardModifiers modifiers, s32 x, s32 y, s32 index);
   ~PointerData();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Returns button type. */
    PointerButton button() const;
    /*! Returns action type. */
    PointerAction action() const;
    /*! Returns X position. */
    s32 x() const;
    /*! Returns Y position. */
    s32 y() const;
    /*! Returns pointer index. */
    s32 index() const;
    /*! Returns keyboard modifiers. */
    const KeyboardModifiers& modifiers() const;

  private:

    /*! Pointer index. */
    s32 m_index;
    /*! X position of event. */
    s32 m_x;
    /*! Y position of event. */
    s32 m_y;
    /*! Button type. */
    PointerButton m_button;
    /*! Action type. */
    PointerAction m_action;
    /*! Keyboard modifier. */
    KeyboardModifiers m_modifiers;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_POINTERDATA_H