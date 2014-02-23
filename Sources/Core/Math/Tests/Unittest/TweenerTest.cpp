#include "TestFramework/Interface/TestBase.h"
#include <math.h>
#include <EGETweener.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class TweenerTest : public TestBase
{
  protected:

    /*! Calculates tween value based on power function eased at the begining.
     *  @param  parameter Parameter in [0-1] range for which tween value is to be calculated.
     *  @param  power     Power function degree.
     *  @return Calculated tween value.
     */
    float32 powerEaseIn(float32 parameter, s32 power) const;
    /*! Calculates tween value based on power function eased at the end.
     *  @param  parameter Parameter in [0-1] range for which tween value is to be calculated.
     *  @param  power     Power function degree.
     *  @return Calculated tween value.
     */
    float32 powerEaseOut(float32 parameter, s32 power) const;
    /*! Calculates tween value based on power function eased at the begining and at the end.
     *  @param  parameter Parameter in [0-1] range for which tween value is to be calculated.
     *  @param  power     Power function degree.
     *  @return Calculated tween value.
     */
    float32 powerEaseInOut(float32 parameter, s32 power) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 TweenerTest::powerEaseIn(float32 parameter, s32 power) const
{
  return powf(parameter, static_cast<float32>(power));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 TweenerTest::powerEaseOut(float32 parameter, s32 power) const
{
  s32 sign = (power & 0x1) ? 1 : -1;

  return sign * (powf(parameter - 1.0f, static_cast<float32>(power)) + sign);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 TweenerTest::powerEaseInOut(float32 parameter, s32 power) const
{
  parameter *= 2;

  if (1.0f > parameter)
  {
    return powerEaseIn(parameter, power) * 0.5f;
  }

  s32 sign = (power & 0x1) ? 1 : -1;

  return sign * 0.5f * (powf(parameter - 2.0f, static_cast<float32>(power)) + sign * 2.0f);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TweenerTest, Linear)
{
  LinearTweener tweener;

  for (float32 t = 0.0f; t <= 1.0f; t += 0.01f)
  {
    EGE_EXPECT_FLOAT_EQ(t, tweener.easeIn(t), epsilon());
    EGE_EXPECT_FLOAT_EQ(t, tweener.easeOut(t), epsilon());
    EGE_EXPECT_FLOAT_EQ(t, tweener.easeInOut(t), epsilon());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TweenerTest, Power)
{
  // perform fixed number of tests
  const s32 KMinPower = 2;  // from quadratic...
  const s32 KMaxPower = 5;  // ...to quintic
  for (int i = KMinPower; i <= KMaxPower; ++i)
  {
    PowerTweener tweener(i);

    for (float32 t = 0.0f; t <= 1.0f; t += 0.01f)
    {
      EGE_EXPECT_FLOAT_EQ(powerEaseIn(t, i), tweener.easeIn(t), epsilon());
      EGE_EXPECT_FLOAT_EQ(powerEaseOut(t, i), tweener.easeOut(t), epsilon());
      EGE_EXPECT_FLOAT_EQ(powerEaseInOut(t, i), tweener.easeInOut(t), epsilon());
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TweenerTest, Sine)
{
  SineTweener tweener;

  for (float32 t = 0.0f; t <= 1.0f; t += 0.01f)
  {
    EGE_EXPECT_FLOAT_EQ(sinf(static_cast<float32>(t * M_PI_2 - M_PI_2)) + 1.0f, tweener.easeIn(t), epsilon());
    EGE_EXPECT_FLOAT_EQ(sinf(static_cast<float32>(t * M_PI_2)), tweener.easeOut(t), epsilon());
    EGE_EXPECT_FLOAT_EQ((sinf(static_cast<float32>(t * M_PI - M_PI_2)) + 1.0f) * 0.5f, tweener.easeInOut(t), epsilon());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
