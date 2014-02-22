#include "TestFramework/Interface/TestBase.h"
#include "Core/Math/Tests/Unittest/Helpers/MatrixHelper.h"
#include "Core/Math/Tests/Unittest/Helpers/VectorHelper.h"
#include "Core/Math/Tests/Unittest/Helpers/QuaternionHelper.h"
#include "Core/Math/Tests/Unittest/Helpers/MathHelper.h"
#include <EGEMath.h>
#include <EGEQuaternion.h>
#include <EGEMatrix.h>
#include <EGEVector2.h>
#include <EGEVector3.h>
#include <EGEVector4.h>
#include <EGEAngle.h>
#include <EGEAlignment.h>
#include <EGERect.h>

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
    std::vector<float32> convertQuaternion(const std::vector<float32>& quaternion) const;
    /*! Converts rotation represented by quaternion into axis and angle representatin.
     *  @param  axis        Rotation axis (Vector3).
     *  @param  angle       Rotation angle around axis (in radians).
     *  @param  quaternion  Quaternion data.
     *  @return Resulting 4x4 matrix.
     */
    void convertQuaternion(std::vector<float32>& axis, float32& angle, const std::vector<float32>& quaternion) const;
    /*! Creates matrix from given translation, scale and rotation.
     *  @param  translation Translation vector.
     *  @param  scale       Scale vector.
     *  @param  orientation Orientation quaternion.
     *  @return Resulting 4x4 matrix.
     */
    std::vector<float32> createMatrix(const std::vector<float32>& translation, const std::vector<float32>& scale,  
                                      const std::vector<float32>& orientation) const;
    /*! Creates quaternion from given rotation matix.
     *  @param  matrix  Rotation 4x4 matrix.
     *  @return Resulting quaternion.
     */
    std::vector<float32> createQuaternion(const std::vector<float32>& matrix) const;
    /*! Aligns given point with specified alignment around 'virtual frame' of a given size to new alignment.
      * @param  point     Point to be realigned.
      * @param  frameSize Size of the 'virtual frame' point is attached to.
      * @param  pointAlignment  Current alignment of the point with respect to 'virtual frame'.
      * @param  newAlignment    New aligment.
      * @return Calculated point.
      */
    std::vector<float32> alignPoint(const std::vector<float32>& point, const std::vector<float32>& frameSize, Alignment pointAlignment, 
                                    Alignment newAlignment) const;

    /*! Aligns rectangle with respect to another rectangle. 
     *  @param rect             Rectangle to align.
     *  @param otherRect        Another rectangle with respect to which first rectangle is to be aligned.
     *  @param currentAlignment Current rectangle alignment.
     *  @param newAlignment     New rectangle alignment.
     *  @return Returns realigned rectangle.
     */
    std::vector<float32> alignRect(const std::vector<float32>& rect, const std::vector<float32>& otherRect, Alignment currentAlignment, 
                                   Alignment newAlignment) const;
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
std::vector<float32> MathTest::convertQuaternion(const std::vector<float32>& quaternion) const
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
void MathTest::convertQuaternion(std::vector<float32>& axis, float32& angle, const std::vector<float32>& quaternion) const
{
  // calculate inverse length of imaginary axes
  const float32 invLength = 1.0f / (quaternion[0] * quaternion[0] + quaternion[1] * quaternion[1] + quaternion[2] * quaternion[2]);

  // check if length is greater than error thershold
  if (std::numeric_limits<float32>::epsilon() < invLength)
  {
    // calculate axes
    axis[0] = quaternion[0] * invLength;
    axis[1] = quaternion[1] * invLength;
    axis[2] = quaternion[2] * invLength;

    angle = 2 * acosf(quaternion[3]);
  }
  else
  {
    // length is 0 or errorous
    axis[0] = 0;
    axis[1] = 0;
    axis[2] = 0;

    angle = 0;
  }
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
  std::vector<float32> rotationMatrix = convertQuaternion(orientation);

  // (Pre) multiply translation with rotation
  std::vector<float32> matrixTR = MatrixHelper::Multiply(translationMatrix, rotationMatrix);

  // (Pre) multiply combined transltion-and-rotation with scale
  std::vector<float32> matrix = MatrixHelper::Multiply(matrixTR, scaleMatrix);

  return matrix;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<float32> MathTest::createQuaternion(const std::vector<float32>& matrix) const
{
  std::vector<float32> out = QuaternionHelper::Identity();

  float32 trace = matrix[0] + matrix[5] + matrix[10];
  float32 root;

  if (0 < trace)
  {
    // |w| > 1/2, may as well choose w > 1/2
    root = sqrtf(trace + 1);  // 2w

    out[3] = 0.5f * root;
    
    root = 0.5f / root;  // 1/(4w)
    
    out[0] = (matrix[9] - matrix[6]) * root;
    out[1] = (matrix[2] - matrix[8]) * root;
    out[2] = (matrix[4] - matrix[1]) * root;
  }
  else
  {
    static s32 KIndexing[3] = { 1, 2, 0 };

    // |w| <= 1/2
    s32 i = 0;
    if (matrix[5] > matrix[0])
    {
      i = 1;
    }

    if (matrix[10] > matrix[i * 4 + i])
    {
      i = 2;
    }

    s32 j = KIndexing[i];
    s32 k = KIndexing[j];

    root = Math::Sqrt(matrix[i * 4 + i]- matrix[j * 4 +  j] - matrix[k * 4 + k] + 1);

    float32* apkQuat[3] = { &out[0], &out[1], &out[2] };

    *apkQuat[i] = 0.5f * root;
    root = 0.5f / root;

    out[3] = (matrix[k * 4 + j] - matrix[j * 4 + k]) * root;
    
    *apkQuat[j] = (matrix[j * 4 + i] + matrix[i * 4 + j]) * root;
    *apkQuat[k] = (matrix[k * 4 + i] + matrix[i * 4 + k]) * root;
  }

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<float32> MathTest::alignPoint(const std::vector<float32>& point, const std::vector<float32>& frameSize, Alignment pointAlignment, 
                                          Alignment newAlignment) const
{
  EXPECT_GT(frameSize[0], 0);
  EXPECT_GT(frameSize[1], 0);

  std::vector<float32> out = point;

  // recalculate horizontal position of new point depending on requirements

  if (pointAlignment & ALIGN_LEFT)
  {
    if (newAlignment & ALIGN_HCENTER)
    {
      out[0] -= frameSize[0] * 0.5f;
    }
    else if (newAlignment & ALIGN_RIGHT)
    {
      out[0] -= frameSize[0];
    }
  }
  else if (pointAlignment & ALIGN_HCENTER)
  {
    if (newAlignment & ALIGN_LEFT)
    {
      out[0] += frameSize[0] * 0.5f;
    }
    else if (newAlignment & ALIGN_RIGHT)
    {
      out[0] -= frameSize[0] * 0.5f;
    }
  }
  else if (pointAlignment & ALIGN_RIGHT)
  {
    if (newAlignment & ALIGN_LEFT)
    {
      out[0] += frameSize[0];
    }
    else if (newAlignment & ALIGN_HCENTER)
    {
      out[0] += frameSize[0] * 0.5f;
    }
  }

  // recalculate vertical position of new point depending on requirements

  if (pointAlignment & ALIGN_TOP)
  {
    if (newAlignment & ALIGN_VCENTER)
    {
      out[1] -= frameSize[1] * 0.5f;
    }
    else if (newAlignment & ALIGN_BOTTOM)
    {
      out[1] -= frameSize[1];
    }
  }
  else if (pointAlignment & ALIGN_VCENTER)
  {
    if (newAlignment & ALIGN_TOP)
    {
      out[1] += frameSize[1] * 0.5f;
    }
    else if (newAlignment & ALIGN_BOTTOM)
    {
      out[1] -= frameSize[1] * 0.5f;
    }
  }
  else if (pointAlignment & ALIGN_BOTTOM)
  {
    if (newAlignment & ALIGN_TOP)
    {
      out[1] += frameSize[1];
    }
    else if (newAlignment & ALIGN_VCENTER)
    {
      out[1] += frameSize[1] * 0.5f;
    }
  }

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<float32> MathTest::alignRect(const std::vector<float32>& rect, const std::vector<float32>& otherRect, Alignment currentAlignment, 
                                         Alignment newAlignment) const
{
  EXPECT_GE(rect[2], 0);
  EXPECT_GE(rect[3], 0);
  EXPECT_GE(otherRect[2], 0);
  EXPECT_GE(otherRect[3], 0);

  std::vector<float32> out = rect;

  // align back to TOP-LEFT
  if (currentAlignment & ALIGN_RIGHT)
  {
    out[0] -= (otherRect[2] - rect[2]);
  }
  else if (currentAlignment & ALIGN_HCENTER)
  {
    out[0] -= (otherRect[2] - rect[2]) * 0.5f;
  }

  if (currentAlignment & ALIGN_BOTTOM)
  {
    out[1] -= (otherRect[3] - rect[3]);
  }
  else if (currentAlignment & ALIGN_VCENTER)
  {
    out[1] -= (otherRect[3] - rect[3]) * 0.5f;
  }

  // do new alignment
  if (newAlignment & ALIGN_RIGHT)
  {
    out[0] += (otherRect[2] - rect[2]);
  }
  else if (newAlignment & ALIGN_HCENTER)
  {
    out[0] += (otherRect[2] - rect[2]) * 0.5f;
  }

  if (newAlignment & ALIGN_BOTTOM)
  {
    out[1] += (otherRect[3] - rect[3]);
  }
  else if (newAlignment & ALIGN_VCENTER)
  {
    out[1] += (otherRect[3] - rect[3]) * 0.5f;
  }

  return out;
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
    matrixData = convertQuaternion(quaternionData);

    // calculate actual data
    const Quaternionf quaternion(quaternionData[0], quaternionData[1], quaternionData[2], quaternionData[3]);
    Matrix4f matrix;
    Math::Convert(matrix, quaternion);

    // compare
    EXPECT_TRUE(MatrixHelper::AreEqual(matrixData, matrix.data));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MathTest, ConvertQuaternionToAxisAndAngle)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const std::vector<float32> quaternionData = QuaternionHelper::RandomData();
    std::vector<float32> axisData = VectorHelper::RandomVector3Data();   
    float32 angle;

    // calculate reference values
    convertQuaternion(axisData, angle, quaternionData);

    // calculate actual data
    const Quaternionf quaternion(quaternionData[0], quaternionData[1], quaternionData[2], quaternionData[3]);

    Vector3f axis;
    Angle axisAngle;
    Math::Convert(axis, axisAngle, quaternion);

    // compare
    EXPECT_FLOAT_EQ(axisData[0], axis.x);
    EXPECT_FLOAT_EQ(axisData[1], axis.y);
    EXPECT_FLOAT_EQ(axisData[2], axis.z);
    EXPECT_FLOAT_EQ(axisAngle.radians(), angle);
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
TEST_F(MathTest, CreateQuaternion)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const std::vector<float32> matrixData = MatrixHelper::RandomMatrix4();

    // calculate reference value
    const std::vector<float32> quaternionData = createQuaternion(matrixData);

    // calculte actual value
    const Matrix4f matrix(matrixData[0], matrixData[1], matrixData[2], matrixData[3], matrixData[4], matrixData[5], matrixData[6], matrixData[7], matrixData[8], 
                          matrixData[9], matrixData[10], matrixData[11], matrixData[12], matrixData[13], matrixData[14], matrixData[15]);

    const Quaternionf quaternion = Math::CreateQuaternion(matrix);

    EXPECT_FLOAT_EQ(quaternionData[0], quaternion.x);
    EXPECT_FLOAT_EQ(quaternionData[1], quaternion.y);
    EXPECT_FLOAT_EQ(quaternionData[2], quaternion.z);
    EXPECT_FLOAT_EQ(quaternionData[3], quaternion.w);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MathTest, AlignPoint)
{
  // perform fixed number of random tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    std::vector<Alignment> verticalAlignments;
    verticalAlignments.push_back(ALIGN_TOP);
    verticalAlignments.push_back(ALIGN_VCENTER);
    verticalAlignments.push_back(ALIGN_BOTTOM);

    std::vector<Alignment> horizontalAlignments;
    horizontalAlignments.push_back(ALIGN_LEFT);
    horizontalAlignments.push_back(ALIGN_HCENTER);
    horizontalAlignments.push_back(ALIGN_RIGHT);

    const std::vector<float32> point = VectorHelper::RandomVector2Data(); 
    std::vector<float32> frameSize;
    frameSize.push_back(random() + 2.0f); 
    frameSize.push_back(random() + 2.0f); 

    const Alignment pointAlignment = verticalAlignments[rand() % (verticalAlignments.size() - 1)] | 
                                     horizontalAlignments[rand() % (horizontalAlignments.size() - 1)];
    const Alignment newAlignment   = verticalAlignments[rand() % (verticalAlignments.size() - 1)] | 
                                     horizontalAlignments[rand() % (horizontalAlignments.size() - 1)];
  
    // calculate reference value
    std::vector<float32> out = alignPoint(point, frameSize, pointAlignment, newAlignment);

    // calculate actual value
    const Vector2f point2(point[0], point[1]);
    const Vector2f frameSize2(frameSize[0], frameSize[1]);
    const Vector2f out2 = Math::Align(point2, frameSize2, pointAlignment, newAlignment);

    // test
    EXPECT_FLOAT_EQ(out[0], out2.x);
    EXPECT_FLOAT_EQ(out[1], out2.y);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MathTest, AlignRectangle)
{
  // perform fixed number of random tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    std::vector<Alignment> verticalAlignments;
    verticalAlignments.push_back(ALIGN_TOP);
    verticalAlignments.push_back(ALIGN_VCENTER);
    verticalAlignments.push_back(ALIGN_BOTTOM);

    std::vector<Alignment> horizontalAlignments;
    horizontalAlignments.push_back(ALIGN_LEFT);
    horizontalAlignments.push_back(ALIGN_HCENTER);
    horizontalAlignments.push_back(ALIGN_RIGHT);

    std::vector<float32> rect1;
    rect1.push_back(random());
    rect1.push_back(random());
    rect1.push_back(randomPositive());
    rect1.push_back(randomPositive());

    std::vector<float32> rect2;
    rect2.push_back(random());
    rect2.push_back(random());
    rect2.push_back(randomPositive());
    rect2.push_back(randomPositive());

    const Alignment pointAlignment = verticalAlignments[rand() % (verticalAlignments.size() - 1)] | 
                                     horizontalAlignments[rand() % (horizontalAlignments.size() - 1)];
    const Alignment newAlignment   = verticalAlignments[rand() % (verticalAlignments.size() - 1)] | 
                                     horizontalAlignments[rand() % (horizontalAlignments.size() - 1)];
  
    // calculate reference value
    std::vector<float32> out = alignRect(rect1, rect2, pointAlignment, newAlignment);

    // calculate actual value
    const Rectf rectangle1(rect1[0], rect1[1], rect1[2], rect1[3]);
    const Rectf rectangle2(rect2[0], rect2[1], rect2[2], rect2[3]);

    const Rectf out2 = Math::Align(rectangle1, rectangle2, pointAlignment, newAlignment);

    // test
    EXPECT_FLOAT_EQ(out[0], out2.x);
    EXPECT_FLOAT_EQ(out[1], out2.y);
    EXPECT_FLOAT_EQ(out[2], out2.width);
    EXPECT_FLOAT_EQ(out[3], out2.height);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MathTest, RandomDeviant2D)
{
  float32 angle;
  Vector2f vec;

  // fixed test for some obvious cases

  // +-125deg
  vec = Math::RandomDeviant(Angle::FromDegrees(125), Vector2f(0, 1));
  angle = Math::ACos(vec.normalized().dotProduct(Vector2f(0, 1)));
  EXPECT_LE(Math::RadiansToDegrees(Math::Abs(angle)), 125);

  vec = Math::RandomDeviant(Angle::FromDegrees(-125), Vector2f(1, 0));
  angle = Math::ACos(vec.normalized().dotProduct(Vector2f(1, 0)));
  EXPECT_LE(Math::RadiansToDegrees(Math::Abs(angle)), 125);

  // +-90deg
  vec = Math::RandomDeviant(Angle::FromDegrees(90), Vector2f(0, 1));
  angle = Math::ACos(vec.normalized().dotProduct(Vector2f(0, 1)));
  EXPECT_LE(Math::RadiansToDegrees(Math::Abs(angle)), 90);

  vec = Math::RandomDeviant(Angle::FromDegrees(-90), Vector2f(1, 0));
  angle = Math::ACos(vec.normalized().dotProduct(Vector2f(1, 0)));
  EXPECT_LE(Math::RadiansToDegrees(Math::Abs(angle)), 90);

  // +-45deg
  vec = Math::RandomDeviant(Angle::FromDegrees(-45), Vector2f(0, 1));
  angle = Math::ACos(vec.normalized().dotProduct(Vector2f(0, 1)));
  EXPECT_LE(Math::RadiansToDegrees(Math::Abs(angle)), 45);

  vec = Math::RandomDeviant(Angle::FromDegrees(-45), Vector2f(1, 0));
  angle = Math::ACos(vec.normalized().dotProduct(Vector2f(1, 0)));
  EXPECT_LE(Math::RadiansToDegrees(Math::Abs(angle)), 45);

  // +-0deg
  vec = Math::RandomDeviant(Angle::FromDegrees(0), Vector2f(0, 1));
  angle = Math::ACos(vec.normalized().dotProduct(Vector2f(0, 1)));
  EXPECT_LE(Math::RadiansToDegrees(Math::Abs(angle)), 0);

  vec = Math::RandomDeviant(Angle::FromDegrees(0), Vector2f(1, 0));
  angle = Math::ACos(vec.normalized().dotProduct(Vector2f(1, 0)));
  EXPECT_LE(Math::RadiansToDegrees(Math::Abs(angle)), 0);

  // perform fixed number of random tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const Angle angle(Math::DegreesToRadians(random(90.0f)));
    const Vector2f vector(random(), random());

    // calculate
    const Vector2f out = Math::RandomDeviant(angle, vector);

    // test
    const float32 radians = Math::ACos(out.normalized().dotProduct(vector.normalized()));
    EXPECT_LE(Math::RadiansToDegrees(radians), Math::Abs(angle.degrees()));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MathTest, RandomDeviant3DNoUp)
{
  float32 angle;
  Vector3f vec;

  // +-0deg
  vec = Math::RandomDeviant(Angle::FromDegrees(0), Vector3f(0, 1, 0));
  angle = Math::ACos(vec.normalized().dotProduct(Vector3f(0, 1, 0)));
  EXPECT_LE(Math::RadiansToDegrees(Math::Abs(angle)), 0);

  vec = Math::RandomDeviant(Angle::FromDegrees(0), Vector3f(1, 0, 0));
  angle = Math::ACos(vec.normalized().dotProduct(Vector3f(1, 0, 0)));
  EXPECT_LE(Math::RadiansToDegrees(Math::Abs(angle)), 0);

  vec = Math::RandomDeviant(Angle::FromDegrees(0), Vector3f(0, 0, 1));
  angle = Math::ACos(vec.normalized().dotProduct(Vector3f(0, 0, 1)));
  EXPECT_LE(Math::RadiansToDegrees(Math::Abs(angle)), 0);

  // perform fixed number of random tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const Angle angle(Math::DegreesToRadians(random(90.0f)));
    Vector3f vector(random(), random(), random());
    vector.normalize();

    // calculate
    const Vector3f out = Math::RandomDeviant(angle, vector);

    // test
    const float32 radians = Math::ACos(out.normalized().dotProduct(vector.normalized()));

    // NOTE: do comparison as integers as comparing floating point numbers is tricky here ie no EXPECT_FLOAT_LE etc. so it is possible expected value is just
    //       a bit greater than the other ie 37.9830 and 37.98302...
    EXPECT_LE(static_cast<int>(Math::RadiansToDegrees(radians)), static_cast<int>(Math::Abs(angle.degrees())));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MathTest, RandomDeviant3D)
{
  float32 angle;
  Vector3f vec;

  // +-0deg
  vec = Math::RandomDeviant(Angle::FromDegrees(0), Vector3f(0, 1, 0), &Vector3f::UNIT_X);
  angle = Math::ACos(vec.normalized().dotProduct(Vector3f(0, 1, 0)));
  EXPECT_LE(Math::RadiansToDegrees(Math::Abs(angle)), 0);

  vec = Math::RandomDeviant(Angle::FromDegrees(0), Vector3f(1, 0, 0) , &Vector3f::UNIT_Y);
  angle = Math::ACos(vec.normalized().dotProduct(Vector3f(1, 0, 0)));
  EXPECT_LE(Math::RadiansToDegrees(Math::Abs(angle)), 0);

  vec = Math::RandomDeviant(Angle::FromDegrees(0), Vector3f(0, 0, 1), &Vector3f::UNIT_Z);
  angle = Math::ACos(vec.normalized().dotProduct(Vector3f(0, 0, 1)));
  EXPECT_LE(Math::RadiansToDegrees(Math::Abs(angle)), 0);

  // perform fixed number of random tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const Angle angle(Math::DegreesToRadians(random(90.0f)));
    Vector3f vector(random(), random(), random());
    vector.normalize();
    
    // generate up vector
    const Vector3f up = vector.perpendicular().normalized();

    // calculate
    const Vector3f out = Math::RandomDeviant(angle, vector, &up);

    // test
    const float32 radians = Math::ACos(out.normalized().dotProduct(vector.normalized()));

    // NOTE: do comparison as integers as comparing floating point numbers is tricky here ie no EXPECT_FLOAT_LE etc. so it is possible expected value is just
    //       a bit greater than the other ie 37.9830 and 37.98302...
    EXPECT_LE(static_cast<int>(Math::RadiansToDegrees(radians)), static_cast<int>(Math::Abs(angle.degrees())));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MathTest, ZeroRoundOff)
{
  EXPECT_EQ(0, Math::ZeroRoundOff(0.0f));
  EXPECT_EQ(1, Math::ZeroRoundOff(1.0f));
  EXPECT_EQ(-1, Math::ZeroRoundOff(-1.0f));
  EXPECT_NE(0, Math::ZeroRoundOff(Math::EPSILON));
  EXPECT_NE(0, Math::ZeroRoundOff(-Math::EPSILON));
  EXPECT_EQ(0, Math::ZeroRoundOff(Math::EPSILON * 0.9f));
  EXPECT_EQ(0, Math::ZeroRoundOff(-Math::EPSILON * 0.9f));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
