#include "Win32/Tools/DesktopServicesWin32_p.h"

EGE_NAMESPACE

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
/*! Opens given URL in Web browser. */
bool DesktopServicesPrivate::openUrl(const String& url)
{
  return (32 < reinterpret_cast<int>(ShellExecuteA(NULL, "open", url.toAscii(), NULL, NULL, SW_SHOWNORMAL)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
