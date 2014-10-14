#ifndef EGE_CORE_KEYEVENT_H
#define EGE_CORE_KEYEVENT_H

#include "EGE.h"
#include "Core/Input/Interface/Key.h"
#include "Core/Input/Interface/KeyModifiers.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Available actions. */
enum KeyAction
{
  EKeyActionDown = 0,
  EKeyActionUp
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(KeyEvent, PKeyEvent)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class KeyEvent : public Object
{
  public:

    KeyEvent(Key key, KeyAction action, KeyboardModifiers modifiers);
   ~KeyEvent();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Returns action type. */
    KeyAction action() const;
    /*! Returns key. */
    Key key() const;
    /*! Returns keyboard modifiers. */
    const KeyboardModifiers& modifiers() const;

  private:

    /*! Key type. */
    Key m_key;
    /*! Action type. */
    KeyAction m_action;
    /*! Keyboard modifier. */
    KeyboardModifiers m_modifiers;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_KEYEVENT_H