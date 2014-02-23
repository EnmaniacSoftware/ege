#include "TestFramework/Interface/TestBase.h"
#include "Core/Math/Tests/Unittest/Helpers/VectorHelper.h"
#include <EGELine2.h>
#include <EGEVector2.h>

/*! Tests are focusing on Line2<float32> instantiations. */

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const int KRepetitionsCount = 20;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Line2Test : public TestBase
{
  protected:

    Line2Test();

    /*! Calculates line-line intersection point.
     *  @param  line1PointA  First point belonging to the line 1.
     *  @param  line1PointB  Second point belonging to the line 1.
     *  @param  line2PointA  First point belonging to the line 2.
     *  @param  line2PointB  Second point belonging to the line 2.
     *  @return Point of intersection. If lines do not intersect empty data is returned.
     */
    std::vector<float32> lineLineIntersectionPoint(const std::vector<float32>& line1PointA, const std::vector<float32>& line1PointB,
                                                   const std::vector<float32>& line2PointA, const std::vector<float32>& line2PointB) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Line2Test::Line2Test() : TestBase(0.0001f)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<float32> Line2Test::lineLineIntersectionPoint(const std::vector<float32>& line1PointA, const std::vector<float32>& line1PointB,
                                                          const std::vector<float32>& line2PointA, const std::vector<float32>& line2PointB) const
{
  std::vector<float32> out;

  // calculate denotmiator
  const float32 denominator = (line1PointA[0] - line1PointB[0]) * (line2PointA[1] - line2PointB[1]) - 
                              (line1PointA[1] - line1PointB[1]) * (line2PointA[0] - line2PointB[0]);
  if (std::numeric_limits<float32>::epsilon() <= fabsf(denominator))
  {
    // calculate some coefficient for optimizaton purposes
    const float32 nominatorCoe1  = (line1PointA[0] * line1PointB[1] - line1PointA[1] * line1PointB[0]);
    const float32 nominatorCoe2  = (line2PointA[0] * line2PointB[1] - line2PointA[1] * line2PointB[0]);
    const float32 invDenominator = 1.0f / denominator;

    out.push_back((nominatorCoe1 * (line2PointA[0] - line2PointB[0]) - (line1PointA[0] - line1PointB[0]) * nominatorCoe2) * invDenominator);
    out.push_back((nominatorCoe1 * (line2PointA[1] - line2PointB[1]) - (line1PointA[1] - line1PointB[1]) * nominatorCoe2) * invDenominator);
  }

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Line2Test, SetValue)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const std::vector<float32> point1 = VectorHelper::RandomVector2Data();
    const std::vector<float32> point2 = VectorHelper::RandomVector2Data();

    const Vector2f p1(point1[0], point1[1]);
    const Vector2f p2(point2[0], point2[1]);

    // setting via constructors
    const Line2f line(p1, p2);
    EXPECT_FLOAT_EQ(point1[0], line.offset.x);
    EXPECT_FLOAT_EQ(point1[1], line.offset.y);

    EXPECT_FLOAT_EQ((p2 - p1).normalized().x, line.dir.x);
    EXPECT_FLOAT_EQ((p2 - p1).normalized().y, line.dir.y);

    const Line2f line2(line);
    
    EXPECT_FLOAT_EQ(line.dir.x, line2.dir.x);
    EXPECT_FLOAT_EQ(line.dir.y, line2.dir.y);
    EXPECT_FLOAT_EQ(line.offset.x, line2.offset.x);
    EXPECT_FLOAT_EQ(line.offset.y, line2.offset.y);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Line2Test, Intersects)
{
  Vector2f point;

  Line2f line1;
  Line2f line2;

  // Intersection at P(5, 0)
  Vector2f expected(5, 0);
  
  line1 = Line2f(Vector2f(0, 0), Vector2f(10, 0));
  line2 = Line2f(Vector2f(5, -5), Vector2f(5, -4));

  EXPECT_TRUE(line1.intersects(point, line2));
  EXPECT_TRUE(expected == point);

  // No intersection
  line1 = Line2f(Vector2f(0, 0), Vector2f(10, 0));
  line2 = Line2f(Vector2f(10, 7), Vector2f(11, 7));

  EXPECT_FALSE(line1.intersects(point, line2));

  // perform fixed number of random tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const std::vector<float32> line1Point1 = VectorHelper::RandomVector2Data();
    const std::vector<float32> line1Point2 = VectorHelper::RandomVector2Data();
    const std::vector<float32> line2Point1 = VectorHelper::RandomVector2Data();
    const std::vector<float32> line2Point2 = VectorHelper::RandomVector2Data();

    const Vector2f line1p1(line1Point1[0], line1Point1[1]);
    const Vector2f line1p2(line1Point2[0], line1Point2[1]);
    const Vector2f line2p1(line2Point1[0], line2Point1[1]);
    const Vector2f line2p2(line2Point2[0], line2Point2[1]);

    line1 = Line2f(Vector2f(line1Point1[0], line1Point1[1]), Vector2f(line1Point2[0], line1Point2[1]));
    line2 = Line2f(Vector2f(line2Point1[0], line2Point1[1]), Vector2f(line2Point2[0], line2Point2[1]));

    // test line1 intersecting line2
    std::vector<float32> out = lineLineIntersectionPoint(line1Point1, line1Point2, line2Point1, line2Point2);
    bool intersects = line1.intersects(point, line2);

    // TAGE - seems if lines are close to be parallel that intersection point is far far away and precision lost is quite big...
    //        perhaps can make some adjustments here to not allow too parallel intersections to happen
    EXPECT_TRUE(out.empty() != intersects);
    EGE_EXPECT_FLOAT_EQ(out[0], point.x, 0.1f);
    EGE_EXPECT_FLOAT_EQ(out[1], point.y, 0.1f);

    // test line2 intersecting line1
    out = lineLineIntersectionPoint(line2Point1, line2Point2, line1Point1, line1Point2);
    intersects = line2.intersects(point, line1);

    // TAGE - seems if lines are close to be parallel that intersection point is far far away and precision lost is quite big...
    //        perhaps can make some adjustments here to not allow too parallel intersections to happen
    EXPECT_TRUE(out.empty() != intersects);
    EGE_EXPECT_FLOAT_EQ(out[0], point.x, 0.1f);
    EGE_EXPECT_FLOAT_EQ(out[1], point.y, 0.1f);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
