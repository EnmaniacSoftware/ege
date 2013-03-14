#ifndef EGE_CORE_POINTERDATA_H
#define EGE_CORE_POINTERDATA_H

#include "EGE.h"
#include "EGEInput.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(PointerData, PPointerData)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PointerData : public Object
{
  public:

    PointerData(Action action, Button button, KeyboardModifiers modifiers, s32 x, s32 y, s32 index);
   ~PointerData();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Returns button type. */
    Button button() const { return m_button; }
    /*! Returns action type. */
    Action action() const { return m_action; }
    /*! Returns X position. */
    s32 x() const { return m_x; }
    /*! Returns Y position. */
    s32 y() const { return m_y; }
    /*! Returns pointer index. */
    s32 index() const { return m_index; }
    /*! Returns keyboard modifiers. */
    const KeyboardModifiers& modifiers() const { return m_modifiers; }

  private:

    /*! Pointer index. */
    s32 m_index;
    /*! X position of event. */
    s32 m_x;
    /*! Y position of event. */
    s32 m_y;
    /*! Button type. */
    Button m_button;
    /*! Action type. */
    Action m_action;
    /*! Keyboard modifier. */
    KeyboardModifiers m_modifiers;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_POINTERDATA_H