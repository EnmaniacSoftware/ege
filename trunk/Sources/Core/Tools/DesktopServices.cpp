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
DesktopServices::DesktopServices()
{
  m_p = ege_new DesktopServicesPrivate(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DesktopServices::~DesktopServices()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool DesktopServices::isValid() const
{
  return (NULL != m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Opens given URL in Web browser. */
bool DesktopServices::openUrl(const String& url)
{
  if (isValid())
  {
    return p_func()->openUrl(url);
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END