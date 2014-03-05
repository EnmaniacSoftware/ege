#ifndef EGE_CORE_DATABASE_DATABASESQLITE_H
#define EGE_CORE_DATABASE_DATABASESQLITE_H

/** Sqlite based implementation of databse.
  */

#include "Core/Database/Interface/Database.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
struct sqlite3;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class DatabaseSqlite : public Database
{
  public:

    DatabaseSqlite();
   ~DatabaseSqlite();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! @see Database::open. */
    EGEResult open(const String& path, bool readOnly, bool create) override;
    /*! @see Database::close. */
    EGEResult close() override;

  protected:

    /*! @see Database::beginTransaction. */
    EGEResult beginTransaction() override;
    /*! @see Database::endTransaction. */
    EGEResult endTransaction() override;
    /*! @see Database::abortTransaction. */
    EGEResult abortTransaction() override;

  private:

    /*! Database handler. */
    ::sqlite3* m_handle;
    /*! In transaction flag. */
    bool m_inTransaction;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DATABASE_DATABASESQLITE_H
