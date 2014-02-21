#include "TestFramework/Interface/TestBase.h"
#include <EGEAngle.h>
#include <EGEMath.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const int KRepetitionsCount = 20;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AngleTest : public TestBase
{
  protected:

    AngleTest();
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AngleTest::AngleTest() : TestBase(0.0001f)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AngleTest, SetValue)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 radians = random(Math::TWO_PI);
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
    const float32 radians = randomAngle();

    // Normalization to [0-2PI]
    Angle angle(radians);
    angle.normalize(Math::PI);

    Angle normalized = angle.normalized();

    EXPECT_FLOAT_EQ(radians, angle.radians());
    EXPECT_FLOAT_EQ(radians, normalized.radians());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AngleTest, Distance)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 radians = random(Math::PI);                          // [-180, +180]
    const float32 diff    = random(Math::PI * 0.5f) + Math::PI * 0.5f; // [0 - 180]

    Angle angle1(radians);
    Angle angle2(radians + diff);
    Angle angleDiff(diff);

    EGE_EXPECT_FLOAT_EQ(angle1.distanceTo(angle2, EClockwise).radians(), Math::TWO_PI - diff, epsilon());
    EGE_EXPECT_FLOAT_EQ(angle1.distanceTo(angle2, ECounterClockwise).radians(), diff, epsilon());
    EGE_EXPECT_FLOAT_EQ(angle1.distanceTo(angle2, EShortest).radians(), 
                        Math::Min(angle1.distanceTo(angle2, EClockwise).radians(), angle1.distanceTo(angle2, ECounterClockwise).radians()), epsilon());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AngleTest, LessThan)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 radians = random(Math::PI);                          // [-180, +180]
    const float32 diff    = random(Math::PI * 0.5f) + Math::PI * 0.5f; // [0 - 180]

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
    const float32 radians = random(Math::PI);                          // [-180, +180]
    const float32 diff    = random(Math::PI * 0.5f) + Math::PI * 0.5f; // [0 - 180]

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
TEST_F(AngleTest, Equality)
{
  // same (both normalized)
  EXPECT_TRUE(Angle(Math::PI) == Angle(Math::PI));
  EXPECT_FALSE(Angle(Math::PI) != Angle(Math::PI));

  // same (one not normalized)
  EXPECT_TRUE(Angle(Math::PI) == Angle(Math::PI + Math::TWO_PI));
  EXPECT_FALSE(Angle(Math::PI) != Angle(Math::PI + Math::TWO_PI));
  EXPECT_TRUE(Angle(Math::PI) == Angle(Math::PI - Math::TWO_PI));
  EXPECT_FALSE(Angle(Math::PI) != Angle(Math::PI - Math::TWO_PI));

  // different
  EXPECT_TRUE(Angle(Math::PI) != Angle(Math::PI + 0.001f));
  EXPECT_FALSE(Angle(Math::PI) == Angle(Math::PI + 0.001f));
  EXPECT_TRUE(Angle(Math::PI) != Angle(Math::PI - 0.001f));
  EXPECT_FALSE(Angle(Math::PI) == Angle(Math::PI - 0.001f));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
