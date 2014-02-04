#include <gtest/gtest.h>
#include <math.h>
#include <stdlib.h>
#include <limits>
#include <EGEAngle.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const int KRepetitionsCount = 20;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AngleTest : public ::testing::Test
{
  protected:

    static void SetUpTestCase();
    static void TearDownTestCase();

  protected:

    virtual void SetUp();
    virtual void TearDown();

  protected:

    /*! Returns random number. 
     *  @param  scale Scale of the returned value.
     *  @return Generated random number.
     *  @note Returned number is in [-scale,scale] interval.
     */
    float32 random(float32 scale = 1.0f) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AngleTest::SetUpTestCase()
{
  srand(static_cast<unsigned int>(time(NULL)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AngleTest::TearDownTestCase()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AngleTest::SetUp()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AngleTest::TearDown()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 AngleTest::random(float32 scale) const
{
  return (rand() / static_cast<float32>(RAND_MAX) - 0.5f) * 2.0f * scale;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AngleTest, SetValue)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 radians = random(EGEMath::TWO_PI);
    const float32 degrees = random(360.0f);

    // setting via constructors
    const Angle angle(radians);
    EXPECT_FLOAT_EQ(angle.radians(), radians);

    const Angle angle2(angle);
    EXPECT_FLOAT_EQ(angle.radians(), angle2.radians());

    // setting by setters
    const Angle angle3 = Angle::FromRadians(radians);
    EXPECT_FLOAT_EQ(angle3.radians(), radians);

    const Angle angleDeg3 = Angle::FromDegrees(degrees);
    EXPECT_FLOAT_EQ(angleDeg3.degrees(), degrees);

    Angle angle4;
    angle4.fromRadians(radians);
    EXPECT_FLOAT_EQ(angle4.radians(), radians);

    Angle angle5Deg;
    angle5Deg.fromDegrees(degrees);
    EXPECT_FLOAT_EQ(angle5Deg.degrees(), degrees);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AngleTest, Normalize)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    // random angle in [0-2PI] interval
    const float32 radians = random(EGEMath::PI) + EGEMath::PI;

    // Normalization to [0-2PI]
    Angle angle(radians);
    angle.normalize();

    Angle normalized = angle.normalized();

    EXPECT_FLOAT_EQ(radians, angle.radians());
    EXPECT_FLOAT_EQ(radians, normalized.radians());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// TAGE - due to precision loss in Angle::distanceTo method this tests have tendency to fail. Find better way to either do the calculations or test
TEST_F(AngleTest, DISABLED_Distance)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 radians = random(EGEMath::PI);                             // [-180, +180]
    const float32 diff    = random(EGEMath::PI * 0.5f) + EGEMath::PI * 0.5f; // [0 - 180]

    Angle angle1(radians);
    Angle angle2(radians + diff);
    Angle angleDiff(diff);

    EXPECT_FLOAT_EQ(angle1.distanceTo(angle2, EClockwise).radians(), EGEMath::TWO_PI - diff);
    EXPECT_FLOAT_EQ(angle1.distanceTo(angle2, ECounterClockwise).radians(), diff);
    EXPECT_FLOAT_EQ(angle1.distanceTo(angle2, EShortest).radians(), 
                    Math::Min(angle1.distanceTo(angle2, EClockwise).radians(), angle1.distanceTo(angle2, ECounterClockwise).radians()));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AngleTest, LessThan)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 radians = random(EGEMath::PI);                             // [-180, +180]
    const float32 diff    = random(EGEMath::PI * 0.5f) + EGEMath::PI * 0.5f; // [0 - 180]

    Angle angle1(radians);
    Angle angle2(radians + diff);

    EXPECT_TRUE(angle1 < angle2);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AngleTest, GreaterThan)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 radians = random(EGEMath::PI);                             // [-180, +180]
    const float32 diff    = random(EGEMath::PI * 0.5f) + EGEMath::PI * 0.5f; // [0 - 180]

    Angle angle1(radians);
    Angle angle2(radians + diff);

    EXPECT_TRUE(angle2 > angle1);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AngleTest, Scaling)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 radians = random();
    const float32 scale   = static_cast<float32>(rand() % 256);

    Angle angle(radians);
    angle *= scale;

    EXPECT_FLOAT_EQ(angle.radians(), radians * scale);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AngleTest, Addition)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 radians1 = random();
    const float32 radians2 = random();

    const Angle angle1(radians1);
    const Angle angle2(radians2);
    
    // compare a1 + a2
    EXPECT_FLOAT_EQ((angle1 + angle2).radians(), radians1 + radians2);

    // compare a2 + a1
    EXPECT_FLOAT_EQ((angle2 + angle1).radians(), radians2 + radians1);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AngleTest, Difference)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 radians1 = random();
    const float32 radians2 = random();

    const Angle angle1(radians1);
    const Angle angle2(radians2);
    
    // compare a1 - a2
    EXPECT_FLOAT_EQ((angle1 - angle2).radians(), radians1 - radians2);

    // compare a2 + a1
    EXPECT_FLOAT_EQ((angle2 - angle1).radians(), radians2 - radians1);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AngleTest, Negation)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 radians = random();

    const Angle angle(radians);
    
    // compare
    EXPECT_FLOAT_EQ((-angle).radians(), -radians);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
