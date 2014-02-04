#include <gtest/gtest.h>
#include <math.h>
#include <stdlib.h>
#include <limits>
#include <EGEVector.h>

/** Tests are focusing TVector4<float32> instantiations. */

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const int KRepetitionsCount = 20;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Vector4Test : public ::testing::Test
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
    /*! Normalized given vector data. 
     *  @param  x     Vector X value.
     *  @param  y     Vector Y value.
     *  @param  z     Vector Z value.
     *  @param  w     Vector W value.
     *  @param  outX  Normalized vector X value.
     *  @param  outY  Normalized vector Y value.
     *  @param  outZ  Normalized vector Z value.
     *  @param  outW  Normalized vector W value.
     */
    void normalize(float32 x, float32 y, float32 z, float32 w, float32& outX, float32& outY, float32& outZ, float32& outW) const;
    /*! Returns length of the given vector data.
     *  @param  x     Vector X value.
     *  @param  y     Vector Y value.
     *  @param  z     Vector Z value.
     *  @param  w     Vector W value.
     *  @return Length of the vector.
     */ 
    float32 length(float32 x, float32 y, float32 z, float32 w) const;
    /*! Returns squared length of the given vector data.
     *  @param  x     Vector X value.
     *  @param  y     Vector Y value.
     *  @param  z     Vector Z value.
     *  @param  w     Vector W value.
     *  @return Squared length of the vector.
     */ 
    float32 lengthSquared(float32 x, float32 y, float32 z, float32 w) const;
    /*! Calculates dot product between vectors. 
     *  @param  x1  Vector 1 X value.
     *  @param  y1  Vector 1 Y value.
     *  @param  z1  Vector 1 Z value.
     *  @param  w1  Vector 1 W value.
     *  @param  x2  Vector 2 X value.
     *  @param  y2  Vector 2 Y value.
     *  @param  z2  Vector 2 Z value.
     *  @param  w2  Vector 2 W value.
     *  @return Dot product between two vectors.
     */
    float32 dotProduct(float32 x1, float32 y1, float32 z1, float32 w1, float32 x2, float32 y2, float32 z2, float32 w2) const;
    /*! Calculates distance between vectors. 
     *  @param  x1  Vector 1 X value.
     *  @param  y1  Vector 1 Y value.
     *  @param  z1  Vector 1 Z value.
     *  @param  w1  Vector 1 W value.
     *  @param  x2  Vector 2 X value.
     *  @param  y2  Vector 2 Y value.
     *  @param  z2  Vector 2 Z value.
     *  @param  w2  Vector 2 W value.
     *  @return Distance between two vectors.
     */
    float32 distance(float32 x1, float32 y1, float32 z1, float32 w1, float32 x2, float32 y2, float32 z2, float32 w2) const;
    /*! Calculates squared distance between vectors. 
     *  @param  x1  Vector 1 X value.
     *  @param  y1  Vector 1 Y value.
     *  @param  z1  Vector 1 Z value.
     *  @param  w1  Vector 1 W value.
     *  @param  x2  Vector 2 X value.
     *  @param  y2  Vector 2 Y value.
     *  @param  z2  Vector 2 Z value.
     *  @param  w2  Vector 2 W value.
     *  @return Squared distance between two vectors.
     */
    float32 distanceSquare(float32 x1, float32 y1, float32 z1, float32 w1, float32 x2, float32 y2, float32 z2, float32 w2) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Vector4Test::SetUpTestCase()
{
  srand(static_cast<unsigned int>(time(NULL)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Vector4Test::TearDownTestCase()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Vector4Test::SetUp()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Vector4Test::TearDown()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 Vector4Test::random(float32 scale) const
{
  return (rand() / static_cast<float32>(RAND_MAX) - 0.5f) * 2.0f * scale;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Vector4Test::normalize(float32 x, float32 y, float32 z, float32 w, float32& outX, float32& outY, float32& outZ, float32& outW) const
{
  outX = x;
  outY = y;
  outZ = z;
  outW = w;

  // get length
  float32 len = length(x, y, z, w);

  if (std::numeric_limits<float32>::epsilon() <= len)
  {
    // get inverse of length
    float32 invLength = 1.0f / len;

    // normalize
	  outX *= invLength;
	  outY *= invLength;
    outZ *= invLength;
    outW = 1.0f;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 Vector4Test::length(float32 x, float32 y, float32 z, float w) const
{
  return sqrtf((x * x) + (y * y) + (z * z) + (w * w));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 Vector4Test::lengthSquared(float32 x, float32 y, float32 z, float w) const
{
  return (x * x) + (y * y) + (z * z) + (w * w);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 Vector4Test::dotProduct(float32 x1, float32 y1, float32 z1, float32 w1, float32 x2, float32 y2, float32 z2, float w2) const
{
  return (x1 * x2) + (y1 * y2) + (z1 * z2) + (w1 * w2); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 Vector4Test::distance(float32 x1, float32 y1, float32 z1, float32 w1, float32 x2, float32 y2, float32 z2, float32 w2) const
{
  return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2) + (w1 - w2) * (w1 - w2));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 Vector4Test::distanceSquare(float32 x1, float32 y1, float32 z1, float32 w1, float32 x2, float32 y2, float32 z2, float32 w2) const
{
  return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2) + (w1 - w2) * (w1 - w2);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Vector4Test, SetValue)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x = random();
    const float32 y = random();
    const float32 z = random();
    const float32 w = random();

    // setting via constructors
    const Vector4f vector(x, y, z, w);
    EXPECT_FLOAT_EQ(vector.x, x);
    EXPECT_FLOAT_EQ(vector.y, y);
    EXPECT_FLOAT_EQ(vector.z, z);
    EXPECT_FLOAT_EQ(vector.w, w);

    const Vector4f vector2(vector);
    EXPECT_FLOAT_EQ(vector2.x, vector.x);
    EXPECT_FLOAT_EQ(vector2.y, vector.y);
    EXPECT_FLOAT_EQ(vector2.z, vector.z);
    EXPECT_FLOAT_EQ(vector2.w, vector.w);

    // setting by setters
    Vector4f vector3;
    vector3.set(x, y, z, w);
    EXPECT_FLOAT_EQ(vector3.x, x);
    EXPECT_FLOAT_EQ(vector3.y, y);
    EXPECT_FLOAT_EQ(vector3.z, z);
    EXPECT_FLOAT_EQ(vector3.w, w);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Vector4Test, Equality)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x = random();
    const float32 y = random();
    const float32 z = random();
    const float32 w = random();

    const Vector4f vector1(x, y, z, w);
    const Vector4f vector2(x, y, z, w);
    
    EXPECT_TRUE(vector1 == vector2);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Vector4Test, Addition)
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

    const Vector4f vector1(x1, y1, z1, w1);
    const Vector4f vector2(x2, y2, z2, w2);
    
    // compare v1 + v2
    EXPECT_FLOAT_EQ((vector1 + vector2).x, x1 + x2);
    EXPECT_FLOAT_EQ((vector1 + vector2).y, y1 + y2);
    EXPECT_FLOAT_EQ((vector1 + vector2).z, z1 + z2);
    EXPECT_FLOAT_EQ((vector1 + vector2).w, w1 + w2);

    // compare v2 + v1
    EXPECT_FLOAT_EQ((vector2 + vector1).x, x2 + x1);
    EXPECT_FLOAT_EQ((vector2 + vector1).y, y2 + y1);
    EXPECT_FLOAT_EQ((vector2 + vector1).z, z2 + z1);
    EXPECT_FLOAT_EQ((vector2 + vector1).w, w2 + w1);

    // compare v1 + v2 and v2 + v1
    EXPECT_FLOAT_EQ((vector1 + vector2).x, (vector2 + vector1).x);
    EXPECT_FLOAT_EQ((vector1 + vector2).y, (vector2 + vector1).y);
    EXPECT_FLOAT_EQ((vector1 + vector2).z, (vector2 + vector1).z);
    EXPECT_FLOAT_EQ((vector1 + vector2).w, (vector2 + vector1).w);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Vector4Test, Difference)
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

    const Vector4f vector1(x1, y1, z1, w1);
    const Vector4f vector2(x2, y2, z2, w2);
    
    // compare v1 - v2
    EXPECT_FLOAT_EQ((vector1 - vector2).x, x1 - x2);
    EXPECT_FLOAT_EQ((vector1 - vector2).y, y1 - y2);
    EXPECT_FLOAT_EQ((vector1 - vector2).z, z1 - z2);
    EXPECT_FLOAT_EQ((vector1 - vector2).w, w1 - w2);

    // compare v2 - v1
    EXPECT_FLOAT_EQ((vector2 - vector1).x, x2 - x1);
    EXPECT_FLOAT_EQ((vector2 - vector1).y, y2 - y1);
    EXPECT_FLOAT_EQ((vector2 - vector1).z, z2 - z1);
    EXPECT_FLOAT_EQ((vector2 - vector1).w, w2 - w1);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
