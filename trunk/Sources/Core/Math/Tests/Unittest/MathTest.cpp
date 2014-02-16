#include "TestFramework/Interface/TestBase.h"
#include "Core/Math/Tests/Unittest/Helpers/MatrixHelper.h"
#include "Core/Math/Tests/Unittest/Helpers/VectorHelper.h"
#include "Core/Math/Tests/Unittest/Helpers/QuaternionHelper.h"
#include "Core/Math/Tests/Unittest/Helpers/MathHelper.h"
#include <EGEMath.h>
#include <EGEQuaternion.h>
#include <EGEMatrix.h>
#include <EGEVector2.h>
#include <EGEAngle.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const int KRepetitionsCount = 20;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class MathTest : public TestBase
{
  protected:

    MathTest();

    /*! Clamps given value within specified interval.
     *  @param  value Value to process.
     *  @param  min   Minimum allowed value.
     *  @param  max   Maximum allowed value.
     */
    float32 clamp(float32 value, float32 min, float32 max) const;
    /*! Converts rotation represented by quaternion into matrix representation.
     *  @param  quaternion  Quaternion data.
     *  @return Resulting 4x4 matrix.
     */
    std::vector<float32> convert(const std::vector<float32>& quaternion) const;
    /*! Creates matrix from given translation, scale and rotation.
     *  @param  translation Translation vector.
     *  @param  scale       Scale vector.
     *  @param  orientation Orientation quaternion.
     *  @return Resulting 4x4 matrix.
     */
    std::vector<float32> createMatrix(const std::vector<float32>& translation, const std::vector<float32>& scale,  
                                      const std::vector<float32>& orientation) const;
    /*! Calculates angle (in radians) between vector and positive X axis.
     *  @param  direction 2D direction vector.
     *  @return Calculated angle.
     */
    float32 angle(const std::vector<float32>& direction) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MathTest::MathTest() : TestBase(0.0001f)
{
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
std::vector<float32> MathTest::convert(const std::vector<float32>& quaternion) const
{
  std::vector<float32> matrix = MatrixHelper::Identity();

  // 1st column
	matrix[0] = 1.0f - 2.0f * (quaternion[1] * quaternion[1] + quaternion[2] * quaternion[2]); 
	matrix[1] = 2.0f * (quaternion[0] * quaternion[1] - quaternion[2] * quaternion[3]);
	matrix[2] = 2.0f * (quaternion[0] * quaternion[2] + quaternion[1] * quaternion[3]);
	matrix[3] = 0;

	// 2nd column
	matrix[4] = 2.0f * (quaternion[0] * quaternion[1] + quaternion[2] * quaternion[3]);  
	matrix[5] = 1.0f - 2.0f * (quaternion[0] * quaternion[0] + quaternion[2] * quaternion[2]); 
	matrix[6] = 2.0f * (quaternion[2] * quaternion[1] - quaternion[0] * quaternion[3]);  
	matrix[7] = 0;

	// 3rd column
	matrix[8]  = 2.0f * (quaternion[0] * quaternion[2] - quaternion[1] * quaternion[3]);
	matrix[9]  = 2.0f * (quaternion[1] * quaternion[2] + quaternion[0] * quaternion[3]);
	matrix[10] = 1.0f - 2.0f *(quaternion[0] * quaternion[0] + quaternion[1] * quaternion[1]);  
	matrix[11] = 0;

	// 4th column
	matrix[12] = 0;
	matrix[13] = 0;
	matrix[14] = 0;  
	matrix[15] = 1;

  return matrix;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<float32> MathTest::createMatrix(const std::vector<float32>& translation, const std::vector<float32>& scale, 
                                            const std::vector<float32>& orientation) const
{
  // Ordering: Final = S * R * T

  // make translation matrix data
  std::vector<float32> translationMatrix = MatrixHelper::Identity();
  translationMatrix[12] = translation[0];
  translationMatrix[13] = translation[1];
  translationMatrix[14] = translation[2];
  
  // make scale matrix data
  std::vector<float32> scaleMatrix = MatrixHelper::Identity();
  scaleMatrix[0]  = scale[0];
  scaleMatrix[5]  = scale[1];
  scaleMatrix[10] = scale[2];

  // convert quaternion into rotation matrix
  std::vector<float32> rotationMatrix = convert(orientation);

  // (Pre) multiply translation with rotation
  std::vector<float32> matrixTR = MatrixHelper::Multiply(translationMatrix, rotationMatrix);

  // (Pre) multiply combined transltion-and-rotation with scale
  std::vector<float32> matrix = MatrixHelper::Multiply(matrixTR, scaleMatrix);

  return matrix;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 MathTest::angle(const std::vector<float32>& direction) const
{
  EXPECT_EQ(2, direction.size());

  return atan2(direction[1], direction[0]);
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
    std::vector<float32> quaternionData;   
    std::vector<float32> matrixData;   

    // randomize quaternion
    quaternionData = QuaternionHelper::RandomData();
    
    // calculate reference values
    matrixData = convert(quaternionData);

    // calculate actual data
    const Quaternionf quaternion(quaternionData[0], quaternionData[1], quaternionData[2], quaternionData[3]);
    Matrix4f matrix;
    Math::Convert(matrix, quaternion);

    // compare
    EXPECT_TRUE(MatrixHelper::AreEqual(matrixData, matrix.data));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MathTest, TransformVector)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    std::vector<float32> vectorData; 
    std::vector<float32> vectorDataOut; 
    std::vector<float32> matrixData;   

    // randomize vector and matrix
    vectorData = VectorHelper::RandomVector4Data();
    matrixData = MatrixHelper::RandomMatrix4();

    // calculate reference values
    vectorDataOut = MathHelper::MultiplyVector(matrixData, vectorData);

    // test methods...
    Vector4f vector = Math::Transform(Vector4f(vectorData[0], vectorData[1], vectorData[2], vectorData[3]), Matrix4f(&matrixData[0]));
    std::vector<float32> vectorOut;
    vectorOut.push_back(vector.x);
    vectorOut.push_back(vector.y);
    vectorOut.push_back(vector.z);
    vectorOut.push_back(vector.w);
    EXPECT_TRUE(VectorHelper::AreEqual(vectorDataOut, vectorOut));

    // ...and operators
    vector =  Matrix4f(&matrixData[0]) * Vector4f(vectorData[0], vectorData[1], vectorData[2], vectorData[3]);
    vectorOut.clear();
    vectorOut.push_back(vector.x);
    vectorOut.push_back(vector.y);
    vectorOut.push_back(vector.z);
    vectorOut.push_back(vector.w);
    EXPECT_TRUE(VectorHelper::AreEqual(vectorDataOut, vectorOut));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MathTest, CreateMatrix)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    std::vector<float32> translationData; 
    std::vector<float32> scaleData; 
    std::vector<float32> orientationData;   
    std::vector<float32> matrixData;

    // randomize vector and matrix
    translationData = VectorHelper::RandomVector4Data();
    scaleData       = VectorHelper::RandomVector4Data();
    orientationData = QuaternionHelper::RandomData();

    // calculate reference values
    matrixData = createMatrix(translationData, scaleData, orientationData);

    // calculate actual value
    const Matrix4f matrix = Math::CreateMatrix(Vector4f(translationData[0], translationData[1], translationData[2], translationData[3]),
                                               Vector4f(scaleData[0], scaleData[1], scaleData[2], scaleData[3]),
                                               Quaternionf(orientationData[0], orientationData[1], orientationData[2], orientationData[3]));
    EXPECT_TRUE(MatrixHelper::AreEqual(matrixData, matrix.data));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MathTest, GetAngle)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    std::vector<float32> direction = VectorHelper::RandomVector2Data();

    // make sure not zero vector
    if ((0 == direction[0]) && (0 == direction[1]))
    {
      direction[0] += 0.1f;
    }

    EXPECT_FLOAT_EQ(angle(direction), Math::GetAngle(Vector2f(direction[0], direction[1])).radians());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
