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
