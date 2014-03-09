#ifndef EGE_WIN32_SERVICES_DEVICESERVICES_H
#define EGE_WIN32_SERVICES_DEVICESERVICES_H

/*! Windows implementation of Device Services.
 */

#include "EGE.h"
#include "EGEString.h"
#include "Core/Services/Interface/DeviceServices.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class DeviceServicesWin32 : public DeviceServices
{
  public:

    DeviceServicesWin32();
   ~DeviceServicesWin32();

    /*! @see DeviceServices::openUrl. */
    bool openUrl(const String& url) override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_WIN32_SERVICES_DEVICESERVICES_H