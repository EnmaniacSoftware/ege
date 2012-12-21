#include "Core/Tools/DesktopServices.h"

#ifdef EGE_PLATFORM_WIN32
  #include "Win32/Tools/DesktopServicesWin32_p.h"
#elif EGE_PLATFORM_AIRPLAY
  #include "Airplay/Tools/DesktopServicesAirplay_p.h"
#endif

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(DesktopServices)
EGE_DEFINE_DELETE_OPERATORS(DesktopServices)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DesktopServices::DesktopServices() : m_p(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DesktopServices::~DesktopServices()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult DesktopServices::construct()
{
  EGEResult result;

  // create private implemenation
  m_p = ege_new DesktopServicesPrivate(this);
  if (NULL == m_p)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  if (EGE_SUCCESS != (result = m_p->construct()))
  {
    // error!
    return result;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool DesktopServices::openUrl(const String& url)
{
  return p_func()->openUrl(url);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END