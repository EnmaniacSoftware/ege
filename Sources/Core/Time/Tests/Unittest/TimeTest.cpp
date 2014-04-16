#include "TestFramework/Interface/TestBase.h"
#include <EGETime.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class TimeTest : public TestBase
{
  protected:

    static void SetUpTestCase();
    static void TearDownTestCase();

  protected:

    virtual void SetUp();
    virtual void TearDown();
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeTest::SetUpTestCase()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeTest::TearDownTestCase()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeTest::SetUp()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeTest::TearDown()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TimeTest, Construct)
{
  const s64 KTestMicroseconds = 123467890LL;
  const s64 KTestMiliseconds  = 1234560;
  const float32 KTestSeconds  = 30.125f;

  // constructors
  Time time1(KTestMicroseconds);
  EXPECT_EQ(KTestMicroseconds, time1.microseconds());

  Time time2(KTestSeconds);
  EXPECT_FLOAT_EQ(KTestSeconds, time2.seconds());

  Time time3(time1);
  EXPECT_EQ(time1.microseconds(), time3.microseconds());

  // operators
  Time time;
  
  time = KTestSeconds;
  EXPECT_FLOAT_EQ(KTestSeconds, time.seconds());

  time = KTestMicroseconds;
  EXPECT_EQ(KTestMicroseconds, time.microseconds());

  time = time1;
  EXPECT_EQ(time1.microseconds(), time.microseconds());

  // setters
  time.fromMiliseconds(KTestMiliseconds);
  EXPECT_EQ(KTestMiliseconds, time.miliseconds());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TimeTest, Greater)
{
  const s64 KTestMicroseconds    = 1020304050LL;
  const s64 KGreaterMicroseconds = KTestMicroseconds + 1LL;

  const Time time(KTestMicroseconds);
  const Time greaterTime(KGreaterMicroseconds);

  EXPECT_TRUE(greaterTime > time);
  EXPECT_TRUE(greaterTime >= time);
  EXPECT_FALSE(time > greaterTime);
  EXPECT_FALSE(time >= greaterTime);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TimeTest, Lesser)
{
  const s64 KTestMicroseconds    = 1020304050LL;
  const s64 KLesserMicroseconds  = KTestMicroseconds - 1LL;

  const Time time(KTestMicroseconds);
  const Time lesserTime(KLesserMicroseconds);

  EXPECT_TRUE(lesserTime < time);
  EXPECT_TRUE(lesserTime <= time);
  EXPECT_FALSE(time < lesserTime);
  EXPECT_FALSE(time <= lesserTime);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TimeTest, Equality)
{
  const s64 KTestMicroseconds1  = 1020304050LL;
  const s64 KTestMicroseconds2  = 0102030405LL;

  const Time time1(KTestMicroseconds1);
  const Time anotherTime1(KTestMicroseconds1);
  const Time time2(KTestMicroseconds2);

  EXPECT_FALSE(time1 == time2);
  EXPECT_FALSE(time2 == time1);

  EXPECT_TRUE(time1 == anotherTime1);
  EXPECT_TRUE(time1 == time1);
  EXPECT_TRUE(anotherTime1 == time1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TimeTest, NonEquality)
{
  const s64 KTestMicroseconds1  = 1020304050LL;
  const s64 KTestMicroseconds2  = 0102030405LL;

  const Time time1(KTestMicroseconds1);
  const Time anotherTime1(KTestMicroseconds1);
  const Time time2(KTestMicroseconds2);

  EXPECT_TRUE(time1 != time2);
  EXPECT_TRUE(time2 != time1);

  EXPECT_FALSE(time1 != anotherTime1);
  EXPECT_FALSE(time1 != time1);
  EXPECT_FALSE(anotherTime1 != time1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TimeTest, Addition)
{
  const s64 KTestMicroseconds1  = 1020304050LL;
  const s64 KTestMicroseconds2  = 0102030405LL;
  const s64 KTestSum            = KTestMicroseconds1 + KTestMicroseconds2;

  const Time time1(KTestMicroseconds1);
  const Time time2(KTestMicroseconds2);

  Time time3 = time1 + time2;
  EXPECT_EQ(KTestSum, time3.microseconds());

  time3 = time1;
  time3 += time2;
  EXPECT_EQ(KTestSum, time3.microseconds());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TimeTest, Subtraction)
{
  const s64 KTestMicroseconds1  = 1020304050LL;
  const s64 KTestMicroseconds2  = 0102030405LL;
  const s64 KTestDifference     = KTestMicroseconds1 - KTestMicroseconds2;

  const Time time1(KTestMicroseconds1);
  const Time time2(KTestMicroseconds2);

  Time time3 = time1 - time2;
  EXPECT_EQ(KTestDifference, time3.microseconds());

  time3 = time1;
  time3 -= time2;
  EXPECT_EQ(KTestDifference, time3.microseconds());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TimeTest, Division)
{
  const s64 KTestMicroseconds  = 100500LL;
  const s32 KTestDivision      = 2;
  const s64 KTestAfterDivision = KTestMicroseconds / KTestDivision;

  const Time time(KTestMicroseconds);

  Time time2 = time / static_cast<float32>(KTestDivision);
  EXPECT_EQ(KTestAfterDivision, time2.microseconds());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TimeTest, Multiplication)
{
  const s64 KTestMicroseconds         = 100500LL;
  const s32 KTestMultiplier           = 4;
  const s64 KTestAfterMultiplication  = KTestMicroseconds * KTestMultiplier;

  const Time time(KTestMicroseconds);

  Time time2 = time * static_cast<float32>(KTestMultiplier);
  EXPECT_EQ(KTestAfterMultiplication, time2.microseconds());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TimeTest, Conversion)
{
  const s64 KTestBaseMicroseconds = 1000LL * 1000LL * 100LL;                  // 100 secs
  const s64 KTestMiliseconds      = KTestBaseMicroseconds / 1000;             
  const float32 KTestSeconds      = KTestBaseMicroseconds / 1000000.0f;          
  const float32 KTestMinutes      = KTestBaseMicroseconds / (1000000 * 60.0f);   

  const Time time(KTestBaseMicroseconds);

  EXPECT_EQ(KTestBaseMicroseconds, time.microseconds());
  EXPECT_EQ(KTestMiliseconds, time.miliseconds());
  EXPECT_FLOAT_EQ(KTestSeconds, time.seconds());
  EXPECT_FLOAT_EQ(KTestMinutes, time.minutes());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
