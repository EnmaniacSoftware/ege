#include "TestFramework/Interface/TestBase.h"
#include <EGEQuaternion.h>

/** Tests are focusing TQuaternion<float32> instantiations. */

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const int KRepetitionsCount = 20;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class QuaternionTest : public TestBase
{
  protected:

    QuaternionTest();

    /*! Generates random matrix data. 
     *  @param  data  Array of data to be randomized.
     */
    void randomData(float32 data[16]) const;
    /*! Calculates data from given axis and angle around it. 
     *  @param  axisX   Axis X value.
     *  @param  axisY   Axis Y value.
     *  @param  axisZ   Axis Z value.
     *  @param  radians Angle in radians.
     *  @param  outX    Calculated quaternion X value.
     *  @param  outY    Calculated quaternion Y value.
     *  @param  outZ    Calculated quaternion Z value.
     *  @param  outW    Calculated quaternion W value.
     */
    void calculate(float32 axisX, float32 axisY, float32 axisZ, float32 radians, float32& outX, float32& outY, float32& outZ, float32& outW) const;
    /*! Calculates data from given matrix. 
     *  @param  matridData  Matrix data.
     *  @param  outX    Calculated quaternion X value.
     *  @param  outY    Calculated quaternion Y value.
     *  @param  outZ    Calculated quaternion Z value.
     *  @param  outW    Calculated quaternion W value.
     */
    void calculate(const float32 matrixData[16], float32& outX, float32& outY, float32& outZ, float32& outW) const;
    /*! Normalizes quaternion data. 
     *  @param  x     Quaternion X data.
     *  @param  y     Quaternion Y data.
     *  @param  z     Quaternion Z data.
     *  @param  w     Quaternion W data.
     *  @param  outX  Calculated quaternion X value.
     *  @param  outY  Calculated quaternion Y value.
     *  @param  outZ  Calculated quaternion Z value.
     *  @param  outW  Calculated quaternion W value.
     */
    void normalize(float32 x, float32 y, float32 z, float32 w, float32& outX, float32& outY, float32& outZ, float32& outW) const;
    /*! Returns quaternions length.
     *  @param  x Quaternion X data.
     *  @param  y Quaternion Y data.
     *  @param  z Quaternion Z data.
     *  @param  w Quaternion W data.
     */
    float32 length(float32 x, float32 y, float32 z, float32 w) const;
    /*! Returns quaternions squered length.
     *  @param  x Quaternion X data.
     *  @param  y Quaternion Y data.
     *  @param  z Quaternion Z data.
     *  @param  w Quaternion W data.
     */
    float32 lengthSquared(float32 x, float32 y, float32 z, float32 w) const;
    /*! Calculates conjugate of a quaternion.
     *  @param  x     Quaternion X data.
     *  @param  y     Quaternion Y data.
     *  @param  z     Quaternion Z data.
     *  @param  w     Quaternion W data.
     *  @param  outX  Calculated quaternion X value.
     *  @param  outY  Calculated quaternion Y value.
     *  @param  outZ  Calculated quaternion Z value.
     *  @param  outW  Calculated quaternion W value.
     */
    void conjugate(float32 x, float32 y, float32 z, float32 w, float32& outX, float32& outY, float32& outZ, float32& outW) const;
    /*! Calculates multiplication of two quaternions.
     *  @param  x1    Quaternion 1 X data.
     *  @param  y1    Quaternion 1 Y data.
     *  @param  z1    Quaternion 1 Z data.
     *  @param  w1    Quaternion 1 W data.
     *  @param  x2    Quaternion 2 X data.
     *  @param  y2    Quaternion 2 Y data.
     *  @param  z2    Quaternion 2 Z data.
     *  @param  w2    Quaternion 2 W data.
     *  @param  outX  Calculated quaternion X value.
     *  @param  outY  Calculated quaternion Y value.
     *  @param  outZ  Calculated quaternion Z value.
     *  @param  outW  Calculated quaternion W value.
     */
    void multiply(float32 x1, float32 y1, float32 z1, float32 w1, float32 x2, float32 y2, float32 z2, float32 w2, 
                  float32& outX, float32& outY, float32& outZ, float32& outW) const;
    /*! Calculates dot product of two quaternions.
     *  @param  x1    Quaternion 1 X data.
     *  @param  y1    Quaternion 1 Y data.
     *  @param  z1    Quaternion 1 Z data.
     *  @param  w1    Quaternion 1 W data.
     *  @param  x2    Quaternion 2 X data.
     *  @param  y2    Quaternion 2 Y data.
     *  @param  z2    Quaternion 2 Z data.
     *  @param  w2    Quaternion 2 W data.
     *  @return Calculated dot product between quaternions.
     */
    float32 dotProduct(float32 x1, float32 y1, float32 z1, float32 w1, float32 x2, float32 y2, float32 z2, float32 w2) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QuaternionTest::QuaternionTest() : TestBase(0.0001f)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void QuaternionTest::randomData(float32 data[16]) const
{
  for (int i = 0; i < 16; ++i)
  {
    data[i] = random();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void QuaternionTest::calculate(float32 axisX, float32 axisY, float32 axisZ, float32 radians, float32& outX, float32& outY, float32& outZ, float32& outW) const
{
  // axis is unit length
  // angle = A
  // The quaternion representing the rotation is
  //   q = cos(A / 2) + sin(A / 2) * (x * i + y * j + z * k)

  // get half angle
  float32 halfAngle = radians * 0.5f;

  // calculate the sin(halfAngle) once for optimization
	float32 sin = sinf(halfAngle);
		
	// calculate the x, y and z of the quaternion
	outX = axisX * sin;
	outY = axisY * sin;
	outZ = axisZ * sin;
	outW = cosf(halfAngle);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void QuaternionTest::calculate(const float32 matrixData[16], float32& outX, float32& outY, float32& outZ, float32& outW) const
{
  // Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
  // article "Quaternion Calculus and Fast Animation".

  float32 trace = matrixData[0] + matrixData[5] + matrixData[10];
  float32 root;

  if (0 < trace)
  {
    // |w| > 1/2, may as well choose w > 1/2
    root = sqrtf(trace + 1.0f);  // 2w
    outW = 0.5f * root;
    root = 0.5f / root;  // 1/(4w)
    outX = (matrixData[9] - matrixData[6]) * root;
    outY = (matrixData[2] - matrixData[8]) * root;
    outZ = (matrixData[4] - matrixData[1]) * root;
  }
  else
  {
    // |w| <= 1/2
    static size_t s_iNext[3] = { 1, 2, 0 };

    size_t i = 0;
    if (matrixData[5] > matrixData[0])
    {
      i = 1;
    }
    if (matrixData[10] > matrixData[i * 4 + i])
    {
      i = 2;
    }
    
    size_t j = s_iNext[i];
    size_t k = s_iNext[j];

    root = sqrtf(matrixData[i * 4 + i] - matrixData[j * 4 + j] - matrixData[k * 4 + k] + 1.0f);
    float32* apkQuat[3] = { &outX, &outY, &outZ };
    *apkQuat[i] = 0.5f * root;
    root = 0.5f / root;
    outW = (matrixData[k * 4 + j] - matrixData[j * 4 + k]) * root;
    *apkQuat[j] = (matrixData[j * 4 + i] + matrixData[i * 4 + j]) * root;
    *apkQuat[k] = (matrixData[k * 4 + i] + matrixData[i * 4 + k]) * root;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void QuaternionTest::normalize(float32 x, float32 y, float32 z, float32 w, float32& outX, float32& outY, float32& outZ, float32& outW) const
{
  outX = x;
  outY = y;
  outZ = z;
  outW = w;

  // get length
  float32 len = length(x, y, z, w);

  if (std::numeric_limits<float32>::epsilon() <= len)
  {
    float32 factor = 1.0f / len;
  
    outX = x * factor;
    outY = y * factor;
    outZ = z * factor;
    outW = w * factor;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 QuaternionTest::length(float32 x, float32 y, float32 z, float32 w) const
{
  return sqrtf((x * x) + (y * y) + (z * z) + (w * w));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 QuaternionTest::lengthSquared(float32 x, float32 y, float32 z, float32 w) const
{
  return (x * x) + (y * y) + (z * z) + (w * w);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void QuaternionTest::conjugate(float32 x, float32 y, float32 z, float32 w, float32& outX, float32& outY, float32& outZ, float32& outW) const
{
  outX = -x;
  outY = -y;
  outZ = -z;
  outW = w;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void QuaternionTest::multiply(float32 x1, float32 y1, float32 z1, float32 w1, float32 x2, float32 y2, float32 z2, float32 w2, 
                              float32& outX, float32& outY, float32& outZ, float32& outW) const
{
  outX = w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2;
  outY = w1 * y2 + y1 * w2 + z1 * x2 - x1 * z2;
  outZ = w1 * z2 + z1 * w2 + x1 * y2 - y1 * x2;
  outW = w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 QuaternionTest::dotProduct(float32 x1, float32 y1, float32 z1, float32 w1, float32 x2, float32 y2, float32 z2, float32 w2) const
{
  return (x1 * x2) + (y1 * y2) + (z1 * z2) + (w1 * w2);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(QuaternionTest, SetValue)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x = random();
    const float32 y = random();
    const float32 z = random();
    const float32 w = random();

    float32 out[4];

    // setting via constructors...

    // 1st
    const Quaternionf quaternion(x, y, z, w);
    EXPECT_FLOAT_EQ(quaternion.x, x);
    EXPECT_FLOAT_EQ(quaternion.y, y);
    EXPECT_FLOAT_EQ(quaternion.z, z);
    EXPECT_FLOAT_EQ(quaternion.w, w);

    // 2nd
    const Quaternionf quaternion2(quaternion);
    EXPECT_FLOAT_EQ(quaternion2.x, quaternion.x);
    EXPECT_FLOAT_EQ(quaternion2.y, quaternion.y);
    EXPECT_FLOAT_EQ(quaternion2.z, quaternion.z);
    EXPECT_FLOAT_EQ(quaternion2.w, quaternion.w);

    // 3rd
    const float32 axisX   = random();
    const float32 axisY   = random();
    const float32 axisZ   = random();
    const float32 radians = random();

    calculate(axisX, axisY, axisZ, radians, out[0], out[1], out[2], out[3]);
    const Quaternionf quaternion3(Vector3f(axisX, axisY, axisZ), Angle(radians));

    EXPECT_FLOAT_EQ(quaternion3.x, out[0]);
    EXPECT_FLOAT_EQ(quaternion3.y, out[1]);
    EXPECT_FLOAT_EQ(quaternion3.z, out[2]);
    EXPECT_FLOAT_EQ(quaternion3.w, out[3]);

    // 4th
    float32 data[16];
    randomData(data);

    const Matrix4f matrix(data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10], data[11], data[12], data[13], 
                          data[14], data[15]);

    calculate(data, out[0], out[1], out[2], out[3]);
    Quaternionf quaternion4(matrix);

    EXPECT_FLOAT_EQ(quaternion4.x, out[0]);
    EXPECT_FLOAT_EQ(quaternion4.y, out[1]);
    EXPECT_FLOAT_EQ(quaternion4.z, out[2]);
    EXPECT_FLOAT_EQ(quaternion4.w, out[3]);

    // setting by setters
    calculate(axisX, axisY, axisZ, radians, out[0], out[1], out[2], out[3]);
    Quaternionf quaternion5;
    quaternion5.create(Vector3f(axisX, axisY, axisZ), Angle(radians));

    EXPECT_FLOAT_EQ(quaternion5.x, out[0]);
    EXPECT_FLOAT_EQ(quaternion5.y, out[1]);
    EXPECT_FLOAT_EQ(quaternion5.z, out[2]);
    EXPECT_FLOAT_EQ(quaternion5.w, out[3]);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(QuaternionTest, Normalize)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x = random();
    const float32 y = random();
    const float32 z = random();
    const float32 w = random();

    float32 out[4];

    Quaternionf quaternion(x, y, z, w);
    quaternion.normalize();

    normalize(x, y, z, w, out[0], out[1], out[2], out[3]);

    EXPECT_FLOAT_EQ(quaternion.x, out[0]);
    EXPECT_FLOAT_EQ(quaternion.y, out[1]);
    EXPECT_FLOAT_EQ(quaternion.z, out[2]);
    EXPECT_FLOAT_EQ(quaternion.w, out[3]);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// TAGE - due to precision loss in Angle::distanceTo method this tests have tendency to fail. Find better way to either do the calculations or test
TEST_F(QuaternionTest, DISABLED_ConvertToVector)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 axisX   = random();
    const float32 axisY   = random();
    const float32 axisZ   = random();
    const float32 radians = random();

    const Quaternionf quaterion1(Vector3f(axisX, axisY, axisZ), Angle(radians));
    
    Vector3f axis;
    Angle angle;
    quaterion1.convertTo(axis, angle);

    EGE_EXPECT_FLOAT_EQ(axisX, axis.x, epsilon());
    EGE_EXPECT_FLOAT_EQ(axisY, axis.y, epsilon());
    EGE_EXPECT_FLOAT_EQ(axisZ, axis.z, epsilon());
    EGE_EXPECT_FLOAT_EQ(radians, angle.radians(), epsilon());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// TAGE - investigate why it does not work
TEST_F(QuaternionTest, DISABLED_ConvertToMatrix)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    // ...and to Matrix4
    float32 data[16];
    randomData(data);

    const Matrix4f matrix(data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10], data[11], data[12], data[13], 
                          data[14], data[15]);

    const Quaternionf quaterion2(matrix);

    Matrix4f matrixOut;
    quaterion2.convertTo(matrixOut);

    // NOTE: only test 3x3 rotational matrix
    EGE_EXPECT_FLOAT_EQ(data[0], matrixOut.data[0], epsilon());
    EGE_EXPECT_FLOAT_EQ(data[1], matrixOut.data[1], epsilon());
    EGE_EXPECT_FLOAT_EQ(data[2], matrixOut.data[2], epsilon());

    EGE_EXPECT_FLOAT_EQ(data[4], matrixOut.data[4], epsilon());
    EGE_EXPECT_FLOAT_EQ(data[5], matrixOut.data[5], epsilon());
    EGE_EXPECT_FLOAT_EQ(data[6], matrixOut.data[6], epsilon());

    EGE_EXPECT_FLOAT_EQ(data[8], matrixOut.data[8], epsilon());
    EGE_EXPECT_FLOAT_EQ(data[9], matrixOut.data[9], epsilon());
    EGE_EXPECT_FLOAT_EQ(data[10], matrixOut.data[10], epsilon());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// TAGE - due to precision loss in Angle::distanceTo method this tests have tendency to fail. Find better way to either do the calculations or test
TEST_F(QuaternionTest, DISABLED_Angle)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    // test convertion to Vector3...
    const float32 axisX   = random();
    const float32 axisY   = random();
    const float32 axisZ   = random();
    const float32 radians = random();

    //float32 len = sqrtf(axisX * axisX + axisY * axisY + axisZ * axisZ);
    //axisX /= len;
    //axisY /= len;
    //axisY /= len;
 
    const Quaternionf quaterion(Vector3f(axisX, axisY, axisZ), Angle(radians));
    
    // TAGE - it seems the angle is sometimes negated, inestigate why this can happen
    EXPECT_FLOAT_EQ(fabs(radians), fabs(quaterion.angle().radians()));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(QuaternionTest, Conjugate)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x = random();
    const float32 y = random();
    const float32 z = random();
    const float32 w = random();

    float32 out[4];

    Quaternionf quaternion1(x, y, z, w);
    Quaternionf quaternion2 = quaternion1.conjugated();
    quaternion1.conjugate();

    conjugate(x, y, z, w, out[0], out[1], out[2], out[3]);

    EXPECT_FLOAT_EQ(quaternion1.x, out[0]);
    EXPECT_FLOAT_EQ(quaternion1.y, out[1]);
    EXPECT_FLOAT_EQ(quaternion1.z, out[2]);
    EXPECT_FLOAT_EQ(quaternion1.w, out[3]);

    EXPECT_FLOAT_EQ(quaternion2.x, out[0]);
    EXPECT_FLOAT_EQ(quaternion2.y, out[1]);
    EXPECT_FLOAT_EQ(quaternion2.z, out[2]);
    EXPECT_FLOAT_EQ(quaternion2.w, out[3]);

    // conjugate the congugated quaternion
    quaternion1.conjugate();

    EXPECT_FLOAT_EQ(quaternion1.x, x);
    EXPECT_FLOAT_EQ(quaternion1.y, y);
    EXPECT_FLOAT_EQ(quaternion1.z, z);
    EXPECT_FLOAT_EQ(quaternion1.w, w);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(QuaternionTest, Scaling)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x     = random();
    const float32 y     = random();
    const float32 z     = random();
    const float32 w     = random();
    const float32 scale = random();

    Quaternionf quaternion(x, y, z, w);
    quaternion = quaternion * scale;
    
    EXPECT_FLOAT_EQ(quaternion.x, x * scale);
    EXPECT_FLOAT_EQ(quaternion.y, y * scale);
    EXPECT_FLOAT_EQ(quaternion.z, z * scale);
    EXPECT_FLOAT_EQ(quaternion.w, w * scale);

    quaternion = Quaternionf(x, y, z, w);
    quaternion = scale * quaternion;
    
    EXPECT_FLOAT_EQ(quaternion.x, x * scale);
    EXPECT_FLOAT_EQ(quaternion.y, y * scale);
    EXPECT_FLOAT_EQ(quaternion.z, z * scale);
    EXPECT_FLOAT_EQ(quaternion.w, w * scale);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(QuaternionTest, Addition)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x1 = random();
    const float32 y1 = random();
    const float32 z1 = random();
    const float32 w1 = random();
    const float32 x2 = random();
    const float32 y2 = random();
    const float32 z2 = random();
    const float32 w2 = random();

    const Quaternionf quaternion1(x1, y1, z1, w1);
    const Quaternionf quaternion2(x2, y2, z2, w2);

    // compare q1 + q2
    EXPECT_FLOAT_EQ((quaternion1 + quaternion2).x, x1 + x2);
    EXPECT_FLOAT_EQ((quaternion1 + quaternion2).y, y1 + y2);
    EXPECT_FLOAT_EQ((quaternion1 + quaternion2).z, z1 + z2);
    EXPECT_FLOAT_EQ((quaternion1 + quaternion2).w, w1 + w2);

    // compare q2 + q1
    EXPECT_FLOAT_EQ((quaternion2 + quaternion1).x, x2 + x1);
    EXPECT_FLOAT_EQ((quaternion2 + quaternion1).y, y2 + y1);
    EXPECT_FLOAT_EQ((quaternion2 + quaternion1).z, z1 + z2);
    EXPECT_FLOAT_EQ((quaternion2 + quaternion1).w, w1 + w2);

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
    const float32 x     = random();
    const float32 y     = random();
    const float32 z     = random();
    const float32 w     = random();

    Quaternionf quaternion(x, y, z, w);
    quaternion = -quaternion;
    
    EXPECT_FLOAT_EQ(quaternion.x, -x);
    EXPECT_FLOAT_EQ(quaternion.y, -y);
    EXPECT_FLOAT_EQ(quaternion.z, -z);
    EXPECT_FLOAT_EQ(quaternion.w, -w);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(QuaternionTest, Multiply)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x1 = random();
    const float32 y1 = random();
    const float32 z1 = random();
    const float32 w1 = random();
    const float32 x2 = random();
    const float32 y2 = random();
    const float32 z2 = random();
    const float32 w2 = random();

    float32 out12[4];
    float32 out21[4];

    const Quaternionf quaternion1(x1, y1, z1, w1);
    const Quaternionf quaternion2(x2, y2, z2, w2);
    
    // calculate reference results
    multiply(x1, y1, z1, w1, x2, y2, z2, w2, out12[0], out12[1], out12[2], out12[3]);
    multiply(x2, y2, z2, w2, x1, y1, z1, w1, out21[0], out21[1], out21[2], out21[3]);

    // test multiplication by methods...
    Quaternionf quat12 = quaternion1.multiply(quaternion2);
    Quaternionf quat21 = quaternion2.multiply(quaternion1);

    EXPECT_FLOAT_EQ(quat12.x, out12[0]);
    EXPECT_FLOAT_EQ(quat12.y, out12[1]);
    EXPECT_FLOAT_EQ(quat12.z, out12[2]);
    EXPECT_FLOAT_EQ(quat12.w, out12[3]);

    EXPECT_FLOAT_EQ(quat21.x, out21[0]);
    EXPECT_FLOAT_EQ(quat21.y, out21[1]);
    EXPECT_FLOAT_EQ(quat21.z, out21[2]);
    EXPECT_FLOAT_EQ(quat21.w, out21[3]);

    // ...and operators
    quat12 = quaternion1 * quaternion2;
    quat21 = quaternion2 * quaternion1;

    EXPECT_FLOAT_EQ(quat12.x, out12[0]);
    EXPECT_FLOAT_EQ(quat12.y, out12[1]);
    EXPECT_FLOAT_EQ(quat12.z, out12[2]);
    EXPECT_FLOAT_EQ(quat12.w, out12[3]);

    EXPECT_FLOAT_EQ(quat21.x, out21[0]);
    EXPECT_FLOAT_EQ(quat21.y, out21[1]);
    EXPECT_FLOAT_EQ(quat21.z, out21[2]);
    EXPECT_FLOAT_EQ(quat21.w, out21[3]);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(QuaternionTest, DotProduct)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x1 = random();
    const float32 y1 = random();
    const float32 z1 = random();
    const float32 w1 = random();
    const float32 x2 = random();
    const float32 y2 = random();
    const float32 z2 = random();
    const float32 w2 = random();

    const Quaternionf quaternion1(x1, y1, z1, w1);
    const Quaternionf quaternion2(x2, y2, z2, w2);
    
    // test q1 * q2
    EXPECT_FLOAT_EQ(dotProduct(x1, y1, z1, w1, x2, y2, z2, w2), quaternion1.dotProduct(quaternion2));
    
    // test q2 * q1
    EXPECT_FLOAT_EQ(dotProduct(x2, y2, z2, w2, x1, y1, z1, w1), quaternion2.dotProduct(quaternion1));
    
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
    const float32 x1 = random();
    const float32 y1 = random();
    const float32 z1 = random();
    const float32 w1 = random();
    const float32 x2 = random();
    const float32 y2 = random();
    const float32 z2 = random();
    const float32 w2 = random();

    const Quaternionf quaternion1(x1, y1, z1, w1);      //
    const Quaternionf quaternion2(x1, y1, z1, w1);      // same
    const Quaternionf quaternion3(x2, y2, z2, w2);
    
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
