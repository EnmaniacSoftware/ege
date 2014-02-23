#ifndef EGE_AIRPLAY_SERVICES_DEVICESERVICES_PRIVATE_H
#define EGE_AIRPLAY_SERVICES_DEVICESERVICES_PRIVATE_H

#include <EGE.h>
#include <EGEString.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class DeviceServices;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class DeviceServicesPrivate
{
  public:

    DeviceServicesPrivate(DeviceServices* base);
   ~DeviceServicesPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS
    
    EGE_DECLARE_PUBLIC_IMPLEMENTATION(DeviceServices)

    /*! Creates object. */
    EGEResult construct();
    /*! Opens given URL in Web browser. */
    bool openUrl(const String& url);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_AIRPLAY_SERVICES_DEVICESERVICES_PRIVATE_H