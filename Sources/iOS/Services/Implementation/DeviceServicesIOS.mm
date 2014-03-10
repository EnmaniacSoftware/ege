#include "iOS/Services/Interface/DeviceServicesIOS.h"
#include "EGEDebug.h"
#import <UIKit/UIKit.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KConfidentialValuesPrefix = "ege.deviceservices.confidentialstore.";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DeviceServicesIOS::DeviceServicesIOS() : DeviceServices()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DeviceServicesIOS::~DeviceServicesIOS()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool DeviceServicesIOS::openUrl(const String& url)
{
  // convert URL
  NSString* nsUrl = [NSString stringWithCString: url.c_str() encoding: NSASCIIStringEncoding];
  
  // try to open it
  return (YES == [[UIApplication sharedApplication] openURL: [NSURL URLWithString: nsUrl]]);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult DeviceServicesIOS::storeConfidentialValue(const String& name, const String& value)
{
  // convert name and value
  NSString* nsName  = [NSString stringWithCString: (KConfidentialValuesPrefix + name).c_str() encoding: NSASCIIStringEncoding];
  NSString* nsValue = [NSString stringWithCString: value.c_str() encoding: NSASCIIStringEncoding];

  // store
  NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];

  [defaults setObject: nsValue forKey: nsName];

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult DeviceServicesIOS::storeConfidentialValue(const String& name, const PDataBuffer value)
{
  EGEResult result = EGE_SUCCESS;

  // begin transaction
  /*result = m_database.beginTransaction();
  if (EGE_SUCCESS == result)
  {
    // remove existing entry if present already
    SqlQuery deleteQuery = String("DELETE FROM %1 WHERE %2 = ?").arg(KConfidentialDBStoreTableName)
                                                                .arg(KConfidentialDBStoreTableColumnName);
    deleteQuery.addBindValue(name);

    result = m_database.execute(deleteQuery);
    if (EGE_SUCCESS == result)
    {
      // add new entry
      SqlQuery query = String("INSERT INTO %1 (%2, %3) VALUES (?, ?)").arg(KConfidentialDBStoreTableName)
                                                                      .arg(KConfidentialDBStoreTableColumnName)
                                                                      .arg(KConfidentialDBStoreTableColumnBlobs);
      query.addBindValue(name);
      query.addBindValue(value);

      result = m_database.execute(query);
    }

    if (EGE_SUCCESS != result)
    {
      // abort transaction
      m_database.abortTransaction();
    }
    else
    {
      // end transaction
      m_database.endTransaction();
    }
  }*/

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult DeviceServicesIOS::retrieveConfidentialValue(const String& name, String& value)
{
  EGEResult result = EGE_ERROR_NOT_FOUND;

  // convert name
  NSString* nsName = [NSString stringWithCString: (KConfidentialValuesPrefix + name).c_str() encoding: NSASCIIStringEncoding];

  // retrieve
  NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
  NSString* nsValue = [defaults stringForKey: nsName];
  EGE_ASSERT(nil != nsValue);
    
  if (nil != nsValue)
  {
    // convert to EGE format
    value = [nsValue cStringUsingEncoding: NSASCIIStringEncoding];
  
    // success
    result = EGE_SUCCESS;
  }
  
  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult DeviceServicesIOS::retrieveConfidentialValue(const String& name, PDataBuffer& value)
{
  EGEResult result = EGE_SUCCESS;
  /*
  SqlQuery query = String("SELECT %1 FROM %2 WHERE %3=?;").arg(KConfidentialDBStoreTableColumnStrings)
                                                          .arg(KConfidentialDBStoreTableName)
                                                          .arg(KConfidentialDBStoreTableColumnName);
  query.addBindValue(name);
  
  result = m_database.execute(query);
  if (EGE_SUCCESS == result)
  {
    // retrieve result
    PSqlResult sqlResult = m_database.result();
    if (NULL == sqlResult)
    {
      // error!
      result = EGE_ERROR;
    }
    else
    {
      // check if any data returned
      if (0 == sqlResult->rowCount())
      {
        // no data
        result = EGE_ERROR_NOT_FOUND;
      }
      else
      {
        // get output object
        PObject blobObject = sqlResult->value(0, KConfidentialDBStoreTableColumnBlobs);
        EGE_ASSERT(NULL != blobObject);
        EGE_ASSERT(EGE_OBJECT_UID_DATA_BUFFER == blobObject->uid());

        // store value
        value = ege_pcast<PDataBuffer>(blobObject);
      }
    }
  }*/

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
