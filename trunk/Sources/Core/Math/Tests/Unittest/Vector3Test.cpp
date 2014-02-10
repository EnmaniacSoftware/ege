#include "TestFramework/Interface/TestBase.h"
#include <limits>
#include <EGEVector.h>

/** Tests are focusing TVector3<float32> instantiations. */

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const int KRepetitionsCount = 20;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Vector3Test : public TestBase
{
  protected:

    /*! Normalized given vector data. 
     *  @param  x     Vector's X value.
     *  @param  y     Vector's Y value.
     *  @param  z     Vector's Z value.
     *  @param  outX  Normalized vector's X value.
     *  @param  outY  Normalized vector's Y value.
     *  @param  outZ  Normalized vector's Z value.
     */
    void normalize(float32 x, float32 y, float32 z, float32& outX, float32& outY, float32& outZ) const;
    /*! Returns length of the given vector data.
     *  @param  x     Vector's X value.
     *  @param  y     Vector's Y value.
     *  @param  z     Vector's Z value.
     *  @return Length of the vector.
     */ 
    float32 length(float32 x, float32 y, float32 z) const;
    /*! Returns squared length of the given vector data.
     *  @param  x     Vector's X value.
     *  @param  y     Vector's Y value.
     *  @param  z     Vector's Z value.
     *  @return Squared length of the vector.
     */ 
    float32 lengthSquared(float32 x, float32 y, float32 z) const;
    /*! Calculates dot product between vectors. 
     *  @param  x1  Vector 1 X value.
     *  @param  y1  Vector 1 Y value.
     *  @param  z1  Vector 1 Z value.
     *  @param  x2  Vector 2 X value.
     *  @param  y2  Vector 2 Y value.
     *  @param  z2  Vector 2 Z value.
     *  @return Dot product between two vectors.
     */
    float32 dotProduct(float32 x1, float32 y1, float32 z1, float32 x2, float32 y2, float32 z2) const;
    /*! Calculates distance between vectors. 
     *  @param  x1  Vector 1 X value.
     *  @param  y1  Vector 1 Y value.
     *  @param  z1  Vector 1 Z value.
     *  @param  x2  Vector 2 X value.
     *  @param  y2  Vector 2 Y value.
     *  @param  z2  Vector 2 Z value.
     *  @return Distance between two vectors.
     */
    float32 distance(float32 x1, float32 y1, float32 z1, float32 x2, float32 y2, float32 z2) const;
    /*! Calculates squared distance between vectors. 
     *  @param  x1  Vector 1 X value.
     *  @param  y1  Vector 1 Y value.
     *  @param  z1  Vector 1 Z value.
     *  @param  x2  Vector 2 X value.
     *  @param  y2  Vector 2 Y value.
     *  @param  z2  Vector 2 Z value.
     *  @return Squared distance between two vectors.
     */
    float32 distanceSquare(float32 x1, float32 y1, float32 z1, float32 x2, float32 y2, float32 z2) const;
    /*! Calculates perpendicular vector of a given one.
     *  @param  x     Vector X value.
     *  @param  y     Vector Y value.
     *  @param  z     Vector Z value.
     *  @param  outX  Perpendicular vector X value.
     *  @param  outY  Perpendicular vector Y value.
     *  @param  outZ  Perpendicular vector Z value.
     */
    void perpendicular(float32 x, float32 y, float32 z, float32& outX, float32& outY, float32& outZ) const;
    /*! Calculates cross product between vectors. 
     *  @param  x1    Vector 1 X value.
     *  @param  y1    Vector 1 Y value.
     *  @param  z1    Vector 1 Z value.
     *  @param  x2    Vector 2 X value.
     *  @param  y2    Vector 2 Y value.
     *  @param  z2    Vector 2 Z value.
     *  @param  outX  Calculated vector X value.
     *  @param  outY  Calculated vector Y value.
     *  @param  outZ  Calculated vector Z value.
     */
    void crossProduct(float32 x1, float32 y1, float32 z1, float32 x2, float32 y2, float32 z2, float32& outX, float32& outY, float32& outZ) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Vector3Test::normalize(float32 x, float32 y, float32 z, float32& outX, float32& outY, float32& outZ) const
{
  outX = x;
  outY = y;
  outZ = z;

  // get length
  float32 len = length(x, y, z);

  if (std::numeric_limits<float32>::epsilon() <= len)
  {
    // get inverse of length
    float32 invLength = 1.0f / len;

    // normalize
	  outX *= invLength;
	  outY *= invLength;
    outZ *= invLength;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 Vector3Test::length(float32 x, float32 y, float32 z) const
{
  return sqrtf((x * x) + (y * y) + (z * z));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 Vector3Test::lengthSquared(float32 x, float32 y, float32 z) const
{
  return (x * x) + (y * y) + (z * z);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 Vector3Test::dotProduct(float32 x1, float32 y1, float32 z1, float32 x2, float32 y2, float32 z2) const
{
  return (x1 * x2) + (y1 * y2) + (z1 * z2); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 Vector3Test::distance(float32 x1, float32 y1, float32 z1, float32 x2, float32 y2, float32 z2) const
{
  return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 Vector3Test::distanceSquare(float32 x1, float32 y1, float32 z1, float32 x2, float32 y2, float32 z2) const
{
  return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Vector3Test::perpendicular(float32 x, float32 y, float32 z, float32& outX, float32& outY, float32& outZ) const
{
  crossProduct(x, y, z, 1.0f, 0, 0, outX, outY, outZ);

  // check length
  if ((std::numeric_limits<float32>::epsilon() * std::numeric_limits<float32>::epsilon()) > lengthSquared(outX, outY, outZ))
  {
    // this vector is the Y axis multiplied by a scalar, so we have to use another axis
    crossProduct(x, y, z, 0, 1.0f, 0, outX, outY, outZ);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Vector3Test::crossProduct(float32 x1, float32 y1, float32 z1, float32 x2, float32 y2, float32 z2, float32& outX, float32& outY, float32& outZ) const
{
  outX = (y1 * z2) - (z1 * y2);
  outY = (z1 * x2) - (x1 * z2);
  outZ = (x1 * y2) - (y1 * x2);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Vector3Test, SetValue)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x = random();
    const float32 y = random();
    const float32 z = random();

    // setting via constructors
    const Vector3f vector(x, y, z);
    EXPECT_FLOAT_EQ(vector.x, x);
    EXPECT_FLOAT_EQ(vector.y, y);
    EXPECT_FLOAT_EQ(vector.z, z);

    const Vector3f vector2(vector);
    EXPECT_FLOAT_EQ(vector2.x, vector.x);
    EXPECT_FLOAT_EQ(vector2.y, vector.y);
    EXPECT_FLOAT_EQ(vector2.z, vector.z);

    // setting by setters
    Vector3f vector3;
    vector3.set(x, y, z);
    EXPECT_FLOAT_EQ(vector3.x, x);
    EXPECT_FLOAT_EQ(vector3.y, y);
    EXPECT_FLOAT_EQ(vector3.z, z);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Vector3Test, Length)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x = random();
    const float32 y = random();
    const float32 z = random();

    Vector3f vector(x, y, z);
    
    // compare
    EXPECT_FLOAT_EQ(vector.length(), length(x, y, z));
    EXPECT_FLOAT_EQ(vector.lengthSquared(), lengthSquared(x, y, z));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Vector3Test, Normalize)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x = random();
    const float32 y = random();
    const float32 z = random();

    Vector3f vector(x, y, z);
    
    // get normalized vector first
    Vector3f normalized = vector.normalized();

    // normalize
    vector.normalize();

    // calculate reference normalization
    float32 outX;
    float32 outY;
    float32 outZ;
    normalize(x, y, z, outX, outY, outZ);

    // compare
    EXPECT_FLOAT_EQ(vector.x, outX);
    EXPECT_FLOAT_EQ(vector.y, outY);
    EXPECT_FLOAT_EQ(vector.z, outZ);
    EXPECT_FLOAT_EQ(normalized.x, outX);
    EXPECT_FLOAT_EQ(normalized.y, outY);
    EXPECT_FLOAT_EQ(normalized.z, outZ);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Vector3Test, DotProduct)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x1 = random();
    const float32 y1 = random();
    const float32 z1 = random();
    const float32 x2 = random();
    const float32 y2 = random();
    const float32 z2 = random();

    const Vector3f vector1(x1, y1, z1);
    const Vector3f vector2(x2, y2, z2);
     
    // compare dot product between v1 and v2
    EXPECT_FLOAT_EQ(vector1.dotProduct(vector2), dotProduct(x1, y1, z1, x2, y2, z2));

    // compare dot product between v2 and v1
    EXPECT_FLOAT_EQ(vector2.dotProduct(vector1), dotProduct(x2, y2, z2, x1, y1, z1));

    // compare dot products of v1 * v2 and v2 * v1
    EXPECT_FLOAT_EQ(vector2.dotProduct(vector1), vector1.dotProduct(vector2));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Vector3Test, Distance)
{
  const float32 KVectorScale = 100.0f;

  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x1 = random(KVectorScale);
    const float32 y1 = random(KVectorScale);
    const float32 z1 = random(KVectorScale);
    const float32 x2 = random(KVectorScale);
    const float32 y2 = random(KVectorScale);
    const float32 z2 = random(KVectorScale);

    const Vector3f vector1(x1, y1, z1);
    const Vector3f vector2(x2, y2, z2);
     
    // compare distance between v1 and v2
    EXPECT_FLOAT_EQ(vector1.distanceTo(vector2), distance(x1, y1, z1, x2, y2, z2));
    EXPECT_FLOAT_EQ(vector1.distanceSquaredTo(vector2), distanceSquare(x1, y1, z1, x2, y2, z2));

    // compare distance between v2 and v1
    EXPECT_FLOAT_EQ(vector2.distanceTo(vector1), distance(x2, y2, z2, x1, y1, z1));
    EXPECT_FLOAT_EQ(vector2.distanceSquaredTo(vector1), distanceSquare(x2, y2, z2, x1, y1, z1));

    // compare distances between v1 & v2 and v2 & v1 
    EXPECT_FLOAT_EQ(vector2.distanceTo(vector1), vector1.distanceTo(vector2));
    EXPECT_FLOAT_EQ(vector2.distanceSquaredTo(vector1), vector1.distanceSquaredTo(vector2));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Vector3Test, CrossProduct)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x1 = random();
    const float32 y1 = random();
    const float32 z1 = random();
    const float32 x2 = random();
    const float32 y2 = random();
    const float32 z2 = random();

    const Vector3f vector1(x1, y1, z1);
    const Vector3f vector2(x2, y2, z2);
     
    float32 outX;
    float32 outY;
    float32 outZ;
    crossProduct(x1, y1, z1, x2, y2, z2, outX, outY, outZ);

    // compare cross product between v1 and v2
    EXPECT_FLOAT_EQ(vector1.crossProduct(vector2).x, outX);
    EXPECT_FLOAT_EQ(vector1.crossProduct(vector2).y, outY);
    EXPECT_FLOAT_EQ(vector1.crossProduct(vector2).z, outZ);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Vector3Test, Perpendicular)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x = random();
    const float32 y = random();
    const float32 z = random();

    const Vector3f vector(x, y, z);
    
    // calculate reference value
    float32 outX;
    float32 outY;
    float32 outZ;
    perpendicular(x, y, z, outX, outY, outZ);

    // compare distance between v1 and v2
    EXPECT_FLOAT_EQ(vector.perpendicular().x, outX);
    EXPECT_FLOAT_EQ(vector.perpendicular().y, outY);
    EXPECT_FLOAT_EQ(vector.perpendicular().z, outZ);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Vector3Test, Equality)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x = random();
    const float32 y = random();
    const float32 z = random();

    const Vector3f vector1(x, y, z);
    const Vector3f vector2(x, y, z);
    
    EXPECT_TRUE(vector1 == vector2);
    EXPECT_FALSE(vector1 != vector2);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Vector3Test, Scaling)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x     = random();
    const float32 y     = random();
    const float32 z     = random();
    const float32 scale = static_cast<float32>(rand() % 256);

    Vector3f vector(x, y, z);
    vector *= scale;

    EXPECT_FLOAT_EQ(vector.x, x * scale);
    EXPECT_FLOAT_EQ(vector.y, y * scale);
    EXPECT_FLOAT_EQ(vector.z, z * scale);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Vector3Test, Addition)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x1 = random();
    const float32 y1 = random();
    const float32 z1 = random();
    const float32 x2 = random();
    const float32 y2 = random();
    const float32 z2 = random();

    const Vector3f vector1(x1, y1, z1);
    const Vector3f vector2(x2, y2, z2);
    
    // compare v1 + v2
    EXPECT_FLOAT_EQ((vector1 + vector2).x, x1 + x2);
    EXPECT_FLOAT_EQ((vector1 + vector2).y, y1 + y2);
    EXPECT_FLOAT_EQ((vector1 + vector2).z, z1 + z2);

    // compare v2 + v1
    EXPECT_FLOAT_EQ((vector2 + vector1).x, x2 + x1);
    EXPECT_FLOAT_EQ((vector2 + vector1).y, y2 + y1);
    EXPECT_FLOAT_EQ((vector2 + vector1).z, z2 + z1);

    // compare v1 + v2 and v2 + v1
    EXPECT_FLOAT_EQ((vector1 + vector2).x, (vector2 + vector1).x);
    EXPECT_FLOAT_EQ((vector1 + vector2).y, (vector2 + vector1).y);
    EXPECT_FLOAT_EQ((vector1 + vector2).z, (vector2 + vector1).z);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Vector3Test, Difference)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x1 = random();
    const float32 y1 = random();
    const float32 z1 = random();
    const float32 x2 = random();
    const float32 y2 = random();
    const float32 z2 = random();

    const Vector3f vector1(x1, y1, z1);
    const Vector3f vector2(x2, y2, z2);
    
    // compare v1 - v2
    EXPECT_FLOAT_EQ((vector1 - vector2).x, x1 - x2);
    EXPECT_FLOAT_EQ((vector1 - vector2).y, y1 - y2);
    EXPECT_FLOAT_EQ((vector1 - vector2).z, z1 - z2);

    // compare v2 - v1
    EXPECT_FLOAT_EQ((vector2 - vector1).x, x2 - x1);
    EXPECT_FLOAT_EQ((vector2 - vector1).y, y2 - y1);
    EXPECT_FLOAT_EQ((vector2 - vector1).z, z2 - z1);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Vector3Test, Negation)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x = random();
    const float32 y = random();
    const float32 z = random();

    const Vector3f vector(x, y, z);
    const Vector3f negativeVector = -vector;
    
    // compare
    EXPECT_FLOAT_EQ(negativeVector.x, -x);
    EXPECT_FLOAT_EQ(negativeVector.y, -y);
    EXPECT_FLOAT_EQ(negativeVector.z, -z);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
