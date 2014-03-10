#ifndef EGE_WIN32_SERVICES_DEVICESERVICES_H
#define EGE_WIN32_SERVICES_DEVICESERVICES_H

/*! Windows implementation of Device Services.
 */

#include "EGE.h"
#include "EGEString.h"
#include "EGEDatabase.h"
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
    /*! @see DeviceServices::retrieveConfidentialValue. */
    EGEResult storeConfidentialValue(const String& name, const String& value) override;
    EGEResult storeConfidentialValue(const String& name, const PDataBuffer value) override;
    /*! @see DeviceServices::retrieveConfidentialValue. */
    EGEResult retrieveConfidentialValue(const String& name, String& value) override;
    EGEResult retrieveConfidentialValue(const String& name, PDataBuffer& value) override;

  private:

    /*! Confidential database. */
    DatabaseSqlite m_database;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_WIN32_SERVICES_DEVICESERVICES_H