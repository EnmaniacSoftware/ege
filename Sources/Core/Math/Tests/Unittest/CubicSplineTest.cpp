#include "TestFramework/Interface/TestBase.h"
#include <EGEMath.h>
#include <EGESpline.h>
#include <EGEVector3.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const int KRepetitionsCount = 20;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class CubicSplineTest : public TestBase
{
  protected:

    CubicSplineTest();

    /*! Returns value at given bezier cubic spline.
     *  @param  t       Parameter describing relative position along spline in [0, 1] range.
     *  @param  point1  Curve start point.
     *  @param  point2  First control point.
     *  @param  point3  Second control point.
     *  @param  point4  End point.
     *  @return Position on the curve.
     */
    Vector3f bezierValue(float32 t, const Vector3f& point1, const Vector3f& point2, const Vector3f& point3, const Vector3f& point4) const;
    /*! Returns value at given hermite cubic spline.
     *  @param  t       Parameter describing relative position along spline in [0, 1] range.
     *  @param  point1  Curve start point.
     *  @param  point2  First control point.
     *  @param  point3  Second control point.
     *  @param  point4  End point.
     *  @return Position on the curve.
     */
    Vector3f hermiteValue(float32 t, const Vector3f& point1, const Vector3f& point2, const Vector3f& point3, const Vector3f& point4) const;
    /*! Returns value at given cardinal (CatMullRom) cubic spline.
     *  @param  t       Parameter describing relative position along spline in [0, 1] range.
     *  @param  point1  Curve start point.
     *  @param  point2  First control point.
     *  @param  point3  Second control point.
     *  @param  point4  End point.
     *  @return Position on the curve.
     */
    Vector3f cardinalValue(float32 t, const Vector3f& point1, const Vector3f& point2, const Vector3f& point3, const Vector3f& point4) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CubicSplineTest::CubicSplineTest() : TestBase(0.000001f)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector3f CubicSplineTest::bezierValue(float32 t, const Vector3f& point1, const Vector3f& point2, const Vector3f& point3, const Vector3f& point4) const
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

  return Vector3f(x, y, z);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector3f CubicSplineTest::hermiteValue(float32 t, const Vector3f& point1, const Vector3f& point2, const Vector3f& point3, const Vector3f& point4) const
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

  return Vector3f(x, y, z);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector3f CubicSplineTest::cardinalValue(float32 t, const Vector3f& point1, const Vector3f& point2, const Vector3f& point3, const Vector3f& point4) const
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

	return Vector3f(x, y, z);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(CubicSplineTest, BezierTest)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const Vector3f point1(random(), random(), random());
    const Vector3f point2(random(), random(), random());
    const Vector3f point3(random(), random(), random());
    const Vector3f point4(random(), random(), random());

    const List<Vector3f> points = List<Vector3f>() << point1 << point2 << point3 << point4;
    
    CubicSpline spline(EBezier);
  
    // add control points
    spline.addPoints(points);

    // simulate traversal along the curve
    for (float32 t = 0; t <= 1.0f; t += 0.1f)
    {
      t = (1.0f < t) ? 1.0f : t;

      // calculate reference value
      Vector3f referenceValue = bezierValue(t, point1, point2, point3, point4);

      // get value
      const Vector3f out = spline.value(t);

      // compare
      EGE_EXPECT_FLOAT_EQ(referenceValue.x, out.x, epsilon());
      EGE_EXPECT_FLOAT_EQ(referenceValue.y, out.y, epsilon());
      EGE_EXPECT_FLOAT_EQ(referenceValue.z, out.z, epsilon());
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(CubicSplineTest, HermiteTest)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const Vector3f point1(random(), random(), random());
    const Vector3f point2(random(), random(), random());
    const Vector3f point3(random(), random(), random());
    const Vector3f point4(random(), random(), random());

    const List<Vector3f> points = List<Vector3f>() << point1 << point2 << point3 << point4;

    CubicSpline spline(EHermite);
  
    // add control points
    spline.addPoints(points);

    // simulate traversal along the curve
    for (float32 t = 0; t <= 1.0f; t += 0.1f)
    {
      t = (1.0f < t) ? 1.0f : t;

      // calculate reference value
      Vector3f referenceValue = hermiteValue(t, point1, point2, point3, point4);

      // get value
      Vector3f out = spline.value(t);

      // compare
      EGE_EXPECT_FLOAT_EQ(referenceValue.x, out.x, epsilon());
      EGE_EXPECT_FLOAT_EQ(referenceValue.y, out.y, epsilon());
      EGE_EXPECT_FLOAT_EQ(referenceValue.z, out.z, epsilon());
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(CubicSplineTest, CardinalTest)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const Vector3f point1(random(), random(), random());
    const Vector3f point2(random(), random(), random());
    const Vector3f point3(random(), random(), random());
    const Vector3f point4(random(), random(), random());

    const List<Vector3f> points = List<Vector3f>() << point1 << point2 << point3 << point4;

    CubicSpline spline(ECardinal);
  
    // add control points
    spline.addPoints(points);

    // simulate traversal along the curve
    for (float32 t = 0; t <= 1.0f; t += 0.1f)
    {
      t = (1.0f < t) ? 1.0f : t;

      // calculate reference value
      Vector3f referenceValue = cardinalValue(t, point1, point2, point3, point4);

      // get value
      Vector3f out = spline.value(t);

      // compare
      EGE_EXPECT_FLOAT_EQ(referenceValue.x, out.x, epsilon());
      EGE_EXPECT_FLOAT_EQ(referenceValue.y, out.y, epsilon());
      EGE_EXPECT_FLOAT_EQ(referenceValue.z, out.z, epsilon());
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(CubicSplineTest, Length)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const Vector3f point1(random(), random(), random());
    const Vector3f point2(random(), random(), random());
    const Vector3f point3(random(), random(), random());
    const Vector3f point4(random(), random(), random());

    const List<Vector3f> points = List<Vector3f>() << point1 << point2 << point3 << point4;

    CubicSpline splineBezier(EBezier);
    CubicSpline splineHermite(EHermite);
    CubicSpline splineCardinal(ECardinal);
  
    // add control points
    splineBezier.addPoints(points);
    splineHermite.addPoints(points);
    splineCardinal.addPoints(points);

    float32 lengthBezier = 0;
    float32 lengthHermite = 0;
    float32 lengthCardinal = 0;

    // simulate traversal along the curve and calculate length
    const float32 dt = 0.01f;
    for (float32 t = 0; t <= 1.0f; t += dt)
    {
      t = (1.0f < t) ? 1.0f : t;

      if (1.0f != t)
      {
        Vector3f a;
        Vector3f b;

        // calculate length of next segment...

        // ...for BEZIER spline...
        a = bezierValue(t, point1, point2, point3, point4);
        b = bezierValue(Math::Min(t + dt, 1.0f), point1, point2, point3, point4);

        lengthBezier += Vector3f(a.x, a.y, a.z).distanceTo(Vector3f(b.x, b.y, b.z));

        // ...for HERMITE spline...
        a = hermiteValue(t, point1, point2, point3, point4);
        b = hermiteValue(Math::Min(t + dt, 1.0f), point1, point2, point3, point4);

        lengthHermite += Vector3f(a.x, a.y, a.z).distanceTo(Vector3f(b.x, b.y, b.z));

        // ...for CARDINAL spline
        a = cardinalValue(t, point1, point2, point3, point4);
        b = cardinalValue(Math::Min(t + dt, 1.0f), point1, point2, point3, point4);

        lengthCardinal += Vector3f(a.x, a.y, a.z).distanceTo(Vector3f(b.x, b.y, b.z));
      }
    }

    // compare
    EGE_EXPECT_FLOAT_EQ(lengthBezier, splineBezier.length(), 0.00001f);
    EGE_EXPECT_FLOAT_EQ(lengthHermite, splineHermite.length(), 0.00001f);
    EGE_EXPECT_FLOAT_EQ(lengthCardinal, splineCardinal.length(), 0.00001f);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
