#include "Core/Database/Interface/DatabaseSqlite.h"
#include "Core/Database/Implementation/SqlResultSqlite.h"
#include "Core/ComplexTypes.h"
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
                                 , m_result(NULL)
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
  EGE_ASSERT(NULL == m_handle);

  EGEResult result = EGE_ERROR;

  if (NULL == m_handle)
  {
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
     
      default:

        result = EGE_ERROR;
        break;
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
EGEResult DatabaseSqlite::execute(const SqlQuery& query)
{
  EGE_ASSERT(NULL != m_handle);

  EGEResult result = EGE_SUCCESS;

  // allocate result object
  PSqlResult resultObject = ege_new SqlResultSqlite();
  if (NULL == resultObject)
  {
    // error!
    result = EGE_ERROR_NO_MEMORY;
  }

  if (EGE_SUCCESS == result)
  {
    ::sqlite3_stmt* statementObject = NULL;

    // prepare statement object
    int sqlResult = ::sqlite3_prepare_v2(m_handle, query.value().toAscii(), static_cast<int>(query.value().length()), &statementObject, NULL);
    switch (sqlResult)
    {
      case SQLITE_OK: result = EGE_SUCCESS; break;
      default:

        // error!
        result = EGE_ERROR;
        egeWarning(KDatabaseSqliteDebugName) << "Could not prepare statement object! Sqlite error:" << sqlResult;
        break;
    }

    if (EGE_SUCCESS == result)
    {
      // bind values
      const List<PObject>& objects = query.values();
      s32 i = 1;
      for (List<PObject>::const_iterator it = objects.begin(); (it != objects.end()) && (EGE_SUCCESS == result); ++it, ++i)
      {
        const PObject object = *it;

        // process accroding to object type
        switch (object->uid())
        {
          case EGE_OBJECT_UID_STRING_BUFFER:

            sqlResult = ::sqlite3_bind_text(statementObject, i, ege_pcast<PStringBuffer>(object)->string().toAscii(), 
                                            static_cast<int>(ege_pcast<PStringBuffer>(object)->string().length()), NULL);
            break;

          case EGE_OBJECT_UID_DATA_BUFFER:

            sqlResult = ::sqlite3_bind_blob(statementObject, i, ege_pcast<PDataBuffer>(object)->data(ege_pcast<PDataBuffer>(object)->readOffset()), 
                                            static_cast<int>(ege_pcast<PDataBuffer>(object)->size() - ege_pcast<PDataBuffer>(object)->readOffset()), NULL);
            break;

          case EGE_OBJECT_UID_INT:

            sqlResult = ::sqlite3_bind_int(statementObject, i, ege_pcast<PInteger>(object)->value());
            break;

          default:

            // error!
            egeWarning(KDatabaseSqliteDebugName) << "Unsupported bind value of type:" << object->uid();
            result = EGE_ERROR_NOT_SUPPORTED;
            break;
        }
      }

      // process statement object
      s32 row = 0;
      while ((SQLITE_DONE != sqlResult) && (EGE_SUCCESS == result))
      {
        // perform process step
        sqlResult = ::sqlite3_step(statementObject);
        switch (sqlResult)
        {
          case SQLITE_ROW:

            // check if columns are not set in resulting objects yet
            if (0 == ege_cast<SqlResultSqlite*>(resultObject)->columnCount())
            {
              // add column names to resulting object
              const int columnCount = sqlite3_column_count(statementObject);
              for (int i = 0; (i < columnCount) && (EGE_SUCCESS == result); ++i)
              {
                result = ege_cast<SqlResultSqlite*>(resultObject)->addColumnName(sqlite3_column_name(statementObject, i));
              }
            }

            if (EGE_SUCCESS == result)
            {
              for (u32 i = 0; (i < ege_cast<SqlResultSqlite*>(resultObject)->columnCount()) && (EGE_SUCCESS == result); ++i)
              {
                int columnType = sqlite3_column_type(statementObject, i);
                switch (columnType)
                {
                  case SQLITE_INTEGER:

                    result = ege_cast<SqlResultSqlite*>(resultObject)->addValue(row, ::sqlite3_column_int(statementObject, i));
                    break;

                  case SQLITE_FLOAT:

                    result = ege_cast<SqlResultSqlite*>(resultObject)->addValue(row, static_cast<float32>(::sqlite3_column_double(statementObject, i)));
                    break;

                  case SQLITE_TEXT:

                    result = ege_cast<SqlResultSqlite*>(resultObject)->addValue(row, reinterpret_cast<const char*>(::sqlite3_column_text(statementObject, i)));
                    break;

                  case SQLITE_BLOB:

                    result = ege_cast<SqlResultSqlite*>(resultObject)->addValue(row, ::sqlite3_column_blob(statementObject, i), 
                                                                                ::sqlite3_column_bytes(statementObject,i));
                    break;

                  default:

                    // error!
                    egeCritical(KDatabaseSqliteDebugName) << "Unknown column type:" << columnType;
                    result = EGE_ERROR_NOT_SUPPORTED;
                    break;
                }
              }
            }
            break;

          case SQLITE_DONE:

            // done
            result = EGE_SUCCESS;
            break;

          default:

            // error!
            result = EGE_ERROR;
            egeWarning(KDatabaseSqliteDebugName) << "Processing of statement object failed! Sqlite error:" << sqlResult;
            break;
        }

        // update row index
        ++row;
      }
    }

    // finalize statement
    sqlResult = ::sqlite3_finalize(statementObject);
    if (SQLITE_OK != sqlResult)
    {
      // error!
      egeWarning(KDatabaseSqliteDebugName) << "Could not finalize statement object! Sqlite error:" << sqlResult;

      result = EGE_ERROR;
    }

    // check if success
    if (EGE_SUCCESS == result)
    {
      // set new result
      m_result = resultObject;
    }
  }

  // check if success
  if (EGE_SUCCESS != result)
  {
    // delete result object
    m_result = NULL;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const PSqlResult DatabaseSqlite::result() const
{
  return m_result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
