#ifndef EGE_CORE_DATABASE_SQLRESULT_H
#define EGE_CORE_DATABASE_SQLRESULT_H

/** Base class for SQL result object.
  * Such object is usually obtainable through database.
  */

#include "EGE.h"
#include "EGEString.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(SqlResult, PSqlResult)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SqlResult : public Object
{
  public:

    SqlResult();
    virtual ~SqlResult();

  public:

    /*! Returns object with a given name at given record. 
     *  @param  rowIndex  Zero based index of the row from which data is requested.
     *  @param  name      Name of the object for which data is to be retrieved.
     *  @return Generic object which caller needs to interpret. NULL if no object with such name has been found.
     */
    virtual PObject value(s32 rowIndex, const String& name) const = 0;
    /*! Returns object with given index at current record. 
     *  @param  rowIndex  Zero based index of the row from which data is requested.
     *  @param  index     Zero based index of the object for which data is to be retrieved.
     *  @return Generic object which caller needs to interpret. NULL if no object at given index has been found.
     */
    virtual PObject value(s32 rowIndex, s32 index) const = 0;
    /*! Returns number rows. */
    virtual u32 rowCount() const = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DATABASE_SQLRESULT_H
