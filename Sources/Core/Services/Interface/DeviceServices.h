#ifndef EGE_CORE_SERVICES_DEVICESERVICES_H
#define EGE_CORE_SERVICES_DEVICESERVICES_H

/*! Interface class for platform specific implementations.
 */

#include "EGE.h"
#include "EGEString.h"
#include "EGEText.h"
#include "EGEDataBuffer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class DeviceServices
{
  public:

    DeviceServices(Application* application);
    virtual ~DeviceServices();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS
  
  public:
  
    /*! Returns pointer to application object. */
    Application* app() const;
  
    /*! Opens given URL in web browser.
     *  @param  url URL to open in the web browser.
     *  @return TRUE if URL could be processed. Otherwise, FALSE.
     *  @note Given URL can also be one of the special URLs.
     */
    virtual bool openUrl(const String& url) = 0;
    /*! Opens built-in email application.
     *  @param  recipient Main recipient of the email.
     *  @param  title     Message title.
     *  @param  body      Message body text.
     *  @return TRUE if opening of email application has succeeded. Otherwise, FALSE.
     */
    virtual bool mailTo(const Text& recipient, const Text& title, const Text& body) = 0;

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
     *  @return EGE_SUCCESS on success, EGE_ERROR_NOT_FOUND if value with given name has not been found. Otherwise, one of the available errors.
     */
    virtual EGEResult retrieveConfidentialValue(const String& name, String& value) = 0;
    virtual EGEResult retrieveConfidentialValue(const String& name, PDataBuffer& value) = 0;
    /*! Returns device-specific AES 128-bit encryption key. 
     *  @return DataBuffer containing key information. NULL if error occured.
     */
    PDataBuffer aesEnctyptionKey();

  private:
  
    /*! Pointer to application object. */
    Application* m_application;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SERVICES_DEVICESERVICES_H