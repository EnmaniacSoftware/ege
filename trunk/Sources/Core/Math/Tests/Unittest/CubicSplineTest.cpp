#include "TestFramework/Interface/TestBase.h"
#include <EGESpline.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const int KRepetitionsCount = 20;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class CubicSplineTest : public TestBase
{
  protected:

    /*! Returns value at given bezier cubic spline.
     *  @param  t       Parameter describing relative position along spline in [0, 1] range.
     *  @param  point1  Curve start point.
     *  @param  point2  First control point.
     *  @param  point3  Second control point.
     *  @param  point4  End point.
     *  @return Position on the curve.
     */
    Vector4f bezierValue(float32 t, const Vector4f& point1, const Vector4f& point2, const Vector4f& point3, const Vector4f& point4) const;
    /*! Returns value at given hermite cubic spline.
     *  @param  t       Parameter describing relative position along spline in [0, 1] range.
     *  @param  point1  Curve start point.
     *  @param  point2  First control point.
     *  @param  point3  Second control point.
     *  @param  point4  End point.
     *  @return Position on the curve.
     */
    Vector4f hermiteValue(float32 t, const Vector4f& point1, const Vector4f& point2, const Vector4f& point3, const Vector4f& point4) const;
    /*! Returns value at given cardinal (CatMullRom) cubic spline.
     *  @param  t       Parameter describing relative position along spline in [0, 1] range.
     *  @param  point1  Curve start point.
     *  @param  point2  First control point.
     *  @param  point3  Second control point.
     *  @param  point4  End point.
     *  @return Position on the curve.
     */
    Vector4f cardinalValue(float32 t, const Vector4f& point1, const Vector4f& point2, const Vector4f& point3, const Vector4f& point4) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector4f CubicSplineTest::bezierValue(float32 t, const Vector4f& point1, const Vector4f& point2, const Vector4f& point3, const Vector4f& point4) const
{
  const float32 tt  = t * t;
  const float32 ttt = tt * t;
  const float32 u   = 1.0f - t;
  const float32 uu  = u * u;
  const float32 uuu = uu * u;

  const float32 a = uuu;
  const float32 b = 3.0f * uu * t;
  const float32 c = 3.0f * u * tt;
  const float32 d = ttt;

  const float32 x = a * point1.x + b * point2.x + c * point3.x + d * point4.x;
  const float32 y = a * point1.y + b * point2.y + c * point3.y + d * point4.y;
  const float32 z = a * point1.z + b * point2.z + c * point3.z + d * point4.z;

  return Vector4f(x, y, z, 1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector4f CubicSplineTest::hermiteValue(float32 t, const Vector4f& point1, const Vector4f& point2, const Vector4f& point3, const Vector4f& point4) const
{
  const float32 tt  = t * t;
  const float32 ttt = tt * t;

  const float32 a = 2.0f * ttt - 3 * tt + 1.0f;
  const float32 b = ttt - 2.0f * tt + t;
  const float32 c = ttt - tt;
  const float32 d = -2.0f * ttt + 3.0f * tt;

  const float32 x = a * point1.x + b * point2.x + c * point3.x + d * point4.x;
  const float32 y = a * point1.y + b * point2.y + c * point3.y + d * point4.y;
  const float32 z = a * point1.z + b * point2.z + c * point3.z + d * point4.z;

  return Vector4f(x, y, z, 1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector4f CubicSplineTest::cardinalValue(float32 t, const Vector4f& point1, const Vector4f& point2, const Vector4f& point3, const Vector4f& point4) const
{
  const float32 tt  = t * t;
  const float32 ttt = tt * t;

  const float32 a = -0.5f * ttt + tt - 0.5f * t;
  const float32 b = -1.5f * ttt + 2 * tt + 0.5f * t;
  const float32 c = 0.5f * ttt - 0.5f * tt;
  const float32 d = 1.5f * ttt - 2.5f * tt + 1.0f;

  const float32 x = a * point1.x + b * point2.x + c * point3.x + d * point4.x;
  const float32 y = a * point1.y + b * point2.y + c * point3.y + d * point4.y;
  const float32 z = a * point1.z + b * point2.z + c * point3.z + d * point4.z;

	return Vector4f(x, y, z, 1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(CubicSplineTest, BezierTest)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const Vector4f point1(random(), random(), random(), 1);
    const Vector4f point2(random(), random(), random(), 1);
    const Vector4f point3(random(), random(), random(), 1);
    const Vector4f point4(random(), random(), random(), 1);

    CubicSpline spline(EBezier);
  
    // add control points
    spline.addPoint(point1, point2);
    spline.addPoint(point4, point3);

    // simulate traversal along the curve
    for (float32 t = 0; t <= 1.0f; t += 0.1f)
    {
      t = (1.0f < t) ? 1.0f : t;

      // calculate reference value
      Vector4f referenceValue = bezierValue(t, point1, point2, point3, point4);

      // get value
      Vector4f out;
      spline.value(out, t);

      // compare
      EGE_EXPECT_FLOAT_EQ(referenceValue.x, out.x, 0.000001f);
      EGE_EXPECT_FLOAT_EQ(referenceValue.y, out.y, 0.000001f);
      EGE_EXPECT_FLOAT_EQ(referenceValue.z, out.z, 0.000001f);
      EGE_EXPECT_FLOAT_EQ(referenceValue.w, out.w, 0.000001f);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(CubicSplineTest, HermiteTest)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const Vector4f point1(random(), random(), random(), 1);
    const Vector4f point2(random(), random(), random(), 1);
    const Vector4f point3(random(), random(), random(), 1);
    const Vector4f point4(random(), random(), random(), 1);

    CubicSpline spline(EHermite);
  
    // add control points
    spline.addPoint(point1, point2);
    spline.addPoint(point4, point3);

    // simulate traversal along the curve
    for (float32 t = 0; t <= 1.0f; t += 0.1f)
    {
      t = (1.0f < t) ? 1.0f : t;

      // calculate reference value
      Vector4f referenceValue = hermiteValue(t, point1, point2, point3, point4);

      // get value
      Vector4f out;
      spline.value(out, t);

      // compare
      EGE_EXPECT_FLOAT_EQ(referenceValue.x, out.x, 0.000001f);
      EGE_EXPECT_FLOAT_EQ(referenceValue.y, out.y, 0.000001f);
      EGE_EXPECT_FLOAT_EQ(referenceValue.z, out.z, 0.000001f);
      EGE_EXPECT_FLOAT_EQ(referenceValue.w, out.w, 0.000001f);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(CubicSplineTest, CardinalTest)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const Vector4f point1(random(), random(), random(), 1);
    const Vector4f point2(random(), random(), random(), 1);
    const Vector4f point3(random(), random(), random(), 1);
    const Vector4f point4(random(), random(), random(), 1);

    CubicSpline spline(ECardinal);
  
    // add control points
    spline.addPoint(point1, point2);
    spline.addPoint(point4, point3);

    // simulate traversal along the curve
    for (float32 t = 0; t <= 1.0f; t += 0.1f)
    {
      t = (1.0f < t) ? 1.0f : t;

      // calculate reference value
      Vector4f referenceValue = cardinalValue(t, point1, point2, point3, point4);

      // get value
      Vector4f out;
      spline.value(out, t);

      // compare
      EGE_EXPECT_FLOAT_EQ(referenceValue.x, out.x, 0.000001f);
      EGE_EXPECT_FLOAT_EQ(referenceValue.y, out.y, 0.000001f);
      EGE_EXPECT_FLOAT_EQ(referenceValue.z, out.z, 0.000001f);
      EGE_EXPECT_FLOAT_EQ(referenceValue.w, out.w, 0.000001f);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
