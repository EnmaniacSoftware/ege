#ifndef EGE_CORE_SERVICES_SPECIALURLS_H
#define EGE_CORE_SERVICES_SPECIALURLS_H

/*! This file lists all available special URLs. These can be passed as an argument to DeviceServices::openUrl method. 
 */

#include "EGE.h"
#include "EGEString.h"

EGE_NAMESPACE_BEGIN

/*! Opens web page where current application can be rated. */
extern const String KSpecialURLRateApp;
/*! Opens web page where current application can be gifted. */
extern const String KSpecialURLGiftApp;

EGE_NAMESPACE_END

#endif // EGE_CORE_SERVICES_SPECIALURLS_H