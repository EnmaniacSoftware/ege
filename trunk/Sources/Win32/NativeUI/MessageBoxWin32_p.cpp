#include "Win32/NativeUI/MessageBoxWin32_p.h"
#include "Core/NativeUI/MessageBox.h"
#include <windows.h>

#ifdef MessageBox
#undef MessageBox
#endif // MessageBox

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(MessageBoxPrivate)
EGE_DEFINE_DELETE_OPERATORS(MessageBoxPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MessageBoxPrivate::MessageBoxPrivate(MessageBox* base) : m_d(base)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MessageBoxPrivate::~MessageBoxPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult MessageBoxPrivate::construct()
{
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 MessageBoxPrivate::show()
{
  int result = MessageBoxW(NULL, d_func()->m_content.data(), d_func()->m_title.data(), 0);

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END