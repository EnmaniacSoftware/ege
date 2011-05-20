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

    PointerData(EGEInput::EButton button, EGEInput::EButtonState state, s32 x, s32 y, s32 index);
   ~PointerData();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Returns button type. */
    inline EGEInput::EButton button() const { return m_button; }
    /*! Returns button state. */
    inline EGEInput::EButtonState state() const { return m_state; }
    /*! Returns X position. */
    inline s32 x() const { return m_x; }
    /*! Returns Y position. */
    inline s32 y() const { return m_y; }
    /*! Returns pointer index. */
    inline s32 index() const { return m_index; }

  private:

    /*! Pointer index. */
    s32 m_index;
    /*! X position of event. */
    s32 m_x;
    /*! Y position of event. */
    s32 m_y;
    /*! Button type. */
    EGEInput::EButton m_button;
    /*! Button state type. */
    EGEInput::EButtonState m_state;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_POINTERDATA_H