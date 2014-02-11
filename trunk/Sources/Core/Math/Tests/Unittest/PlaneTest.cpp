#include "TestFramework/Interface/TestBase.h"
#include <EGEPlane.h>

/** Tests are focusing TPlane<float32> instantiations. */

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const int KRepetitionsCount = 20;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PlaneTest : public TestBase
{
  protected:

    PlaneTest();

    /*! Normalizes given plane data.
     *  @param  x               Plane normal X value.
     *  @param  y               Plane normal Y value.
     *  @param  z               Plane normal Z value.
     *  @param  displacemnt     Plane displacement.
     *  @param  outX            Normalized plane normal X value.
     *  @param  outY            Normalized plane normal Y value.
     *  @param  outZ            Normalized plane normal Z value.
     *  @param  outDisplacemnt  Normalized plane displacement.
     */
    void normalize(float32 x, float32 y, float32 z, float32 displacement, float32& outX, float32& outY, float32& outZ, float32& outDisplacement) const;
    /*! Returns distance of a given point to plane.
     *  @param  x               Plane normal X value.
     *  @param  y               Plane normal Y value.
     *  @param  z               Plane normal Z value.
     *  @param  displacemnt     Plane displacement.
     *  @param  pointX          Point X value.
     *  @param  pointY          Point Y value.
     *  @param  pointZ          Point Z value.
     *  @return Calculated distance.
     */
    float32 distance(float32 x, float32 y, float32 z, float32 displacement, float32 pointX, float32 pointY, float32 pointZ) const;
    /*! Returns the side of the plane on which given point exists
     *  @param  x               Plane normal X value.
     *  @param  y               Plane normal Y value.
     *  @param  z               Plane normal Z value.
     *  @param  displacemnt     Plane displacement.
     *  @param  pointX          Point X value.
     *  @param  pointY          Point Y value.
     *  @param  pointZ          Point Z value.
     *  @return Side of the plane given point is lying.
     */
    PlaneSide side(float32 x, float32 y, float32 z, float32 displacement, float32 pointX, float32 pointY, float32 pointZ) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PlaneTest::PlaneTest() : TestBase(0.0001f)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PlaneTest::normalize(float32 x, float32 y, float32 z, float32 displacement, float32& outX, float32& outY, float32& outZ, float32& outDisplacement) const
{
  outX            = x;
  outY            = y;
  outZ            = z;
  outDisplacement = displacement;

  float32 length = sqrtf((x * x) + (y * y) + (z * z));
  if (std::numeric_limits<float32>::epsilon() <= length)
  {
    outX /= length;
    outY /= length;
    outZ /= length;

    outDisplacement /= length;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 PlaneTest::distance(float32 x, float32 y, float32 z, float32 displacement, float32 pointX, float32 pointY, float32 pointZ) const
{
   return (x * pointX) + (y * pointY) + (z * pointZ) + displacement;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PlaneSide PlaneTest::side(float32 x, float32 y, float32 z, float32 displacement, float32 pointX, float32 pointY, float32 pointZ) const
{
  const float32 pointDistance = distance(x, y, z, displacement, pointX, pointY, pointZ);

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
    const float32 displacement  = random();

    // setting via constructors
    const Planef plane1(Vector3f(normalX, normalY, normalZ), displacement);
    
    EXPECT_FLOAT_EQ(plane1.m_normal.x, normalX);
    EXPECT_FLOAT_EQ(plane1.m_normal.y, normalY);
    EXPECT_FLOAT_EQ(plane1.m_normal.z, normalZ);
    EXPECT_FLOAT_EQ(plane1.m_d, displacement);

    const Planef plane2(plane1);
    
    EXPECT_FLOAT_EQ(plane2.m_normal.x, normalX);
    EXPECT_FLOAT_EQ(plane2.m_normal.y, normalY);
    EXPECT_FLOAT_EQ(plane2.m_normal.z, normalZ);
    EXPECT_FLOAT_EQ(plane2.m_d, displacement);

    // setting by setters
    Planef plane3;
    plane3.create(Vector3f(normalX, normalY, normalZ), displacement);
    
    EXPECT_FLOAT_EQ(plane3.m_normal.x, normalX);
    EXPECT_FLOAT_EQ(plane3.m_normal.y, normalY);
    EXPECT_FLOAT_EQ(plane3.m_normal.z, normalZ);
    EXPECT_FLOAT_EQ(plane3.m_d, displacement);

    Planef plane4;
    plane4.create(plane3);

    EXPECT_FLOAT_EQ(plane4.m_normal.x, normalX);
    EXPECT_FLOAT_EQ(plane4.m_normal.y, normalY);
    EXPECT_FLOAT_EQ(plane4.m_normal.z, normalZ);
    EXPECT_FLOAT_EQ(plane4.m_d, displacement);

    // setting via operators
    Planef plane5;
    plane5 = plane4;

    EXPECT_FLOAT_EQ(plane5.m_normal.x, normalX);
    EXPECT_FLOAT_EQ(plane5.m_normal.y, normalY);
    EXPECT_FLOAT_EQ(plane5.m_normal.z, normalZ);
    EXPECT_FLOAT_EQ(plane5.m_d, displacement);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(PlaneTest, Normalize)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 normalX       = random();
    const float32 normalY       = random();
    const float32 normalZ       = random();
    const float32 displacement  = random();

    float32 outNormalX;
    float32 outNormalY;
    float32 outNormalZ;
    float32 outDisplacement;

    // normalize
    normalize(normalX, normalY, normalZ, displacement, outNormalX, outNormalY, outNormalZ, outDisplacement);

    Planef plane(Vector3f(normalX, normalY, normalZ), displacement);
    plane.normalize();

    EGE_EXPECT_FLOAT_EQ(plane.m_normal.x, outNormalX, epsilon());
    EGE_EXPECT_FLOAT_EQ(plane.m_normal.y, outNormalY, epsilon());
    EGE_EXPECT_FLOAT_EQ(plane.m_normal.z, outNormalZ, epsilon());
    EGE_EXPECT_FLOAT_EQ(plane.m_d, outDisplacement, epsilon());
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
    const float32 displacement  = random();

    const float32 pointX  = random();
    const float32 pointY  = random();
    const float32 pointZ  = random();

    // calculate distance
    float32 referenceDistance = distance(normalX, normalY, normalZ, displacement, pointX, pointY, pointZ);

    Planef plane(Vector3f(normalX, normalY, normalZ), displacement);

    EGE_EXPECT_FLOAT_EQ(referenceDistance, plane.distance(Vector3f(pointX, pointY, pointZ)), epsilon());
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
    const float32 displacement  = random();

    const float32 pointX  = random();
    const float32 pointY  = random();
    const float32 pointZ  = random();

    // calculate point side
    PlaneSide pointSide = side(normalX, normalY, normalZ, displacement, pointX, pointY, pointZ);

    const Planef plane(Vector3f(normalX, normalY, normalZ), displacement);

    EXPECT_EQ(pointSide, plane.side(Vector3f(pointX, pointY, pointZ)));
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
    const float32 displacement  = random();

    const Planef plane1(Vector3f(normalX, normalY, normalZ), displacement);
    const Planef plane2(Vector3f(normalX, normalY, normalZ), displacement);
  
    EXPECT_TRUE(plane1 == plane2);
    EXPECT_TRUE(plane2 == plane1);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
