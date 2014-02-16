#include "TestFramework/Interface/TestBase.h"
#include "Core/Math/Tests/Unittest/Helpers/MatrixHelper.h"
#include <EGEMath.h>
#include <EGEQuaternion.h>
#include <EGEMatrix.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const int KRepetitionsCount = 20;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class MathTest : public TestBase
{
  protected:

    MathTest();

    /*! Generates random quaternion data. 
     *  @param  data  Array of data to be randomized.
     */
    void randomQuaternion(float32 data[4]) const;
    /*! Clamps given value within specified interval.
     *  @param  value Value to process.
     *  @param  min   Minimum allowed value.
     *  @param  max   Maximum allowed value.
     */
    float32 clamp(float32 value, float32 min, float32 max) const;
    /*! Converts rotation represented by quaternion into matrix representation.
     *  @param  data        Matrix data.
     *  @param  quaternion  Quaternion data.
     */
    void convert(float32 data[16], float32 quaternion[4]) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MathTest::MathTest() : TestBase(0.0001f)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MathTest::randomQuaternion(float32 data[4]) const
{
  for (int i = 0; i < 4; ++i)
  {
    data[i] = random();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 MathTest::clamp(float32 value, float32 min, float32 max) const
{
  float32 result = value;

  if (value < min)
  {
    result = min;
  }
  else if (value > max)
  {
    result = max;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MathTest::convert(float32 data[16], float32 quaternion[4]) const
{
  // 1st column
	data[0] = 1.0f - 2.0f * (quaternion[1] * quaternion[1] + quaternion[2] * quaternion[2]); 
	data[1] = 2.0f * (quaternion[0] * quaternion[1] - quaternion[2] * quaternion[3]);
	data[2] = 2.0f * (quaternion[0] * quaternion[2] + quaternion[1] * quaternion[3]);
	data[3] = 0;

	// 2nd column
	data[4] = 2.0f * (quaternion[0] * quaternion[1] + quaternion[2] * quaternion[3]);  
	data[5] = 1.0f - 2.0f * (quaternion[0] * quaternion[0] + quaternion[2] * quaternion[2]); 
	data[6] = 2.0f * (quaternion[2] * quaternion[1] - quaternion[0] * quaternion[3]);  
	data[7] = 0;

	// 3rd column
	data[8]  = 2.0f * (quaternion[0] * quaternion[2] - quaternion[1] * quaternion[3]);
	data[9]  = 2.0f * (quaternion[1] * quaternion[2] + quaternion[0] * quaternion[3]);
	data[10] = 1.0f - 2.0f *(quaternion[0] * quaternion[0] + quaternion[1] * quaternion[1]);  
	data[11] = 0;

	// 4th column
	data[12] = 0;
	data[13] = 0;
	data[14] = 0;  
	data[15] = 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MathTest, Min)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 value1 = random();
    const float32 value2 = random();

    EXPECT_FLOAT_EQ(std::min(value1, value2), Math::Min(value1, value2));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MathTest, Max)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 value1 = random();
    const float32 value2 = random();

    EXPECT_FLOAT_EQ(std::max(value1, value2), Math::Max(value1, value2));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MathTest, Clamp)
{
  const float32 scale = 1.0f;

  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 min   = random();                   // [-1, 1]
    const float32 max   = min + random() + scale;     // [0, 2]
    const float32 value = random(scale * 3.0f);       // [-3, 3]

    EXPECT_FLOAT_EQ(clamp(value, min, max), Math::Clamp(value, min, max));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MathTest, Sqrt)
{
  const float32 scale = 1000.0f;

  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 value = random(scale) + scale;

    EXPECT_FLOAT_EQ(sqrt(value), Math::Sqrt(value));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MathTest, Pow)
{
  const float32 powerScale = 5.0f;

  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 value = random();
    float32 power       = random(powerScale) + powerScale;

    // make sure power value is an integer in case of negative value
    if (0 > value)
    {
      power = floorf(power);
    }

    EXPECT_FLOAT_EQ(pow(value, power), Math::Pow(value, power));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MathTest, Floor)
{
  const float32 scale = 100.0f;

  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 value = random(scale);

    EXPECT_EQ(floor(value), Math::Floor(value));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MathTest, Ceil)
{
  const float32 scale = 100.0f;

  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 value = random(scale);

    EXPECT_EQ(ceil(value), Math::Ceil(value));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MathTest, Absolute)
{
  const float32 scale = 100.0f;

  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 value     = random(scale);
    const s32 integerValue  = Math::Floor(value);

    EXPECT_EQ(abs(value), Math::Abs(value));
    EXPECT_EQ(abs(integerValue), Math::Abs(integerValue));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MathTest, RadiansToDegrees)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 value = random();

    EXPECT_FLOAT_EQ(value * 57.29577951f, Math::RadiansToDegrees(value));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MathTest, DegreesToRadians)
{
  const float32 scale = 180.0f;

  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 value = random(scale);

    EXPECT_FLOAT_EQ(value * 0.017453292f, Math::DegreesToRadians(value));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MathTest, Sine)
{
  const float32 scale = Math::TWO_PI;

  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 value = random(scale);

    EXPECT_FLOAT_EQ(sin(value), Math::Sin(value));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MathTest, Cosine)
{
  const float32 scale = Math::TWO_PI;

  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 value = random(scale);

    EXPECT_FLOAT_EQ(cos(value), Math::Cos(value));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MathTest, ArcCosine)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    float32 value = random();   

    EXPECT_FLOAT_EQ(acos(value), Math::ACos(value));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MathTest, Tangent)
{
  const float32 scale = Math::PI_HALF;

  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 value = random(scale);

    EXPECT_FLOAT_EQ(tan(value), Math::Tan(value));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MathTest, Cotangent)
{
  const float32 scale = Math::PI;

  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    float32 value = random(scale);   

    // Do not allow zero
    if (0.0f == value)
    {
      value = 0.1f;
    }

    EXPECT_FLOAT_EQ(1.0f / tan(value), Math::Ctg(value));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MathTest, ArcTangent2)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    float32 x = random();   
    float32 y = random();   

    // Do not allow both to be 0
    if ((0.0f == x) && (0.0f == y))
    {
      // make one nonzero
      x = 0.1f;
    }

    EXPECT_FLOAT_EQ(atan2(y, x), Math::ATan2(y, x));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MathTest, ConvertQuaternionToMatrix)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    float32 quaternionData[4];   
    float32 matrixData[16];   

    // randomize quaternion
    randomQuaternion(quaternionData);
    
    // calculate reference values
    convert(matrixData, quaternionData);

    // calculate actual data
    const Quaternionf quaternion(quaternionData[0], quaternionData[1], quaternionData[2], quaternionData[3]);
    Matrix4f matrix;
    Math::Convert(matrix, quaternion);

    // compare
    EXPECT_TRUE(MatrixHelper::AreEqual(matrixData, matrix.data));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
