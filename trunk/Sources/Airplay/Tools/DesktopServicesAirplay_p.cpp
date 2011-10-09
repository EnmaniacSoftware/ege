#include "Airplay/Tools/DesktopServicesAirplay_p.h"
#include <s3eOSExec.h>

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
  return S3E_RESULT_SUCCESS == s3eOSExecExecute(url.toAscii(), S3E_FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
