#include "TestFramework/Interface/TestBase.h"
#include <EGEAssert.h>
#include <vector>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AssertTest : public TestBase
{
  protected:

    static void SetUpTestCase();
    static void TearDownTestCase();

    /*! Custom simple assertion handler. */
    static void HandleAssert(const char* fileName, s32 lineNo);
    /*! Custom described assertion handler. */
    static void HandleAssertX(const char* reason, const char* fileName, s32 lineNo);

  protected:

    virtual void SetUp();
    virtual void TearDown();

  protected:

    struct HitInfo
    {
      const char* fileName;
      const char* reason;
      s32 lineNo;
    };

    static std::vector<HitInfo> m_hits;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<AssertTest::HitInfo> AssertTest::m_hits;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AssertTest::SetUpTestCase()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AssertTest::TearDownTestCase()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AssertTest::HandleAssert(const char* fileName, s32 lineNo)
{
  HitInfo info;

  info.fileName = fileName;
  info.lineNo   = lineNo;
  info.reason   = NULL;

  m_hits.push_back(info);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AssertTest::HandleAssertX(const char* reason, const char* fileName, s32 lineNo)
{
  HitInfo info;

  info.fileName = fileName;
  info.lineNo   = lineNo;
  info.reason   = reason;

  m_hits.push_back(info);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AssertTest::SetUp()
{
  Assert::InstallHandlers(AssertTest::HandleAssert, AssertTest::HandleAssertX);

  m_hits.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AssertTest::TearDown()
{
  Assert::InstallHandlers(NULL, NULL);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AssertTest, SimpleAssertion_Macro)
{
  EGE_ASSERT(true);
  EXPECT_EQ(0, m_hits.size());

  EGE_ASSERT(false);
#ifdef EGE_FEATURE_DEBUG
  EXPECT_EQ(1, m_hits.size());
  EXPECT_EQ(__FILE__, m_hits[0].fileName);
  EXPECT_EQ(NULL, m_hits[0].reason);
  EXPECT_EQ(__LINE__ - 5, m_hits[0].lineNo);
#else
  EXPECT_EQ(0, m_hits.size());
#endif // EGE_FEATURE_DEBUG
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AssertTest, DescribedAssertion_Macro)
{
  EGE_ASSERT_X(true, "Because I CAN!!!");
  EXPECT_EQ(0, m_hits.size());

  EGE_ASSERT_X(false, "Because I CAN!!!");
#ifdef EGE_FEATURE_DEBUG
  EXPECT_EQ(1, m_hits.size());
  EXPECT_EQ(__FILE__, m_hits[0].fileName);
  EXPECT_STREQ("Because I CAN!!!", m_hits[0].reason);
  EXPECT_EQ(__LINE__ - 5, m_hits[0].lineNo);
#else
  EXPECT_EQ(0, m_hits.size());
#endif // EGE_FEATURE_DEBUG
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AssertTest, SimpleAssertion_Function)
{
  Assert::HandleAssert(__FILE__, __LINE__);
  EXPECT_EQ(1, m_hits.size());
  EXPECT_EQ(__FILE__, m_hits[0].fileName);
  EXPECT_EQ(NULL, m_hits[0].reason);
  EXPECT_EQ(__LINE__ - 4, m_hits[0].lineNo);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AssertTest, DescribedAssertion_Function)
{
  Assert::HandleAssertX("Because I CAN!!!", __FILE__, __LINE__);
  EXPECT_EQ(1, m_hits.size());
  EXPECT_EQ(__FILE__, m_hits[0].fileName);
  EXPECT_STREQ("Because I CAN!!!", m_hits[0].reason);
  EXPECT_EQ(__LINE__ - 4, m_hits[0].lineNo);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
