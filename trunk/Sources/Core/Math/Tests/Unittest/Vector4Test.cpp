#include "TestFramework/Interface/TestBase.h"
#include <EGEVector4.h>

/** Tests are focusing TVector4<float32> instantiations. */

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const int KRepetitionsCount = 20;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Vector4Test : public TestBase
{
};
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

    // setting by operators
    Vector4f vector4;
    vector4 = vector3;
    EXPECT_FLOAT_EQ(vector4.x, vector3.x);
    EXPECT_FLOAT_EQ(vector4.y, vector3.y);
    EXPECT_FLOAT_EQ(vector4.z, vector3.z);
    EXPECT_FLOAT_EQ(vector4.w, vector3.w);
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

    // test global operators...
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

    // ...and internal operators
    Vector4f vector3(x1, y1, z1, w1);
    vector3 += vector2;

    EXPECT_FLOAT_EQ(vector3.x, x1 + x2);
    EXPECT_FLOAT_EQ(vector3.y, y1 + y2);
    EXPECT_FLOAT_EQ(vector3.z, z1 + z2);
    EXPECT_FLOAT_EQ(vector3.w, w1 + w2);
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

    // test global operators...
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

    // ...and internal operators
    Vector4f vector3(x1, y1, z1, w1);
    vector3 -= vector2;

    EXPECT_FLOAT_EQ(vector3.x, x1 - x2);
    EXPECT_FLOAT_EQ(vector3.y, y1 - y2);
    EXPECT_FLOAT_EQ(vector3.z, z1 - z2);
    EXPECT_FLOAT_EQ(vector3.w, w1 - w2);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Vector4Test, Scaling)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x = random();
    const float32 y = random();
    const float32 z = random();
    const float32 w = random();
    const float32 scale = static_cast<float32>(rand() % 256);

    // test global operators...
    Vector4f vector(x, y, z, w);
    vector = vector * scale;

    EXPECT_FLOAT_EQ(vector.x, x * scale);
    EXPECT_FLOAT_EQ(vector.y, y * scale);
    EXPECT_FLOAT_EQ(vector.z, z * scale);
    EXPECT_FLOAT_EQ(vector.w, w * scale);

    vector.set(x, y, z, w);
    vector = scale * vector;

    EXPECT_FLOAT_EQ(vector.x, x * scale);
    EXPECT_FLOAT_EQ(vector.y, y * scale);
    EXPECT_FLOAT_EQ(vector.z, z * scale);
    EXPECT_FLOAT_EQ(vector.w, w * scale);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
