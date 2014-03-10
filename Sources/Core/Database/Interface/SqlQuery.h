#ifndef EGE_CORE_DATABASE_SQLQUERY_H
#define EGE_CORE_DATABASE_SQLQUERY_H

/** This class provides the means of executing and manipulating SQL statements.
  */

#include "EGE.h"
#include "EGEString.h"
#include "EGEDataBuffer.h"
#include "EGEList.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SqlQuery
{
  public:

    SqlQuery(const String& statement);
   ~SqlQuery();

  public:

    /*! Returns current query string. */
    const String& value() const;
    /*! Binds given value to next available position in query. 
     *  @param  value Value to be bound.
     *  @return TRUE if action succeeded.
     *  @note Passed objects needs to be alive until exection of the query.
     */
    bool addBindValue(const String& value);
    bool addBindValue(PDataBuffer value);
    bool addBindValue(s32 value);

    /*! Returns the list of bound objects. */
    const List<PObject>& values() const;

  private:

    /*! Query. */
    String m_query;
    /*! List of bound values. */
    List<PObject> m_boundValues;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DATABASE_SQLQUERY_H
