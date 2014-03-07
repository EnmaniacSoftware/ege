#ifndef EGE_CORE_DATABASE_SQLRESULTSQLITE_H
#define EGE_CORE_DATABASE_SQLRESULTSQLITE_H

/** SQL result object for SQLite database.
  * This object has fully public interface so DatabaseSqlite can modify its state. However, this object is never passed to client as is. It is only done via
  * SqlResult interface.
  */

#include "EGE.h"
#include "EGEString.h"
#include "EGEMap.h"
#include "EGEDynamicArray.h"
#include "Core/Database/Interface/SqlResult.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SqlResultSqlite : public SqlResult
{
  public:

    SqlResultSqlite();
   ~SqlResultSqlite();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! @see SqlResult::value. */
    PObject value(s32 rowIndex, const String& name) const override;
    /*! @see SqlResult::value. */
    PObject value(s32 rowIndex, s32 index) const override;
    /*! @see SqlResult::rowCount. */
    u32 rowCount() const override;

    /*! Adds column name. 
     *  @param  name  Name of the column.
     *  @return EGE_SUCCESS on success.
     */
    EGEResult addColumnName(const String& name);
    /*! Returns number of columns. */
    u32 columnCount() const;

    /*! Adds string value for a given row. 
     *  @param  index Row index for which value is being added.
     *  @param  value Value to add to the pool of values already assigned for the row.
     *  @return EGE_SUCCESS on success.
     */
    EGEResult addValue(s32 index, const char* value);
    /*! Adds integer value for a given row. 
     *  @param  index Row index for which value is being added.
     *  @param  value Value to add to the pool of values already assigned for the row.
     *  @return EGE_SUCCESS on success.
     */
    EGEResult addValue(s32 index, s32 value);
    /*! Adds float value for a given row. 
     *  @param  index Row index for which value is being added.
     *  @param  value Value to add to the pool of values already assigned for the row.
     *  @return EGE_SUCCESS on success.
     */
    EGEResult addValue(s32 index, float32 value);

  private:

    /*! Names of columns. */
    StringArray m_columnNames;
    /*! Map of rows data. Key = row, value = column values. */
    Map<s32, DynamicArray<PObject> > m_rows;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DATABASE_SQLRESULTSQLITE_H
