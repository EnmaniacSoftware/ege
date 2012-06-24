#include "Core/Input/PointerData.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(PointerData)
EGE_DEFINE_DELETE_OPERATORS(PointerData)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PointerData::PointerData(Action action, Button button, KeyboardModifiers modifiers, s32 x, s32 y, s32 index) 
: Object(NULL, EGE_OBJECT_UID_POINTER_DATA), 
  m_button(button), 
  m_action(action), 
  m_x(x), 
  m_y(y), 
  m_index(index),
  m_modifiers(modifiers)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PointerData::~PointerData()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END