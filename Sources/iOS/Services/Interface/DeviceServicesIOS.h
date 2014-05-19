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
    /*! @see DeviceServices::retrieveConfidentialValue. */
    EGEResult storeConfidentialValue(const String& name, const String& value) override;
    EGEResult storeConfidentialValue(const String& name, const PDataBuffer value) override;
    /*! @see DeviceServices::retrieveConfidentialValue. */
    EGEResult retrieveConfidentialValue(const String& name, String& value) override;
    EGEResult retrieveConfidentialValue(const String& name, PDataBuffer& value) override;

  private:

    /*! Open application rate URL. 
     *  @return Returns TRUE if requests was processed successfully. Otherwise, FALSE.
     */
    bool openApplicationRateURL();
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_IOS_SERVICES_DEVICESERVICES_H