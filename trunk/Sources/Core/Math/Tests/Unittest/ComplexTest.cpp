#include "TestFramework/Interface/TestBase.h"
#include <EGEComplex.h>
#include <EGEAngle.h>

/** Tests are focusing TComplex<float32> instantiations. */

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const int KRepetitionsCount = 20;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ComplexTest : public TestBase
{
  protected:

    /*! Normalizes given complex value. 
     *  @param  x     Complex number real part value.
     *  @param  y     Complex number imaginary part value.
     *  @param  outX  Normalized real part value.
     *  @param  outY  Normalized imaginary part value.
     */
    void normalize(float32 x, float32 y, float32& outX, float32& outY) const;
    /*! Returns length of the graphical representation of a given complex value.
     *  @param  x     Complex number real part value.
     *  @param  y     Complex number imaginary part value.
     *  @return Length of the graphical representation of complex number.
     */ 
    float32 length(float32 x, float32 y) const;
    /*! Returns squared length of the graphical representation of a given complex value.
     *  @param  x     Complex number real part value.
     *  @param  y     Complex number imaginary part value.
     *  @return Squared length of the graphical representation of complex number.
     */ 
    float32 lengthSquared(float32 x, float32 y) const;
    /*! Calculates dot product between graphical representations of a given complex values. 
     *  @param  x1  Complex number 1 real part value.
     *  @param  y1  Complex number 1 imaginary part value.
     *  @param  x2  Complex number 2 real part value.
     *  @param  y2  Complex number 2 imaginary part value.
     *  @return Dot product between graphical representations of a given complex values.
     */
    float32 dotProduct(float32 x1, float32 y1, float32 x2, float32 y2) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ComplexTest::normalize(float32 x, float32 y, float32& outX, float32& outY) const
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
float32 ComplexTest::length(float32 x, float32 y) const
{
  return sqrtf((x * x) + (y * y));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 ComplexTest::lengthSquared(float32 x, float32 y) const
{
  return (x * x) + (y * y);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 ComplexTest::dotProduct(float32 x1, float32 y1, float32 x2, float32 y2) const
{
  return (x1 * x2) + (y1 * y2); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ComplexTest, SetValue)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x = random();
    const float32 y = random();
    const Angle angle = random();

    // setting via constructors
    const Complexf complex1(x, y);
    EXPECT_FLOAT_EQ(complex1.x, x);
    EXPECT_FLOAT_EQ(complex1.y, y);

    const Complexf complex2(complex1);
    EXPECT_FLOAT_EQ(complex1.x, complex2.x);
    EXPECT_FLOAT_EQ(complex1.y, complex2.y);

    // setting by setters
    const Complexf complex3(angle);
    Complexf complex4;
    complex4.create(angle);

    EXPECT_FLOAT_EQ(complex3.x, complex4.x);
    EXPECT_FLOAT_EQ(complex3.y, complex4.y);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ComplexTest, Length)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x = random();
    const float32 y = random();

    const Complexf value(x, y);
    
    // compare
    EXPECT_FLOAT_EQ(value.length(), length(x, y));
    EXPECT_FLOAT_EQ(value.lengthSquared(), lengthSquared(x, y));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ComplexTest, Normalize)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x = random();
    const float32 y = random();

    Complexf value(x, y);
    
    // normalize
    value.normalize();

    // calculate reference normalization
    float32 outX;
    float32 outY;
    normalize(x, y, outX, outY);

    // compare
    EXPECT_FLOAT_EQ(value.x, outX);
    EXPECT_FLOAT_EQ(value.y, outY);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ComplexTest, DotProduct)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const float32 x1 = random();
    const float32 y1 = random();
    const float32 x2 = random();
    const float32 y2 = random();

    const Complexf value1(x1, y1);
    const Complexf value2(x2, y2);
     
    // compare dot product between v1 and v2
    EXPECT_FLOAT_EQ(value1.dotProduct(value2), dotProduct(x1, y1, x2, y2));

    // compare dot product between v2 and v1
    EXPECT_FLOAT_EQ(value2.dotProduct(value1), dotProduct(x2, y2, x1, y1));

    // compare dot products of v1 * v2 and v2 * v1
    EXPECT_FLOAT_EQ(value2.dotProduct(value1), value1.dotProduct(value2));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ComplexTest, ToAngle)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const Angle angle(random());
     
    const Complexf value(angle);

    // compare
    EXPECT_FLOAT_EQ(angle.radians(), value.angle().radians());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------