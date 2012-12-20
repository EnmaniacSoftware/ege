#include "Core/Input/Pointer.h"

#ifdef EGE_PLATFORM_WIN32
#include "Win32/Input/PointerWin32_p.h"
#elif EGE_PLATFORM_AIRPLAY
#include "Airplay/Input/PointerAirplay_p.h"
#endif

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Pointer)
EGE_DEFINE_DELETE_OPERATORS(Pointer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Pointer::Pointer(Application* app) : Object(app),
                                     m_p(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Pointer::~Pointer()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult Pointer::construct()
{
  EGEResult result;

  // create private implementation
  m_p = ege_new PointerPrivate(this);
  if (NULL == m_p)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // construct private implementation
  if (EGE_SUCCESS != (result = m_p->construct()))
  {
    // error!
    return result;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END