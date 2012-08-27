#include "Core/NativeUI/MessageBox.h"

#ifdef EGE_PLATFORM_WIN32
#include "Win32/NativeUI/MessageBoxWin32_p.h"
#elif EGE_PLATFORM_AIRPLAY
#include "Airplay/NativeUI/MessageBoxAirplay_p.h"
#endif

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(MessageBox)
EGE_DEFINE_DELETE_OPERATORS(MessageBox)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MessageBox::MessageBox(const Text& title, const Text& content) : Object(NULL),
                                                                 m_p(NULL),
                                                                 m_title(title),
                                                                 m_content(content)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MessageBox::~MessageBox()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Constructs object. */
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
/*! Shows message box. */
void MessageBox::show()
{
  if (NULL != m_p)
  {
    m_p->show();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Convinience method. Shows message box with given title and content. */
void MessageBox::Show(const Text& title, const Text& content)
{
  MessageBox msg(title, content);
  if (EGE_SUCCESS == msg.construct())
  {
    msg.show();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END