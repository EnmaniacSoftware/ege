#include "Airplay/Tools/DesktopServicesAirplay_p.h"
#include <s3eOSExec.h>

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
  return S3E_RESULT_SUCCESS == s3eOSExecExecute(url.toAscii(), S3E_FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END