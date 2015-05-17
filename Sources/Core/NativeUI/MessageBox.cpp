#include "Core/NativeUI/MessageBox.h"
#include "EGEAssert.h"

#ifdef EGE_PLATFORM_WIN32
  #include "Win32/NativeUI/MessageBoxWin32_p.h"
#elif EGE_PLATFORM_AIRPLAY
  #include "Airplay/NativeUI/MessageBoxAirplay_p.h"
#elif EGE_PLATFORM_IOS
  #include "iOS/NativeUI/MessageBoxIOS_p.h"
#endif

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(MessageBox)
EGE_DEFINE_DELETE_OPERATORS(MessageBox)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MessageBox::MessageBox(const Text& title, const Text& content, MessageBoxButtons buttons) 
: Object()
, m_p(NULL)
, m_title(title)
, m_content(content)
, m_buttons(buttons)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MessageBox::~MessageBox()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult MessageBox::construct()
{
  EGEResult result = EGE_SUCCESS;

  // create private
  m_p = ege_new MessageBoxPrivate(this);
  if (NULL == m_p)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // construct
  if (EGE_SUCCESS != (result = m_p->construct()))
  {
    // error!
    return result;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 MessageBox::show()
{
  if (NULL != m_p)
  {
    return m_p->show();
  }

  return -1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 MessageBox::Show(const Text& title, const Text& content, MessageBoxButtons buttons)
{
  MessageBox msg(title, content, buttons);
  if (EGE_SUCCESS == msg.construct())
  {
    return msg.show();
  }

  return -1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END