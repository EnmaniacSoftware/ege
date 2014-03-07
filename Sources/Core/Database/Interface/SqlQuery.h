#ifndef EGE_CORE_DATABASE_SQLQUERY_H
#define EGE_CORE_DATABASE_SQLQUERY_H

/** This class provides the means of executing and manipulating SQL statements.
  */

#include "EGE.h"
#include "EGEString.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SqlQuery
{
  public:

    SqlQuery(const String& statement);
   ~SqlQuery();

  public:

    const String& value() const;

  private:

    /*! Query. */
    String m_query;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DATABASE_SQLQUERY_H
