#ifndef EGE_CORE_DATABASE_DATABASE_H
#define EGE_CORE_DATABASE_DATABASE_H

/** This is base class for all kinds of transactional database backends.
  */

#include "EGE.h"
#include "EGEString.h"
#include "Core/Database/Interface/SqlQuery.h"
#include "Core/Database/Interface/SqlResult.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Database, PDatabase)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Database : public Object
{
  public:

    Database();
    virtual ~Database();

  public:

    /*! Opens database at given location.
     *  @param  path      Location of database to open.
     *  @param  readOnly  If set, database is open in read-only mode. Otherwise, in read-write mode.
     *  @param  create    If set, database is created.
     *  @return EGE_SUCCESS on success. Otherwise, another error code.
     *  @note If create is not set database must exists. Otherwise, it is not allowed for database to exists.
     */
    virtual EGEResult open(const String& path, bool readOnly, bool create) = 0;
    /*! Closes the database.
     *  @return EGE_SUCCESS if database was successfully closed (and was previously opened). Closing of unopened database is an error.
     */
    virtual EGEResult close() = 0;
    /*! Starts transaction.
     *  @return EGE_SUCCESS on success. Otherwise, another error code. 
     */
    virtual EGEResult beginTransaction()= 0;
    /*! Ends (commits) transaction.
     *  @return EGE_SUCCESS on success. Otherwise, another error code. 
     */
    virtual EGEResult endTransaction() = 0;
    /*! Aborts (rolls back) the transaction.
     *  @return EGE_SUCCESS on success. Otherwise, another error code. 
     */
    virtual EGEResult abortTransaction() = 0;
    /*! Executes given statement.
     *  @param  query Statement to execute.
     *  @return EGE_SUCCESS on success. Otherwise, another error code.
     *  @note Statement is executed as a part of current transaction.
     */
    virtual EGEResult execute(const SqlQuery& query) = 0;
    /*! Returns pointer to result object.
     *  @return Object containing result of last database operation. Can be NULL.
     */
    virtual const PSqlResult result() const = 0;

  private:

    Database(const Database& other);
    Database& operator = (Database& other);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DATABASE_DATABASE_H
