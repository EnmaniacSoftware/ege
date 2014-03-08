#include "TestFramework/Interface/TestBase.h"
#include <EGEMemory.h>
#include <EGEDebug.h>
#include <EGEDatabase.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KValidDatabasePath           = "Database-test/database.sqlite";
static const char* KValidGeneratedDatabasePath  = "Database-test/generated-database.sqlite";
static const char* KInvalidDatabasePath         = "Z://ala.sqlite";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class DatabaseSqliteTest : public TestBase
{
  protected:

    static void SetUpTestCase();
    static void TearDownTestCase();

  protected:

    virtual void SetUp();
    virtual void TearDown();
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DatabaseSqliteTest::SetUpTestCase()
{
  EXPECT_TRUE(MemoryManager::Initialize());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DatabaseSqliteTest::TearDownTestCase()
{
  MemoryManager::Deinitialize();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DatabaseSqliteTest::SetUp()
{
  // cleanup
  remove(KValidGeneratedDatabasePath);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DatabaseSqliteTest::TearDown()
{
  // cleanup
  remove(KValidGeneratedDatabasePath);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DatabaseSqliteTest, OpenNonExisting)
{
  DatabaseSqlite database;

  EXPECT_EQ(EGE_ERROR_NOT_FOUND, database.open(KInvalidDatabasePath, true, false));
  EXPECT_EQ(EGE_ERROR_NOT_FOUND, database.open(KInvalidDatabasePath, false, false));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DatabaseSqliteTest, OpenExistingReadOnly)
{
  DatabaseSqlite database;

  EXPECT_EQ(EGE_SUCCESS, database.open(KValidDatabasePath, true, false));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DatabaseSqliteTest, OpenExistingReadWrite)
{
  DatabaseSqlite database;

  EXPECT_EQ(EGE_SUCCESS, database.open(KValidDatabasePath, false, false));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DatabaseSqliteTest, CreateInvalidLocation)
{
  DatabaseSqlite database;

  EXPECT_EQ(EGE_ERROR, database.open(KInvalidDatabasePath, true, true));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DatabaseSqliteTest, CreateValidLocation)
{
  DatabaseSqlite database;

  EXPECT_EQ(EGE_ERROR, database.open(KValidGeneratedDatabasePath, true, true));
  EXPECT_EQ(EGE_SUCCESS, database.open(KValidGeneratedDatabasePath, false, true));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DatabaseSqliteTest, SelectAutoTransaction)
{
  DatabaseSqlite database;

  // open database
  EXPECT_EQ(EGE_SUCCESS, database.open(KValidDatabasePath, true, false));

  // execute query
  const SqlQuery query("SELECT * FROM Dictionary");
  EXPECT_EQ(EGE_SUCCESS, database.execute(query));

  // close database
  EXPECT_EQ(EGE_SUCCESS, database.close());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DatabaseSqliteTest, SelectManualTransaction)
{
  DatabaseSqlite database;

  // open database
  EXPECT_EQ(EGE_SUCCESS, database.open(KValidDatabasePath, true, false));

  // start transaction
  EXPECT_EQ(EGE_SUCCESS, database.beginTransaction());

  // execute query
  const SqlQuery query("SELECT * FROM Dictionary");
  EXPECT_EQ(EGE_SUCCESS, database.execute(query));

  // end transaction
  EXPECT_EQ(EGE_SUCCESS, database.endTransaction());

  // close database
  EXPECT_EQ(EGE_SUCCESS, database.close());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DatabaseSqliteTest, ResultByIndex)
{
  DatabaseSqlite database;

  // open database
  EXPECT_EQ(EGE_SUCCESS, database.open(KValidDatabasePath, true, false));

  // execute query
  const SqlQuery query("SELECT * FROM Dictionary");
  EXPECT_EQ(EGE_SUCCESS, database.execute(query));

  // retrieve result object
  PSqlResult result = database.result();
  EXPECT_TRUE(NULL != result);

  // check data
  EXPECT_EQ(3, result->rowCount());

  // row 1...
  PObject value = result->value(0, 0);
  EXPECT_TRUE(NULL != value);
  EXPECT_EQ(EGE_OBJECT_UID_STRING_BUFFER, value->uid());
  EXPECT_STREQ("Key-1", ege_cast<StringBuffer*>(value)->string().toAscii());

  value = result->value(0, 1);
  EXPECT_TRUE(NULL != value);
  EXPECT_EQ(EGE_OBJECT_UID_STRING_BUFFER, value->uid());
  EXPECT_STREQ("This is key-1 value", ege_cast<StringBuffer*>(value)->string().toAscii());

  // row 2...
  value = result->value(1, 0);
  EXPECT_TRUE(NULL != value);
  EXPECT_EQ(EGE_OBJECT_UID_STRING_BUFFER, value->uid());
  EXPECT_STREQ("Key-2", ege_cast<StringBuffer*>(value)->string().toAscii());

  value = result->value(1, 1);
  EXPECT_TRUE(NULL != value);
  EXPECT_EQ(EGE_OBJECT_UID_STRING_BUFFER, value->uid());
  EXPECT_STREQ("This is key-2 value", ege_cast<StringBuffer*>(value)->string().toAscii());

  // row 3...
  value = result->value(2, 0);
  EXPECT_TRUE(NULL != value);
  EXPECT_EQ(EGE_OBJECT_UID_STRING_BUFFER, value->uid());
  EXPECT_STREQ("Key-3", ege_cast<StringBuffer*>(value)->string().toAscii());

  value = result->value(2, 1);
  EXPECT_TRUE(NULL != value);
  EXPECT_EQ(EGE_OBJECT_UID_STRING_BUFFER, value->uid());
  EXPECT_STREQ("This is key-3 value", ege_cast<StringBuffer*>(value)->string().toAscii());

  // row 4...
  value = result->value(3, 0);
  EXPECT_TRUE(NULL == value);

  value = result->value(3, 1);
  EXPECT_TRUE(NULL == value);

  // close database
  EXPECT_EQ(EGE_SUCCESS, database.close());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DatabaseSqliteTest, ResultByName)
{
  DatabaseSqlite database;

  // open database
  EXPECT_EQ(EGE_SUCCESS, database.open(KValidDatabasePath, true, false));

  // execute query
  const SqlQuery query("SELECT * FROM Dictionary");
  EXPECT_EQ(EGE_SUCCESS, database.execute(query));

  // retrieve result object
  PSqlResult result = database.result();
  EXPECT_TRUE(NULL != result);

  // check data
  EXPECT_EQ(3, result->rowCount());

  // row 1...
  PObject value = result->value(0, "key");
  EXPECT_TRUE(NULL != value);
  EXPECT_EQ(EGE_OBJECT_UID_STRING_BUFFER, value->uid());
  EXPECT_STREQ("Key-1", ege_cast<StringBuffer*>(value)->string().toAscii());

  value = result->value(0, "value");
  EXPECT_TRUE(NULL != value);
  EXPECT_EQ(EGE_OBJECT_UID_STRING_BUFFER, value->uid());
  EXPECT_STREQ("This is key-1 value", ege_cast<StringBuffer*>(value)->string().toAscii());

  // row 2...
  value = result->value(1, "key");
  EXPECT_TRUE(NULL != value);
  EXPECT_EQ(EGE_OBJECT_UID_STRING_BUFFER, value->uid());
  EXPECT_STREQ("Key-2", ege_cast<StringBuffer*>(value)->string().toAscii());

  value = result->value(1, "value");
  EXPECT_TRUE(NULL != value);
  EXPECT_EQ(EGE_OBJECT_UID_STRING_BUFFER, value->uid());
  EXPECT_STREQ("This is key-2 value", ege_cast<StringBuffer*>(value)->string().toAscii());

  // row 3...
  value = result->value(2, "key");
  EXPECT_TRUE(NULL != value);
  EXPECT_EQ(EGE_OBJECT_UID_STRING_BUFFER, value->uid());
  EXPECT_STREQ("Key-3", ege_cast<StringBuffer*>(value)->string().toAscii());

  value = result->value(2, "value");
  EXPECT_TRUE(NULL != value);
  EXPECT_EQ(EGE_OBJECT_UID_STRING_BUFFER, value->uid());
  EXPECT_STREQ("This is key-3 value", ege_cast<StringBuffer*>(value)->string().toAscii());

  // row 4...
  value = result->value(3, "key");
  EXPECT_TRUE(NULL == value);

  value = result->value(3, "value");
  EXPECT_TRUE(NULL == value);

  // close database
  EXPECT_EQ(EGE_SUCCESS, database.close());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DatabaseSqliteTest, CreateTable)
{
  DatabaseSqlite database;

  // open database
  EXPECT_EQ(EGE_SUCCESS, database.open(KValidGeneratedDatabasePath, false, true));

  // add table
  EXPECT_EQ(EGE_SUCCESS, database.execute(SqlQuery("CREATE TABLE MyTable (Key varchar(255));")));

  // execute query
  EXPECT_EQ(EGE_SUCCESS, database.execute(SqlQuery("SELECT * FROM MyTable")));

  // retrieve result object
  PSqlResult result = database.result();
  EXPECT_TRUE(NULL != result);

  // check data
  EXPECT_EQ(0, result->rowCount());

  // close database
  EXPECT_EQ(EGE_SUCCESS, database.close());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DatabaseSqliteTest, Insert)
{
  DatabaseSqlite database;

  // open database
  EXPECT_EQ(EGE_SUCCESS, database.open(KValidGeneratedDatabasePath, false, true));

  // add table
  EXPECT_EQ(EGE_SUCCESS, database.execute(SqlQuery("CREATE TABLE MyTable (Key varchar(255));")));

  // insert data
  EXPECT_EQ(EGE_SUCCESS, database.execute(SqlQuery("INSERT INTO MyTable VALUES('Key-1');")));
  EXPECT_EQ(EGE_SUCCESS, database.execute(SqlQuery("INSERT INTO MyTable VALUES('Key-2');")));

  // execute query
  EXPECT_EQ(EGE_SUCCESS, database.execute(SqlQuery("SELECT * FROM MyTable")));

  // retrieve result object
  PSqlResult result = database.result();
  EXPECT_TRUE(NULL != result);

  // check data
  EXPECT_EQ(2, result->rowCount());

  // row 1...
  PObject value = result->value(0, 0);
  EXPECT_TRUE(NULL != value);
  EXPECT_EQ(EGE_OBJECT_UID_STRING_BUFFER, value->uid());
  EXPECT_STREQ("Key-1", ege_cast<StringBuffer*>(value)->string().toAscii());

  value = result->value(1, 0);
  EXPECT_TRUE(NULL != value);
  EXPECT_EQ(EGE_OBJECT_UID_STRING_BUFFER, value->uid());
  EXPECT_STREQ("Key-2", ege_cast<StringBuffer*>(value)->string().toAscii());

  // close database
  EXPECT_EQ(EGE_SUCCESS, database.close());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DatabaseSqliteTest, Rollback)
{
  DatabaseSqlite database;

  PSqlResult result;

  // open database
  EXPECT_EQ(EGE_SUCCESS, database.open(KValidGeneratedDatabasePath, false, true));

  // add table
  EXPECT_EQ(EGE_SUCCESS, database.execute(SqlQuery("CREATE TABLE MyTable (Key varchar(255));")));

  // insert data
  EXPECT_EQ(EGE_SUCCESS, database.execute(SqlQuery("INSERT INTO MyTable VALUES('Key-1');")));

  // check data
  EXPECT_EQ(EGE_SUCCESS, database.execute(SqlQuery("SELECT * FROM MyTable")));
  result = database.result();
  EXPECT_TRUE(NULL != result);
  EXPECT_EQ(1, result->rowCount());

  // begin transaction
  EXPECT_EQ(EGE_SUCCESS, database.beginTransaction());

  // insert more data
  EXPECT_EQ(EGE_SUCCESS, database.execute(SqlQuery("INSERT INTO MyTable VALUES('Key-2');")));

  // check data
  EXPECT_EQ(EGE_SUCCESS, database.execute(SqlQuery("SELECT * FROM MyTable")));
  result = database.result();
  EXPECT_TRUE(NULL != result);
  EXPECT_EQ(2, result->rowCount());

  // abort transaction
  EXPECT_EQ(EGE_SUCCESS, database.abortTransaction());

  // retrieve result object
  EXPECT_EQ(EGE_SUCCESS, database.execute(SqlQuery("SELECT * FROM MyTable")));
  result = database.result();
  EXPECT_TRUE(NULL != result);
  EXPECT_EQ(1, result->rowCount());

  // row 1...
  PObject value = result->value(0, 0);
  EXPECT_TRUE(NULL != value);
  EXPECT_EQ(EGE_OBJECT_UID_STRING_BUFFER, value->uid());
  EXPECT_STREQ("Key-1", ege_cast<StringBuffer*>(value)->string().toAscii());

  // close database
  EXPECT_EQ(EGE_SUCCESS, database.close());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DatabaseSqliteTest, Transaction)
{
  DatabaseSqlite database;

  PSqlResult result;

  // open database
  EXPECT_EQ(EGE_SUCCESS, database.open(KValidGeneratedDatabasePath, false, true));

  // add table
  EXPECT_EQ(EGE_SUCCESS, database.execute(SqlQuery("CREATE TABLE MyTable (Key varchar(255));")));

  // check data
  EXPECT_EQ(EGE_SUCCESS, database.execute(SqlQuery("SELECT * FROM MyTable")));
  result = database.result();
  EXPECT_TRUE(NULL != result);
  EXPECT_EQ(0, result->rowCount());

  // begin transaction
  EXPECT_EQ(EGE_SUCCESS, database.beginTransaction());

  // insert data
  EXPECT_EQ(EGE_SUCCESS, database.execute(SqlQuery("INSERT INTO MyTable VALUES('Key-1');")));
  EXPECT_EQ(EGE_SUCCESS, database.execute(SqlQuery("INSERT INTO MyTable VALUES('Key-2');")));

  // check data
  EXPECT_EQ(EGE_SUCCESS, database.execute(SqlQuery("SELECT * FROM MyTable")));
  result = database.result();
  EXPECT_TRUE(NULL != result);
  EXPECT_EQ(2, result->rowCount());

  // end transaction
  EXPECT_EQ(EGE_SUCCESS, database.endTransaction());

  // check data
  EXPECT_EQ(EGE_SUCCESS, database.execute(SqlQuery("SELECT * FROM MyTable")));
  result = database.result();
  EXPECT_TRUE(NULL != result);
  EXPECT_EQ(2, result->rowCount());

  // close database
  EXPECT_EQ(EGE_SUCCESS, database.close());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
