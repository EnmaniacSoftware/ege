#include "Core/Services/DeviceServices.h"
#include "EGEDebug.h"

#ifdef EGE_PLATFORM_WIN32
  #include "Win32/Services/DeviceServicesWin32_p.h"
#elif EGE_PLATFORM_AIRPLAY
  #include "Airplay/Services/DeviceServicesAirplay_p.h"
#elif EGE_PLATFORM_IOS
  #include "iOS/Services/DeviceServicesIOS_p.h"
#endif

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(DeviceServices)
EGE_DEFINE_DELETE_OPERATORS(DeviceServices)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DeviceServices::DeviceServices() : m_p(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DeviceServices::~DeviceServices()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult DeviceServices::construct()
{
  EGEResult result;

  // create private implemenation
  m_p = ege_new DeviceServicesPrivate(this);
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
bool DeviceServices::openUrl(const String& url)
{
  return p_func()->openUrl(url);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END