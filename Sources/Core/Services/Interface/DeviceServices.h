#ifndef EGE_CORE_SERVICES_DEVICESERVICES_H
#define EGE_CORE_SERVICES_DEVICESERVICES_H

/*! Interface class for platform specific implementations.
 */

#include "EGE.h"
#include "EGEString.h"
#include "EGEDataBuffer.h"

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
    /*! Stores value in secured area. 
     *  @param  name  Name of the value.
     *  @param  value Value to be stored.
     *  @return EGE_SUCCESS on success. Otherwise, one of the available errors.
     */
    virtual EGEResult storeConfidentialValue(const String& name, const String& value) = 0;
    virtual EGEResult storeConfidentialValue(const String& name, const PDataBuffer value) = 0;
    /*! Retrieves value from secured area. 
     *  @param  name  Name of the value.
     *  @param  value Placeholder for retrieved value.
     *  @return EGE_SUCCESS on success. Otherwise, one of the available errors.
     */
    virtual EGEResult retrieveConfidentialValue(const String& name, String& value) = 0;
    virtual EGEResult retrieveConfidentialValue(const String& name, PDataBuffer& value) = 0;
    /*! Returns device-specific AES 128-bit encryption key. 
     *  @return DataBuffer containing key information. NULL if error occured.
     */
    PDataBuffer aesEnctyptionKey();
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SERVICES_DEVICESERVICES_H