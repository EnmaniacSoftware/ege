#include "Airplay/Services/DeviceServicesAirplay_p.h"
#include <s3eOSExec.h>

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
  return S3E_RESULT_SUCCESS == s3eOSExecExecute(url.toAscii(), S3E_FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END