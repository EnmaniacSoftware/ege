#include "TestFramework/Interface/TestBase.h"
#include <EGEAssert.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AssertTest : public TestBase
{
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AssertTest, SimpleAssertion_Macro)
{
  // NOTE: since macros are included here and are not a part of EGE on their own, they work regular way, ie. are disabled in Release and enabled in Debug builds

  EGE_ASSERT(true);
  EXPECT_EQ(0, assertHitInfo().size());

  EGE_ASSERT(false);
#ifdef EGE_FEATURE_DEBUG
  EXPECT_EQ(1, assertHitInfo().size());
  EXPECT_EQ(__FILE__, assertHitInfo()[0].fileName);
  EXPECT_EQ(NULL, assertHitInfo()[0].reason);
  EXPECT_EQ(__LINE__ - 5, assertHitInfo()[0].lineNo);
#else
  EXPECT_EQ(0, assertHitInfo().size());
#endif // EGE_FEATURE_DEBUG
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AssertTest, DescribedAssertion_Macro)
{
  // NOTE: since macros are included here and are not a part of EGE on their own, they work regular way, ie. are disabled in Release and enabled in Debug builds

  EGE_ASSERT_X(true, "Because I CAN!!!");
  EXPECT_EQ(0, assertHitInfo().size());

  EGE_ASSERT_X(false, "Because I CAN!!!");
#ifdef EGE_FEATURE_DEBUG
  EXPECT_EQ(1, assertHitInfo().size());
  EXPECT_EQ(__FILE__, assertHitInfo()[0].fileName);
  EXPECT_STREQ("Because I CAN!!!", assertHitInfo()[0].reason);
  EXPECT_EQ(__LINE__ - 5, assertHitInfo()[0].lineNo);
#else
  EXPECT_EQ(0, assertHitInfo().size());
#endif // EGE_FEATURE_DEBUG
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AssertTest, SimpleAssertion_Function)
{
  Assert::HandleAssert(__FILE__, __LINE__);
  EXPECT_EQ(1, assertHitInfo().size());
  EXPECT_EQ(__FILE__, assertHitInfo()[0].fileName);
  EXPECT_EQ(NULL, assertHitInfo()[0].reason);
  EXPECT_EQ(__LINE__ - 4, assertHitInfo()[0].lineNo);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AssertTest, DescribedAssertion_Function)
{
  Assert::HandleAssertX("Because I CAN!!!", __FILE__, __LINE__);
  EXPECT_EQ(1, assertHitInfo().size());
  EXPECT_EQ(__FILE__, assertHitInfo()[0].fileName);
  EXPECT_STREQ("Because I CAN!!!", assertHitInfo()[0].reason);
  EXPECT_EQ(__LINE__ - 4, assertHitInfo()[0].lineNo);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
