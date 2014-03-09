#ifndef EGE_IOS_SERVICES_DEVICESERVICES_H
#define EGE_IOS_SERVICES_DEVICESERVICES_H

/*! iOS implementation of Device Services.
 */

#include "EGE.h"
#include "EGEString.h"
#include "Core/Services/Interface/DeviceServices.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class DeviceServicesIOS : public DeviceServices
{
  public:

    DeviceServicesIOS();
   ~DeviceServicesIOS();

    /*! @see DeviceServices::openUrl. */
    bool openUrl(const String& url) override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_IOS_SERVICES_DEVICESERVICES_H