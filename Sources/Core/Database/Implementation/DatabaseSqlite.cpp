#include "Core/Database/Interface/DatabaseSqlite.h"
#include "EGEDebug.h"
#include <sqlite3.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KDatabaseSqliteDebugName = "EGEDatabaseSqlite";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(DatabaseSqlite)
EGE_DEFINE_DELETE_OPERATORS(DatabaseSqlite)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DatabaseSqlite::DatabaseSqlite() : Database()
                                 , m_handle(NULL)
                                 , m_inTransaction(false)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DatabaseSqlite::~DatabaseSqlite()
{
  close();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult DatabaseSqlite::open(const String& path, bool readOnly, bool create)
{
  EGEResult result = EGE_ERROR;

  // open database
  int openFlags  = readOnly ? SQLITE_OPEN_READONLY : SQLITE_OPEN_READWRITE;
  openFlags     |= create ? SQLITE_OPEN_CREATE : 0;

  ::sqlite3* database = NULL;
  int sqliteResult = ::sqlite3_open_v2(path.toAscii(), &database, openFlags, NULL);

  // process result
  switch (sqliteResult)
  {
    case SQLITE_CANTOPEN: result = EGE_ERROR_NOT_FOUND; break;
    case SQLITE_OK:       result = EGE_SUCCESS; break;
  }

  // check if success
  if (EGE_SUCCESS == result)
  {
    // store handle
    m_handle = database;
  }
  else
  {
    egeWarning(KDatabaseSqliteDebugName) << "Could not open database:" << path;

    // close data base
    // NOTE: it is required to close database even if opening of it failed
    if (NULL != database)
    {
      ::sqlite3_close_v2(database);
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult DatabaseSqlite::close()
{
  EGEResult result = EGE_ERROR;

  // check if any database open
  if (NULL != m_handle)
  {
    int sqliteResult = ::sqlite3_close_v2(m_handle);

    // process return value
    switch (sqliteResult)
    {
      case SQLITE_OK: result = EGE_SUCCESS; break;

      default:

        egeWarning(KDatabaseSqliteDebugName) << "Could not close database.";
        break;
    }

    // clean up
    m_handle = NULL;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult DatabaseSqlite::beginTransaction()
{
  EGE_ASSERT( ! m_inTransaction);
  EGE_ASSERT(NULL != m_handle);

  EGEResult result = EGE_ERROR;

  if ( ! m_inTransaction && (NULL != m_handle))
  {
    // beging transaction
    int sqliteResult = ::sqlite3_exec(m_handle, "begin transaction;", NULL, NULL, NULL);

    // process result
    switch (sqliteResult)
    {
      case SQLITE_OK: result = EGE_SUCCESS; break;

      default:

        egeWarning(KDatabaseSqliteDebugName) << "Could not start transaction. Sqlite error: " << sqliteResult;
        break;
    }
  }

  // check if success
  if (EGE_SUCCESS == result)
  {
    // mark in transaction
    m_inTransaction = true;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult DatabaseSqlite::endTransaction()
{
  EGE_ASSERT(m_inTransaction);
  EGE_ASSERT(NULL != m_handle);

  EGEResult result = EGE_ERROR;

  if (m_inTransaction && (NULL != m_handle))
  {
    // commit transaction
    int sqliteResult = ::sqlite3_exec(m_handle, "commit;", NULL, NULL, NULL);

    // process result
    switch (sqliteResult)
    {
      case SQLITE_OK: result = EGE_SUCCESS; break;

      default:

        egeWarning(KDatabaseSqliteDebugName) << "Could not commit transaction. Aborting. Sqlite error: " << sqliteResult;

        // abort transaction
        abortTransaction();
        break;
    }
  }

  // check if success
  if (EGE_SUCCESS == result)
  {
    // mark no transaction
    m_inTransaction = false;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult DatabaseSqlite::abortTransaction()
{
  EGE_ASSERT(m_inTransaction);
  EGE_ASSERT(NULL != m_handle);

  EGEResult result = EGE_ERROR;

  if (m_inTransaction && (NULL != m_handle))
  {
    // rollback transaction
    int sqliteResult = ::sqlite3_exec(m_handle, "rollback transaction;", NULL, NULL, NULL);
   
    // process result
    switch (sqliteResult)
    {
      case SQLITE_OK: result = EGE_SUCCESS; break;

      default:

        egeWarning(KDatabaseSqliteDebugName) << "Could not rollback transaction. Sqlite error: " << sqliteResult;
        break;
    }
  }

  if (EGE_SUCCESS == result)
  {
    // mark no transaction
    m_inTransaction = false;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
