#include "Core/Database/Implementation/SqlResultSqlite.h"
#include "Core/ComplexTypes.h"
#include "EGEDataBuffer.h"
#include "EGETextStream.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(SqlResultSqlite)
EGE_DEFINE_DELETE_OPERATORS(SqlResultSqlite)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SqlResultSqlite::SqlResultSqlite() 
: SqlResult()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SqlResultSqlite::~SqlResultSqlite()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PObject SqlResultSqlite::value(s32 rowIndex, const String& name) const
{
  EGE_ASSERT(0 <= rowIndex);

  PObject object;

  // determine column index
  const s32 columnIndex = m_columnNames.indexOf(name);
  if (0 <= columnIndex)
  {
    // retrive object
    object = m_rows.value(rowIndex, DynamicArray<PObject>()).at(columnIndex, NULL);
  }

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PObject SqlResultSqlite::value(s32 rowIndex, s32 index) const
{
  EGE_ASSERT(0 <= rowIndex);
  EGE_ASSERT(0 <= index);

  PObject object;

  // retrive object
  object = m_rows.value(rowIndex, DynamicArray<PObject>()).at(index, NULL);

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 SqlResultSqlite::rowCount() const
{
  return static_cast<u32>(m_rows.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SqlResultSqlite::addColumnName(const String& name)
{
  m_columnNames.append(name);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 SqlResultSqlite::columnCount() const
{
  return static_cast<u32>(m_columnNames.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SqlResultSqlite::addValue(s32 index, const char* value)
{
  EGEResult result = EGE_SUCCESS;

  // create value
  PStringBuffer valueObject = ege_new StringBuffer();
  if (NULL == valueObject)
  {
    // error!
    result = EGE_ERROR_NO_MEMORY;
  }
  else
  {
    TextStream stream(valueObject);

    // add value to value object
    stream << value;

    // add it to pool
    m_rows[index].append(valueObject);
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SqlResultSqlite::addValue(s32 index, s32 value)
{
  EGEResult result = EGE_SUCCESS;

  // create value
  PInteger valueObject = ege_new Integer(value);
  if (NULL == valueObject)
  {
    // error!
    result = EGE_ERROR_NO_MEMORY;
  }
  else
  {
    // add it to pool
    m_rows[index].append(valueObject);
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SqlResultSqlite::addValue(s32 index, float32 value)
{
  EGEResult result = EGE_SUCCESS;

  // create value
  PFloat valueObject = ege_new Float(value);
  if (NULL == valueObject)
  {
    // error!
    result = EGE_ERROR_NO_MEMORY;
  }
  else
  {
    // add it to pool
    m_rows[index].append(valueObject);
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SqlResultSqlite::addValue(s32 index, const void* buffer, s32 size)
{
  EGEResult result = EGE_SUCCESS;

  // create value
  PDataBuffer valueObject = ege_new DataBuffer(size);
  if (NULL == valueObject)
  {
    // error!
    result = EGE_ERROR_NO_MEMORY;
  }
  else
  {
    // write data to buffer
    if (size != valueObject->write(buffer, size))
    {
      // error!
      result = EGE_ERROR;
    }
    else
    {
      // add it to pool
      m_rows[index].append(valueObject);
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END