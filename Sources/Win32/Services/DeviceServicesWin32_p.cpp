#include "Win32/Services/DeviceServicesWin32_p.h"
#include <windows.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(DeviceServicesPrivate)
EGE_DEFINE_DELETE_OPERATORS(DeviceServicesPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DeviceServicesPrivate::DeviceServicesPrivate(DeviceServices* base) : m_d(base)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DeviceServicesPrivate::~DeviceServicesPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult DeviceServicesPrivate::construct()
{
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool DeviceServicesPrivate::openUrl(const String& url)
{
  return (32 < reinterpret_cast<int>(ShellExecuteA(NULL, "open", url.toAscii(), NULL, NULL, SW_SHOWNORMAL)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END