#include "Win32/Tools/DesktopServicesWin32_p.h"
#include <windows.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(DesktopServicesPrivate)
EGE_DEFINE_DELETE_OPERATORS(DesktopServicesPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DesktopServicesPrivate::DesktopServicesPrivate(DesktopServices* base) : m_d(base)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DesktopServicesPrivate::~DesktopServicesPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult DesktopServicesPrivate::construct()
{
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool DesktopServicesPrivate::openUrl(const String& url)
{
  return (32 < reinterpret_cast<int>(ShellExecuteA(NULL, "open", url.toAscii(), NULL, NULL, SW_SHOWNORMAL)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END