#ifndef EGE_CORE_SERVICES_DEVICESERVICES_H
#define EGE_CORE_SERVICES_DEVICESERVICES_H

/*! Interface class for platform specific implementations.
 */

#include "EGE.h"
#include "EGEString.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class DeviceServices
{
  public:

    DeviceServices();
    virtual ~DeviceServices();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS
    
    /*! Opens given URL in Web browser. */
    virtual bool openUrl(const String& url) = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SERVICES_DEVICESERVICES_H