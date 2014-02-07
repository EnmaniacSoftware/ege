#include <gtest/gtest.h>
#include <math.h>
#include <stdlib.h>
#include <EGEPlane.h>

/** Tests are focusing TPlane<float32> instantiations. */

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const int KRepetitionsCount = 20;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PlaneTest : public ::testing::Test
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
    /*! Normalizes given plane data.
     *  @param  x               Plane normal X value.
     *  @param  y               Plane normal Y value.
     *  @param  z               Plane normal Z value.
     *  @param  w               Plane normal W value.
     *  @param  displacemnt     Plane displacement.
     *  @param  outX            Normalized plane normal X value.
     *  @param  outY            Normalized plane normal Y value.
     *  @param  outZ            Normalized plane normal Z value.
     *  @param  outW            Normalized plane normal W value.
     *  @param  outDisplacemnt  Normalized plane displacement.
     */
    void normalize(float32 x, float32 y, float32 z, float32 w, float32 displacement, 
                   float32& outX, float32& outY, float32& outZ, float32& outW, float32& outDisplacement) const;
    /*! Returns distance of a given point to plane.
     *  @param  x               Plane normal X value.
     *  @param  y               Plane normal Y value.
     *  @param  z               Plane normal Z value.
     *  @param  w               Plane normal W value.
     *  @param  displacemnt     Plane displacement.
     *  @param  pointX          Point X value.
     *  @param  pointY          Point Y value.
     *  @param  pointZ          Point Z value.
     *  @param  pointW          Point W value.
     *  @return Calculated distance.
     */
    float32 distance(float32 x, float32 y, float32 z, float32 w, float32 displacement, float32 pointX, float32 pointY, float32 pointZ, float32 pointW) const;
    /*! Returns the side of the plane on which given point exists
     *  @param  x               Plane normal X value.
     *  @param  y               Plane normal Y value.
     *  @param  z               Plane normal Z value.
     *  @param  w               Plane normal W value.
     *  @param  displacemnt     Plane displacement.
     *  @param  pointX          Point X value.
     *  @param  pointY          Point Y value.
     *  @param  pointZ          Point Z value.
     *  @param  pointW          Point W value.
     *  @return Side of the plane given point is lying.
     */
    PlaneSide side(float32 x, float32 y, float32 z, float32 w, float32 displacement, float32 pointX, float32 pointY, float32 pointZ, float32 pointW) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PlaneTest::SetUpTestCase()
{
  srand(static_cast<unsigned int>(time(NULL)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PlaneTest::TearDownTestCase()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PlaneTest::SetUp()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PlaneTest::TearDown()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 PlaneTest::random(float32 scale) const
{
  return (rand() / static_cast<float32>(RAND_MAX) - 0.5f) * 2.0f * scale;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PlaneTest::normalize(float32 x, float32 y, float32 z, float32 w, float32 displacement, 
                          float32& outX, float32& outY, float32& outZ, float32& outW, float32& outDisplacement) const
{
  outX            = x;
  outY            = y;
  outZ            = z;
  outW            = w;
  outDisplacement = displacement;

  float32 length = sqrtf((x * x) + (y * y) + (z * z) + (w * w));
  if (std::numeric_limits<float32>::epsilon() <= length)
  {
    outX /= length;
    outY /= length;
    outZ /= length;
    outW = 1.0f;

    outDisplacement /= length;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 PlaneTest::distance(float32 x, float32 y, float32 z, float32 w, float32 displacement, 
                            float32 pointX, float32 pointY, float32 pointZ, float32 pointW) const
{
   return (x * pointX) + (y * pointY) + (z * pointZ) + (w * pointW) + displacement;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PlaneSide PlaneTest::side(float32 x, float32 y, float32 z, float32 w, float32 displacement, 
                          float32 pointX, float32 pointY, float32 pointZ, float32 pointW) const
{
  const float32 pointDistance = distance(x, y, z, w, displacement, pointX, pointY, pointZ, pointW);

  if (0 < pointDistance)
  {
    return EPositive;
  }
  else if (0 > pointDistance)
  {
    return ENegative;
  }

  return EBoth;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(PlaneTest, SetValue)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 normalX       = random();
    const float32 normalY       = random();
    const float32 normalZ       = random();
    const float32 normalW       = random();
    const float32 displacement  = random();

    // setting via constructors
    const Planef plane1(Vector4f(normalX, normalY, normalZ, normalW), displacement);
    
    EXPECT_FLOAT_EQ(plane1.m_normal.x, normalX);
    EXPECT_FLOAT_EQ(plane1.m_normal.y, normalY);
    EXPECT_FLOAT_EQ(plane1.m_normal.z, normalZ);
    EXPECT_FLOAT_EQ(plane1.m_normal.w, normalW);
    EXPECT_FLOAT_EQ(plane1.m_d, displacement);

    const Planef plane2(plane1);
    
    EXPECT_FLOAT_EQ(plane2.m_normal.x, normalX);
    EXPECT_FLOAT_EQ(plane2.m_normal.y, normalY);
    EXPECT_FLOAT_EQ(plane2.m_normal.z, normalZ);
    EXPECT_FLOAT_EQ(plane2.m_normal.w, normalW);
    EXPECT_FLOAT_EQ(plane2.m_d, displacement);

    // setting by setters
    Planef plane3;
    plane3.create(Vector4f(normalX, normalY, normalZ, normalW), displacement);
    
    EXPECT_FLOAT_EQ(plane3.m_normal.x, normalX);
    EXPECT_FLOAT_EQ(plane3.m_normal.y, normalY);
    EXPECT_FLOAT_EQ(plane3.m_normal.z, normalZ);
    EXPECT_FLOAT_EQ(plane3.m_normal.w, normalW);
    EXPECT_FLOAT_EQ(plane3.m_d, displacement);

    Planef plane4;
    plane4.create(plane3);

    EXPECT_FLOAT_EQ(plane4.m_normal.x, normalX);
    EXPECT_FLOAT_EQ(plane4.m_normal.y, normalY);
    EXPECT_FLOAT_EQ(plane4.m_normal.z, normalZ);
    EXPECT_FLOAT_EQ(plane4.m_normal.w, normalW);
    EXPECT_FLOAT_EQ(plane4.m_d, displacement);

    // setting via operators
    Planef plane5;
    plane5 = plane4;

    EXPECT_FLOAT_EQ(plane5.m_normal.x, normalX);
    EXPECT_FLOAT_EQ(plane5.m_normal.y, normalY);
    EXPECT_FLOAT_EQ(plane5.m_normal.z, normalZ);
    EXPECT_FLOAT_EQ(plane5.m_normal.w, normalW);
    EXPECT_FLOAT_EQ(plane5.m_d, displacement);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// TAGE - once TVector4 is refactored this should be enabled again
TEST_F(PlaneTest, DISABLED_Normalize)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 normalX       = random();
    const float32 normalY       = random();
    const float32 normalZ       = random();
    const float32 normalW       = random();
    const float32 displacement  = random();

    float32 outNormalX;
    float32 outNormalY;
    float32 outNormalZ;
    float32 outNormalW;
    float32 outDisplacement;

    // normalize
    normalize(normalX, normalY, normalZ, normalW, displacement, outNormalX, outNormalY, outNormalZ, outNormalW, outDisplacement);

    Planef plane(Vector4f(normalX, normalY, normalZ, normalW), displacement);
    plane.normalize();

    EXPECT_FLOAT_EQ(plane.m_normal.x, outNormalX);
    EXPECT_FLOAT_EQ(plane.m_normal.y, outNormalY);
    EXPECT_FLOAT_EQ(plane.m_normal.z, outNormalZ);
    EXPECT_FLOAT_EQ(plane.m_normal.w, outNormalW);
    EXPECT_FLOAT_EQ(plane.m_d, outDisplacement);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(PlaneTest, Distance)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 normalX       = random();
    const float32 normalY       = random();
    const float32 normalZ       = random();
    const float32 normalW       = random();
    const float32 displacement  = random();

    const float32 pointX  = random();
    const float32 pointY  = random();
    const float32 pointZ  = random();
    const float32 pointW  = random();

    // calculate distance
    float32 referenceDistance = distance(normalX, normalY, normalZ, normalW, displacement, pointX, pointY, pointZ, pointW);

    Planef plane(Vector4f(normalX, normalY, normalZ, normalW), displacement);

    EXPECT_FLOAT_EQ(referenceDistance, plane.distance(Vector4f(pointX, pointY, pointZ, pointW)));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(PlaneTest, Side)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 normalX       = random();
    const float32 normalY       = random();
    const float32 normalZ       = random();
    const float32 normalW       = random();
    const float32 displacement  = random();

    const float32 pointX  = random();
    const float32 pointY  = random();
    const float32 pointZ  = random();
    const float32 pointW  = random();

    // calculate point side
    PlaneSide pointSide = side(normalX, normalY, normalZ, normalW, displacement, pointX, pointY, pointZ, pointW);

    const Planef plane(Vector4f(normalX, normalY, normalZ, normalW), displacement);

    EXPECT_EQ(pointSide, plane.side(Vector4f(pointX, pointY, pointZ, pointW)));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(PlaneTest, Equality)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 normalX       = random();
    const float32 normalY       = random();
    const float32 normalZ       = random();
    const float32 normalW       = random();
    const float32 displacement  = random();

    const Planef plane1(Vector4f(normalX, normalY, normalZ, normalW), displacement);
    const Planef plane2(Vector4f(normalX, normalY, normalZ, normalW), displacement);
  
    EXPECT_TRUE(plane1 == plane2);
    EXPECT_TRUE(plane2 == plane1);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
