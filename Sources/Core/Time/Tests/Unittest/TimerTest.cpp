#include "TestFramework/Interface/TestBase.h"
#include <EGETimer.h>
#include <windows.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class TimerTest : public TestBase
{
  protected:

    static void SetUpTestCase();
    static void TearDownTestCase();

  protected:

    virtual void SetUp();
    virtual void TearDown();

  protected:

    /*! Methods sleeping for a given interval. 
     *  @param  miliseconds Sleep interval.
     */
    void osSleep(u32 miliseconds);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimerTest::SetUpTestCase()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimerTest::TearDownTestCase()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimerTest::SetUp()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimerTest::TearDown()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimerTest::osSleep(u32 miliseconds)
{
  Sleep(miliseconds);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TimerTest, GetMiliseconds)
{
  const u32 KSleepIntervalMs          = 100;
  const u32 KSleepIntervalMsDeviation = 10;

  // register time stamp
  s64 stamp = Timer::GetMiliseconds();

  // sleep using OS functionality
  osSleep(KSleepIntervalMs);

  // calculate time difference
  stamp = Timer::GetMiliseconds() - stamp;

  // validate
  EXPECT_TRUE(((KSleepIntervalMs - KSleepIntervalMsDeviation) <= stamp) && ((KSleepIntervalMs + KSleepIntervalMsDeviation) >= stamp)) 
    << "Difference: " << stamp;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TimerTest, GetMicroseconds)
{
  const u32 KSleepIntervalMs          = 100;
  const u32 KSleepIntervalMsDeviation = 10;

  // register time stamp
  s64 stamp = Timer::GetMicroseconds();

  // sleep using OS functionality
  osSleep(KSleepIntervalMs);

  // calculate time difference
  stamp = Timer::GetMicroseconds() - stamp;

  // validate
  EXPECT_TRUE((((KSleepIntervalMs - KSleepIntervalMsDeviation) * 1000) <= stamp) && (((KSleepIntervalMs + KSleepIntervalMsDeviation) * 1000) >= stamp)) 
    << "Difference: " << stamp;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
