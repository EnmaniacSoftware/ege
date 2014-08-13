#include "Core/Services/Interface/SpecialURLs.h"
#include "Win32/Services/Interface/DeviceServicesWin32.h"
#include "EGEFile.h"
#include "EGEDebug.h"
#include <windows.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
extern const char* KDeviceServicesDebugName;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KConfidentialDBName                      = "confidential.sqlite";
static const char* KConfidentialDBStoreTableName            = "Store";
static const char* KConfidentialDBStoreTableColumnName      = "Name";
static const char* KConfidentialDBStoreTableColumnStrings   = "String";
static const char* KConfidentialDBStoreTableColumnBlobs     = "Blob";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DeviceServicesWin32::DeviceServicesWin32(Engine& engine) : DeviceServices(engine)
{
  // open database
  if (EGE_SUCCESS != m_database.open(KConfidentialDBName, false, ! File::Exists(KConfidentialDBName)))
  {
    // error!
    egeWarning(KDeviceServicesDebugName) << "Could not open databse:" << KConfidentialDBName;
  }
  else
  {
    String queryString = "CREATE TABLE IF NOT EXISTS %1 (%2 TEXT PRIMARY KEY NOT NULL, %3 TEXT, %4 BLOB);";
    queryString = queryString.arg(KConfidentialDBStoreTableName)
                             .arg(KConfidentialDBStoreTableColumnName)
                             .arg(KConfidentialDBStoreTableColumnStrings)
                             .arg(KConfidentialDBStoreTableColumnBlobs);
                                                                                                                          
    SqlQuery query(queryString);
    if (EGE_SUCCESS != m_database.execute(query))
    {
      // error!
      egeWarning(KDeviceServicesDebugName) << "Could not create table in database:" << KConfidentialDBName;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DeviceServicesWin32::~DeviceServicesWin32()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool DeviceServicesWin32::openUrl(const String& url)
{
  // check for special URLs
  if (KSpecialURLRateApp == url)
  {
    // not supported
    return false;
  }

  // process given URL
  return (32 < reinterpret_cast<int>(ShellExecuteA(NULL, "open", url.toAscii(), NULL, NULL, SW_SHOWNORMAL)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool DeviceServicesWin32::mailTo(const Text& recipient, const Text& title, const Text& body)
{
  // compose URL
  Text url = "mailto:%1?subject=%2&body=%3";
  url = url.arg(recipient).arg(title).arg(body);

  // process given URL
  return (32 < reinterpret_cast<int>(ShellExecuteW(NULL, L"open", url.data(), NULL, NULL, SW_SHOWNORMAL)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult DeviceServicesWin32::storeConfidentialValue(const String& name, const String& value)
{
  EGEResult result = EGE_SUCCESS;

  // begin transaction
  result = m_database.beginTransaction();
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
                                                                      .arg(KConfidentialDBStoreTableColumnStrings);
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
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult DeviceServicesWin32::storeConfidentialValue(const String& name, const PDataBuffer value)
{
  EGEResult result = EGE_SUCCESS;

  // begin transaction
  result = m_database.beginTransaction();
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
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult DeviceServicesWin32::retrieveConfidentialValue(const String& name, String& value)
{
  EGEResult result = EGE_SUCCESS;

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
        PObject valueObject = sqlResult->value(0, KConfidentialDBStoreTableColumnStrings);
        EGE_ASSERT(NULL != valueObject);
        EGE_ASSERT(EGE_OBJECT_UID_STRING_BUFFER == valueObject->uid());

        // convert to string buffer
        PStringBuffer stringBuffer = ege_pcast<PStringBuffer>(valueObject);
        EGE_ASSERT(NULL != stringBuffer);

        // store value
        value = stringBuffer->string();
      }
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult DeviceServicesWin32::retrieveConfidentialValue(const String& name, PDataBuffer& value)
{
  EGEResult result = EGE_SUCCESS;

  SqlQuery query = String("SELECT %1 FROM %2 WHERE %3=?;").arg(KConfidentialDBStoreTableColumnBlobs)
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
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END