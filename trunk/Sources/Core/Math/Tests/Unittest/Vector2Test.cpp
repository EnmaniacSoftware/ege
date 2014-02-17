#include "TestFramework/Interface/TestBase.h"
#include <EGEVector2.h>
#include <limits>

/** Tests are focusing TVector2<float32> instantiations. */

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const int KRepetitionsCount = 20;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Vector2Test : public TestBase
{
  protected:

    /*! Normalized given vector data. 
     *  @param  x     Vector X value.
     *  @param  y     Vector Y value.
     *  @param  outX  Normalized vector's X value.
     *  @param  outY  Normalized vector's Y value.
     */
    void normalize(float32 x, float32 y, float32& outX, float32& outY) const;
    /*! Returns length of the given vector data.
     *  @param  x     Vector X value.
     *  @param  y     Vector Y value.
     *  @return Length of the vector.
     */ 
    float32 length(float32 x, float32 y) const;
    /*! Returns squared length of the given vector data.
     *  @param  x     Vector X value.
     *  @param  y     Vector Y value.
     *  @return Squared length of the vector.
     */ 
    float32 lengthSquared(float32 x, float32 y) const;
    /*! Calculates dot product between vectors. 
     *  @param  x1  Vector 1 X value.
     *  @param  y1  Vector 1 Y value.
     *  @param  x2  Vector 2 X value.
     *  @param  y2  Vector 2 Y value.
     *  @return Dot product between two vectors.
     */
    float32 dotProduct(float32 x1, float32 y1, float32 x2, float32 y2) const;
    /*! Calculates distance between vectors. 
     *  @param  x1  Vector 1 X value.
     *  @param  y1  Vector 1 Y value.
     *  @param  x2  Vector 2 X value.
     *  @param  y2  Vector 2 Y value.
     *  @return Distance between two vectors.
     */
    float32 distance(float32 x1, float32 y1, float32 x2, float32 y2) const;
    /*! Calculates squared distance between vectors. 
     *  @param  x1  Vector 1 X value.
     *  @param  y1  Vector 1 Y value.
     *  @param  x2  Vector 2 X value.
     *  @param  y2  Vector 2 Y value.
     *  @return Squared distance between two vectors.
     */
    float32 distanceSquare(float32 x1, float32 y1, float32 x2, float32 y2) const;
    /*! Calculates perpendicular vector of a given one.
     *  @param  x     Vector X value.
     *  @param  y     Vector Y value.
     *  @param  outX  Perpendicular vector X value.
     *  @param  outY  Perpendicular vector Y value.
     */
    void perpendicular(float32 x, float32 y, float32& outX, float32& outY) const;
    /*! Calculates vector resulting from lerping between other two.
     *  @param  outX      Resulting vector X value.
     *  @param  outY      Resulting vector Y value.
     *  @param  x1        Vector 1 X value.
     *  @param  y1        Vector 1 Y value.
     *  @param  x2        Vector 2 X value.
     *  @param  y2        Vector 2 Y value.
     *  @param  parameter Scalar in [0-1] range describing relative progress of interpolation.
     */
    void lerp(float32& outX, float32& outY, float32 x1, float32 y1, float32 x2, float32 y2, float32 parameter) const;    
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Vector2Test::normalize(float32 x, float32 y, float32& outX, float32& outY) const
{
  outX = x;
  outY = y;

  // get length
  float32 len = length(x, y);

  if (std::numeric_limits<float32>::epsilon() <= len)
  {
    // get inverse of length
    float32 invLength = 1.0f / len;

    // normalize
	  outX *= invLength;
	  outY *= invLength;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 Vector2Test::length(float32 x, float32 y) const
{
  return sqrtf((x * x) + (y * y));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 Vector2Test::lengthSquared(float32 x, float32 y) const
{
  return (x * x) + (y * y);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 Vector2Test::dotProduct(float32 x1, float32 y1, float32 x2, float32 y2) const
{
  return (x1 * x2) + (y1 * y2); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 Vector2Test::distance(float32 x1, float32 y1, float32 x2, float32 y2) const
{
  return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 Vector2Test::distanceSquare(float32 x1, float32 y1, float32 x2, float32 y2) const
{
  return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Vector2Test::perpendicular(float32 x, float32 y, float32& outX, float32& outY) const
{
  outX = y;
  outY = -x;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Vector2Test::lerp(float32& outX, float32& outY, float32 x1, float32 y1, float32 x2, float32 y2, float32 parameter) const
{
  outX = x1 * (1.0f - parameter) + x2 * parameter;
  outY = y1 * (1.0f - parameter) + y2 * parameter;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Vector2Test, SetValue)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x = random();
    const float32 y = random();

    // setting via constructors
    const Vector2f vector(x, y);
    EXPECT_FLOAT_EQ(vector.x, x);
    EXPECT_FLOAT_EQ(vector.y, y);

    const Vector2f vector2(vector);
    EXPECT_FLOAT_EQ(vector2.x, vector.x);
    EXPECT_FLOAT_EQ(vector2.y, vector.y);

    // setting by setters
    Vector2f vector3;
    vector3.set(x, y);
    EXPECT_FLOAT_EQ(vector3.x, x);
    EXPECT_FLOAT_EQ(vector3.y, y);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Vector2Test, Length)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x = random();
    const float32 y = random();

    Vector2f vector(x, y);
    
    // compare
    EXPECT_FLOAT_EQ(vector.length(), length(x, y));
    EXPECT_FLOAT_EQ(vector.lengthSquared(), lengthSquared(x, y));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Vector2Test, Normalize)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x = random();
    const float32 y = random();

    Vector2f vector(x, y);
    
    // get normalized vector first
    Vector2f normalized = vector.normalized();

    // normalize
    vector.normalize();

    // calculate reference normalization
    float32 outX;
    float32 outY;
    normalize(x, y, outX, outY);

    // compare
    EXPECT_FLOAT_EQ(vector.x, outX);
    EXPECT_FLOAT_EQ(vector.y, outY);
    EXPECT_FLOAT_EQ(normalized.x, outX);
    EXPECT_FLOAT_EQ(normalized.y, outY);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Vector2Test, DotProduct)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x1 = random();
    const float32 y1 = random();
    const float32 x2 = random();
    const float32 y2 = random();

    const Vector2f vector1(x1, y1);
    const Vector2f vector2(x2, y2);
     
    // compare dot product between v1 and v2
    EXPECT_FLOAT_EQ(vector1.dotProduct(vector2), dotProduct(x1, y1, x2, y2));

    // compare dot product between v2 and v1
    EXPECT_FLOAT_EQ(vector2.dotProduct(vector1), dotProduct(x2, y2, x1, y1));

    // compare dot products of v1 * v2 and v2 * v1
    EXPECT_FLOAT_EQ(vector2.dotProduct(vector1), vector1.dotProduct(vector2));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Vector2Test, Distance)
{
  const float32 KVectorScale = 100.0f;

  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x1 = random(KVectorScale);
    const float32 y1 = random(KVectorScale);
    const float32 x2 = random(KVectorScale);
    const float32 y2 = random(KVectorScale);

    const Vector2f vector1(x1, y1);
    const Vector2f vector2(x2, y2);
     
    // compare distance between v1 and v2
    EXPECT_FLOAT_EQ(vector1.distanceTo(vector2), distance(x1, y1, x2, y2));
    EXPECT_FLOAT_EQ(vector1.distanceSquaredTo(vector2), distanceSquare(x1, y1, x2, y2));

    // compare distance between v2 and v1
    EXPECT_FLOAT_EQ(vector2.distanceTo(vector1), distance(x2, y2, x1, y1));
    EXPECT_FLOAT_EQ(vector2.distanceSquaredTo(vector1), distanceSquare(x2, y2, x1, y1));

    // compare distances between v1 & v2 and v2 & v1 
    EXPECT_FLOAT_EQ(vector2.distanceTo(vector1), vector1.distanceTo(vector2));
    EXPECT_FLOAT_EQ(vector2.distanceSquaredTo(vector1), vector1.distanceSquaredTo(vector2));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Vector2Test, Perpendicular)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x = random();
    const float32 y = random();

    const Vector2f vector(x, y);
    
    // calculate reference value
    float32 outX;
    float32 outY;
    perpendicular(x, y, outX, outY);

    // compare distance between v1 and v2
    EXPECT_FLOAT_EQ(vector.perpendicular().x, outX);
    EXPECT_FLOAT_EQ(vector.perpendicular().y, outY);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Vector2Test, Equality)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x = random();
    const float32 y = random();

    const Vector2f vector1(x, y);
    const Vector2f vector2(x, y);
    
    EXPECT_TRUE(vector1 == vector2);
    EXPECT_FALSE(vector1 != vector2);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Vector2Test, Scaling)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x     = random();
    const float32 y     = random();
    const float32 scale = static_cast<float32>(rand() % 256);

    Vector2f vector(x, y);
    vector *= scale;

    EXPECT_FLOAT_EQ(vector.x, x * scale);
    EXPECT_FLOAT_EQ(vector.y, y * scale);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Vector2Test, Addition)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x1 = random();
    const float32 y1 = random();
    const float32 x2 = random();
    const float32 y2 = random();

    const Vector2f vector1(x1, y1);
    const Vector2f vector2(x2, y2);
    
    // compare v1 + v2
    EXPECT_FLOAT_EQ((vector1 + vector2).x, x1 + x2);
    EXPECT_FLOAT_EQ((vector1 + vector2).y, y1 + y2);

    // compare v2 + v1
    EXPECT_FLOAT_EQ((vector2 + vector1).x, x2 + x1);
    EXPECT_FLOAT_EQ((vector2 + vector1).y, y2 + y1);

    // compare v1 + v2 and v2 + v1
    EXPECT_FLOAT_EQ((vector1 + vector2).x, (vector2 + vector1).x);
    EXPECT_FLOAT_EQ((vector1 + vector2).y, (vector2 + vector1).y);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Vector2Test, Difference)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x1 = random();
    const float32 y1 = random();
    const float32 x2 = random();
    const float32 y2 = random();

    const Vector2f vector1(x1, y1);
    const Vector2f vector2(x2, y2);
    
    // compare v1 - v2
    EXPECT_FLOAT_EQ((vector1 - vector2).x, x1 - x2);
    EXPECT_FLOAT_EQ((vector1 - vector2).y, y1 - y2);

    // compare v2 - v1
    EXPECT_FLOAT_EQ((vector2 - vector1).x, x2 - x1);
    EXPECT_FLOAT_EQ((vector2 - vector1).y, y2 - y1);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Vector2Test, Negation)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x = random();
    const float32 y = random();

    const Vector2f vector(x, y);
    const Vector2f negativeVector = -vector;
    
    // compare
    EXPECT_FLOAT_EQ(negativeVector.x, -x);
    EXPECT_FLOAT_EQ(negativeVector.y, -y);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Vector2Test, Lerp)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x1 = random();
    const float32 y1 = random();
    const float32 x2 = random();
    const float32 y2 = random();

    const Vector2f value1(x1, y1);
    const Vector2f value2(x2, y2);

    // interpolate
    for (float32 t = 0; t <= 1.0f; t += 0.01f)
    {
      float32 out[2];

      // calculate reference value
      lerp(out[0], out[1], x1, y1, x2, y2, t);

      // calculate actual value
      const Vector2f vectorOut = value1.lerp(value2, t);
    
      // test
      EGE_EXPECT_FLOAT_EQ(out[0], vectorOut.x, epsilon());
      EGE_EXPECT_FLOAT_EQ(out[1], vectorOut.y, epsilon());
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
