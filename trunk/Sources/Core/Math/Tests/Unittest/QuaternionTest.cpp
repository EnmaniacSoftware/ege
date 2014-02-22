#include "TestFramework/Interface/TestBase.h"
#include "Core/Math/Tests/Unittest/Helpers/QuaternionHelper.h"
#include "Core/Math/Tests/Unittest/Helpers/MatrixHelper.h"
#include "Core/Math/Tests/Unittest/Helpers/VectorHelper.h"
#include <EGEQuaternion.h>
#include <EGEVector3.h>

/** Tests are focusing TQuaternion<float32> instantiations. */

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const int KRepetitionsCount = 20;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class QuaternionTest : public TestBase
{
  protected:

    QuaternionTest();

    /*! Calculates data from given axis and angle around it. 
     *  @param  axisX   Axis X value.
     *  @param  axisY   Axis Y value.
     *  @param  axisZ   Axis Z value.
     *  @param  radians Angle in radians.
     *  @return Calculated quaternion.
     */
    std::vector<float32> calculate(float32 axisX, float32 axisY, float32 axisZ, float32 radians) const;
    /*! Normalizes quaternion data. 
     *  @param  quaternion  Quaternion to normalize.
     *  @return Normalized quaternion.
     */
    std::vector<float32> normalize(const std::vector<float32>& quaternion) const;
    /*! Returns quaternions length.
     *  @param  quaternion  Quaternion.
     */
    float32 length(const std::vector<float32>& quaternion) const;
    /*! Returns quaternions squered length.
     *  @param  quaternion  Quaternion.
     */
    float32 lengthSquared(const std::vector<float32>& quaternion) const;
    /*! Calculates conjugate of a quaternion.
     *  @param  quaternion  Quaternion.
     *  @return Calculated quaternion.
     */
    std::vector<float32> conjugate(const std::vector<float32>& quaternion) const;
    /*! Calculates multiplication of two quaternions.
     *  @param  quaternion1 Quaternion 1.
     *  @param  quaternion2 Quaternion 2.
     *  @return Calculated quaternion.
     */
    std::vector<float32> multiply(const std::vector<float32>& quaternion1, const std::vector<float32>& quaternion2) const;
    /*! Calculates dot product of two quaternions.
     *  @param  quaternion1 Quaternion 1.
     *  @param  quaternion2 Quaternion 2.
     *  @return Calculated dot product between quaternions.
     */
    float32 dotProduct(const std::vector<float32>& quaternion1, const std::vector<float32>& quaternion2) const;
    /*! Calculates quaternion resulting from slerping between other two.
     *  @param  from      Quaternion from which interpolation should be started.
     *  @param  to        Quaternion to which interpolation is done.
     *  @param  parameter Scalar in [0-1] range describing relative progress of interpolation.
     *  @param  shortest  If set interpolation is done via shortest route.
     *  @return Resulting quaternion.
     */
    std::vector<float32> slerp(const std::vector<float32>& from, const std::vector<float32>& to, float32 parameter, bool shortest) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QuaternionTest::QuaternionTest() : TestBase(0.0001f)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<float32> QuaternionTest::calculate(float32 axisX, float32 axisY, float32 axisZ, float32 radians) const
{
  std::vector<float32> out = QuaternionHelper::Identity();

  // axis is unit length
  // angle = A
  // The quaternion representing the rotation is
  //   q = cos(A / 2) + sin(A / 2) * (x * i + y * j + z * k)

  // get half angle
  float32 halfAngle = radians * 0.5f;

  // calculate the sin(halfAngle) once for optimization
	float32 sin = sinf(halfAngle);
		
	// calculate the x, y and z of the quaternion
	out[0] = axisX * sin;
	out[1] = axisY * sin;
	out[2] = axisZ * sin;
	out[3] = cosf(halfAngle);

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<float32> QuaternionTest::normalize(const std::vector<float32>& quaternion) const
{
  std::vector<float32> out = quaternion;

  // get length
  float32 len = length(quaternion);

  if (std::numeric_limits<float32>::epsilon() <= len)
  {
    float32 factor = 1.0f / len;
  
    out[0] = quaternion[0] * factor;
    out[1] = quaternion[1] * factor;
    out[2] = quaternion[2] * factor;
    out[3] = quaternion[3] * factor;
  }

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 QuaternionTest::length(const std::vector<float32>& quaternion) const
{
  return sqrtf((quaternion[0] * quaternion[0]) + (quaternion[1] * quaternion[1]) + (quaternion[2] * quaternion[2]) + (quaternion[3] * quaternion[3]));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 QuaternionTest::lengthSquared(const std::vector<float32>& quaternion) const
{
  return (quaternion[0] * quaternion[0]) + (quaternion[1] * quaternion[1]) + (quaternion[2] * quaternion[2]) + (quaternion[3] * quaternion[3]);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<float32> QuaternionTest::conjugate(const std::vector<float32>& quaternion) const
{
  std::vector<float32> out = QuaternionHelper::Identity();

  out[0] = -quaternion[0];
  out[1] = -quaternion[1];
  out[2] = -quaternion[2];
  out[3] = +quaternion[3];

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<float32> QuaternionTest::multiply(const std::vector<float32>& quaternion1, const std::vector<float32>& quaternion2) const
{
  std::vector<float32> out = QuaternionHelper::Identity();

  out[0] = quaternion1[3] * quaternion2[0] + quaternion1[0] * quaternion2[3] + quaternion1[1] * quaternion2[2] - quaternion1[2] * quaternion2[1];
  out[1] = quaternion1[3] * quaternion2[1] + quaternion1[1] * quaternion2[3] + quaternion1[2] * quaternion2[0] - quaternion1[0] * quaternion2[2];
  out[2] = quaternion1[3] * quaternion2[2] + quaternion1[2] * quaternion2[3] + quaternion1[0] * quaternion2[1] - quaternion1[1] * quaternion2[0];
  out[3] = quaternion1[3] * quaternion2[3] - quaternion1[0] * quaternion2[0] - quaternion1[1] * quaternion2[1] - quaternion1[2] * quaternion2[2];

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 QuaternionTest::dotProduct(const std::vector<float32>& quaternion1, const std::vector<float32>& quaternion2) const
{
  return (quaternion1[0] * quaternion2[0]) + (quaternion1[1] * quaternion2[1]) + (quaternion1[2] * quaternion2[2]) + (quaternion1[3] * quaternion2[3]);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<float32> QuaternionTest::slerp(const std::vector<float32>& from, const std::vector<float32>& to, float32 parameter, bool shortest) const
{
  std::vector<float32> out = QuaternionHelper::RandomData();
  std::vector<float32> tmp = QuaternionHelper::RandomData();

  //
  // p     - source quaternion
  // q     - destination quaternion
  // t     - time
  // omega - angle between p and q
  //
  //                    p*sin( ( 1-t )*omega )+q*sin( t*omega )
  // slerp( p, q, t ) = ---------------------------------------
  //                                 sin( omega )
  //

  // calculate cosine omega (dot product of 2 quaternions)
  float32 cosOmega = dotProduct(from, to);

  // adjust the signs
  if ((0 > cosOmega) && shortest)
  {
    cosOmega = -cosOmega;

    // NOTE: q and -q rotates from the same start point and to the same end point but thru opposite direction
    tmp[0] = -to[0];
    tmp[1] = -to[1];
    tmp[2] = -to[2];
    tmp[3] = -to[3];
  }
  else
  {
    tmp[0] = to[0];
    tmp[1] = to[1];
    tmp[2] = to[2];
    tmp[3] = to[3];
  }

  // calculate coefficients
  if ((1.0f - std::numeric_limits<float32>::epsilon()) > fabsf(cosOmega))
  {
    // standard case (slerp)
    float32 sinOmega = sqrtf(1 - cosOmega * cosOmega);
    
    float32 invSinOmega = 1 / sinOmega;

    float32 angle = atan2f(sinOmega, cosOmega);

    float32 coeff0 = sinf((1 - parameter) * angle) * invSinOmega;
    float32 coeff1 = sinf(parameter * angle) * invSinOmega;

    out[0] = coeff0 * from[0] + coeff1 * tmp[0];
    out[1] = coeff0 * from[1] + coeff1 * tmp[1];
    out[2] = coeff0 * from[2] + coeff1 * tmp[2];
    out[3] = coeff0 * from[3] + coeff1 * tmp[3];
  }
  else
  {
    // There are two situations:
    // 1. "from" and "to" are very close (cos ~= +1), so we can do a linear interpolation safely.
    // 2. "from" and "to" are almost inverse of each other (cos ~= -1), there are an infinite number of possibilities interpolation. 
    //    Do linear interpolation here as well as no other way to fix it yet.
    out[0] = (1 - parameter) * from[0] + parameter * tmp[0];
    out[1] = (1 - parameter) * from[1] + parameter * tmp[1];
    out[2] = (1 - parameter) * from[2] + parameter * tmp[2];
    out[3] = (1 - parameter) * from[3] + parameter * tmp[3];

    // taking the complement requires renormalisation
    out = normalize(out);
  }

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(QuaternionTest, SetValue)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const std::vector<float32> q = QuaternionHelper::RandomData();
    std::vector<float32> out;

    // setting via constructors...

    // 1st
    const Quaternionf quaternion(q[0], q[1], q[2], q[3]);
    EXPECT_FLOAT_EQ(q[0], quaternion.x);
    EXPECT_FLOAT_EQ(q[1], quaternion.y);
    EXPECT_FLOAT_EQ(q[2], quaternion.z);
    EXPECT_FLOAT_EQ(q[3], quaternion.w);

    // 2nd
    const Quaternionf quaternion2(quaternion);
    EXPECT_FLOAT_EQ(quaternion2.x, quaternion.x);
    EXPECT_FLOAT_EQ(quaternion2.y, quaternion.y);
    EXPECT_FLOAT_EQ(quaternion2.z, quaternion.z);
    EXPECT_FLOAT_EQ(quaternion2.w, quaternion.w);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(QuaternionTest, Normalize)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const std::vector<float32> quaternion = QuaternionHelper::RandomData();

    // calculate reference value
    const std::vector<float32> out = normalize(quaternion);

    // calculate actual value
    Quaternionf quat(quaternion[0], quaternion[1], quaternion[2], quaternion[3]);
    quat.normalize();

    // test
    EXPECT_FLOAT_EQ(out[0], quat.x);
    EXPECT_FLOAT_EQ(out[1], quat.y);
    EXPECT_FLOAT_EQ(out[2], quat.z);
    EXPECT_FLOAT_EQ(out[3], quat.w);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(QuaternionTest, Radians)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const std::vector<float32> axisData = VectorHelper::RandomVector3Data(true);
    const float32 radians = random();

    // create actual value
    const Quaternionf quaterion = Math::CreateQuaternion(Vector3f(axisData[0], axisData[1], axisData[2]), Angle(radians));
   
    // NOTE: it is possible returned angle to have opposite sign as Cosine returns the same values for + and -
    EGE_EXPECT_FLOAT_EQ(fabsf(radians), fabsf(quaterion.radians()), epsilon());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(QuaternionTest, Conjugate)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const std::vector<float32> quaternion = QuaternionHelper::RandomData();

    // calculate reference value
    const std::vector<float32> out = conjugate(quaternion);

    // calculate actual value
    Quaternionf quaternion1(quaternion[0], quaternion[1], quaternion[2], quaternion[3]);
    Quaternionf quaternion2 = quaternion1.conjugated();
    quaternion1.conjugate();

    EXPECT_FLOAT_EQ(out[0], quaternion1.x);
    EXPECT_FLOAT_EQ(out[1], quaternion1.y);
    EXPECT_FLOAT_EQ(out[2], quaternion1.z);
    EXPECT_FLOAT_EQ(out[3], quaternion1.w);

    EXPECT_FLOAT_EQ(out[0], quaternion2.x);
    EXPECT_FLOAT_EQ(out[1], quaternion2.y);
    EXPECT_FLOAT_EQ(out[2], quaternion2.z);
    EXPECT_FLOAT_EQ(out[3], quaternion2.w);

    // conjugate the congugated quaternion
    quaternion1.conjugate();

    EXPECT_FLOAT_EQ(quaternion[0], quaternion1.x);
    EXPECT_FLOAT_EQ(quaternion[1], quaternion1.y);
    EXPECT_FLOAT_EQ(quaternion[2], quaternion1.z);
    EXPECT_FLOAT_EQ(quaternion[3], quaternion1.w);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(QuaternionTest, Scaling)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const std::vector<float32> quat = QuaternionHelper::RandomData();
    const float32 scale = random();

    Quaternionf quaternion(quat[0], quat[1], quat[2], quat[3]);
    quaternion = quaternion * scale;
    
    EXPECT_FLOAT_EQ(quat[0] * scale, quaternion.x);
    EXPECT_FLOAT_EQ(quat[1] * scale, quaternion.y);
    EXPECT_FLOAT_EQ(quat[2] * scale, quaternion.z);
    EXPECT_FLOAT_EQ(quat[3] * scale, quaternion.w);

    quaternion = Quaternionf(quat[0], quat[1], quat[2], quat[3]);
    quaternion = scale * quaternion;
    
    EXPECT_FLOAT_EQ(quat[0] * scale, quaternion.x);
    EXPECT_FLOAT_EQ(quat[1] * scale, quaternion.y);
    EXPECT_FLOAT_EQ(quat[2] * scale, quaternion.z);
    EXPECT_FLOAT_EQ(quat[3] * scale, quaternion.w);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(QuaternionTest, Addition)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const std::vector<float32> quat1 = QuaternionHelper::RandomData();
    const std::vector<float32> quat2 = QuaternionHelper::RandomData();

    const Quaternionf quaternion1(quat1[0], quat1[1], quat1[2], quat1[3]);
    const Quaternionf quaternion2(quat2[0], quat2[1], quat2[2], quat2[3]);

    // compare q1 + q2
    EXPECT_FLOAT_EQ(quat1[0] + quat2[0], (quaternion1 + quaternion2).x);
    EXPECT_FLOAT_EQ(quat1[1] + quat2[1], (quaternion1 + quaternion2).y);
    EXPECT_FLOAT_EQ(quat1[2] + quat2[2], (quaternion1 + quaternion2).z);
    EXPECT_FLOAT_EQ(quat1[3] + quat2[3], (quaternion1 + quaternion2).w);

    // compare q2 + q1
    EXPECT_FLOAT_EQ(quat2[0] + quat1[0], (quaternion2 + quaternion1).x);
    EXPECT_FLOAT_EQ(quat2[1] + quat1[1], (quaternion2 + quaternion1).y);
    EXPECT_FLOAT_EQ(quat2[2] + quat1[2], (quaternion2 + quaternion1).z);
    EXPECT_FLOAT_EQ(quat2[3] + quat1[3], (quaternion2 + quaternion1).w);

    // compare q1 + q2 and q2 + q1
    EXPECT_FLOAT_EQ((quaternion1 + quaternion2).x, (quaternion2 + quaternion1).x);
    EXPECT_FLOAT_EQ((quaternion1 + quaternion2).y, (quaternion2 + quaternion1).y);
    EXPECT_FLOAT_EQ((quaternion1 + quaternion2).z, (quaternion2 + quaternion1).z);
    EXPECT_FLOAT_EQ((quaternion1 + quaternion2).w, (quaternion2 + quaternion1).w);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(QuaternionTest, Negation)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const std::vector<float32> quat = QuaternionHelper::RandomData();

    Quaternionf quaternion(quat[0], quat[1], quat[2], quat[3]);
    quaternion = -quaternion;
    
    EXPECT_FLOAT_EQ(-quat[0], quaternion.x);
    EXPECT_FLOAT_EQ(-quat[1], quaternion.y);
    EXPECT_FLOAT_EQ(-quat[2], quaternion.z);
    EXPECT_FLOAT_EQ(-quat[3], quaternion.w);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(QuaternionTest, Multiply)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const std::vector<float32> quat1 = QuaternionHelper::RandomData();
    const std::vector<float32> quat2 = QuaternionHelper::RandomData();

    const Quaternionf quaternion1(quat1[0], quat1[1], quat1[2], quat1[3]);
    const Quaternionf quaternion2(quat2[0], quat2[1], quat2[2], quat2[3]);

    // calculate reference results
    const std::vector<float32> out12 = multiply(quat1, quat2);
    const std::vector<float32> out21 = multiply(quat2, quat1);
    
    // test multiplication by methods...
    Quaternionf quat12 = quaternion1.multiply(quaternion2);
    Quaternionf quat21 = quaternion2.multiply(quaternion1);

    EXPECT_FLOAT_EQ(out12[0], quat12.x);
    EXPECT_FLOAT_EQ(out12[1], quat12.y);
    EXPECT_FLOAT_EQ(out12[2], quat12.z);
    EXPECT_FLOAT_EQ(out12[3], quat12.w);

    EXPECT_FLOAT_EQ(out21[0], quat21.x);
    EXPECT_FLOAT_EQ(out21[1], quat21.y);
    EXPECT_FLOAT_EQ(out21[2], quat21.z);
    EXPECT_FLOAT_EQ(out21[3], quat21.w);

    // ...and operators
    quat12 = quaternion1 * quaternion2;
    quat21 = quaternion2 * quaternion1;

    EXPECT_FLOAT_EQ(out12[0], quat12.x);
    EXPECT_FLOAT_EQ(out12[1], quat12.y);
    EXPECT_FLOAT_EQ(out12[2], quat12.z);
    EXPECT_FLOAT_EQ(out12[3], quat12.w);

    EXPECT_FLOAT_EQ(out21[0], quat21.x);
    EXPECT_FLOAT_EQ(out21[1], quat21.y);
    EXPECT_FLOAT_EQ(out21[2], quat21.z);
    EXPECT_FLOAT_EQ(out21[3], quat21.w);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(QuaternionTest, DotProduct)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const std::vector<float32> quat1 = QuaternionHelper::RandomData();
    const std::vector<float32> quat2 = QuaternionHelper::RandomData();

    const Quaternionf quaternion1(quat1[0], quat1[1], quat1[2], quat1[3]);
    const Quaternionf quaternion2(quat2[0], quat2[1], quat2[2], quat2[3]);
    
    // test q1 * q2
    EXPECT_FLOAT_EQ(dotProduct(quat1, quat2), quaternion1.dotProduct(quaternion2));
    
    // test q2 * q1
    EXPECT_FLOAT_EQ(dotProduct(quat2, quat1), quaternion2.dotProduct(quaternion1));
    
    // test q1 *q2 == q2 * q1
    EXPECT_FLOAT_EQ(quaternion1.dotProduct(quaternion2), quaternion2.dotProduct(quaternion1));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(QuaternionTest, Equality)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const std::vector<float32> quat1 = QuaternionHelper::RandomData();
    const std::vector<float32> quat2 = QuaternionHelper::RandomData();

    const Quaternionf quaternion1(quat1[0], quat1[1], quat1[2], quat1[3]);    // 
    const Quaternionf quaternion2(quat1[0], quat1[1], quat1[2], quat1[3]);    // same
    const Quaternionf quaternion3(quat2[0], quat2[1], quat2[2], quat2[3]);
    
    // test q1 == q2
    EXPECT_TRUE(quaternion1 == quaternion2);
    
    // test q2 == q1
    EXPECT_TRUE(quaternion2 == quaternion1);
    
    // test q1 != q3
    EXPECT_FALSE(quaternion1 == quaternion3);

    // test q3 != q1
    EXPECT_FALSE(quaternion3 == quaternion1);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(QuaternionTest, Slerp)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const std::vector<float32> from = QuaternionHelper::RandomData();
    const std::vector<float32> to   = QuaternionHelper::RandomData();

    const Quaternionf quaternionFrom(from[0], from[1], from[2], from[3]);
    const Quaternionf quaternionTo(to[0], to[1], to[2], to[3]);

    // interpolate
    for (float32 t = 0; t <= 1.0f; t += 0.01f)
    {
      // calculate reference value (do not necessairly shortest)
      std::vector<float32> out = slerp(from, to, t, false);

      // calculate actual value (do not necessairly shortest)
      Quaternionf quaternionOut = quaternionFrom.slerp(quaternionTo, t, false);
    
      // test
      EGE_EXPECT_FLOAT_EQ(out[0], quaternionOut.x, epsilon());
      EGE_EXPECT_FLOAT_EQ(out[1], quaternionOut.y, epsilon());
      EGE_EXPECT_FLOAT_EQ(out[2], quaternionOut.z, epsilon());
      EGE_EXPECT_FLOAT_EQ(out[3], quaternionOut.w, epsilon());
    
      // calculate reference value (shortest)
      out = slerp(from, to, t, true);

      // calculate actual value (shortest)
      quaternionOut = quaternionFrom.slerp(quaternionTo, t, true);
    
      // test
      EGE_EXPECT_FLOAT_EQ(out[0], quaternionOut.x, epsilon());
      EGE_EXPECT_FLOAT_EQ(out[1], quaternionOut.y, epsilon());
      EGE_EXPECT_FLOAT_EQ(out[2], quaternionOut.z, epsilon());
      EGE_EXPECT_FLOAT_EQ(out[3], quaternionOut.w, epsilon());
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
