#include "TestFramework/Interface/TestBase.h"
#include <EGERect.h>

/** Tests are focusing TRect<s32> instantiations. 
 *  The reason behind testing S32 version rather than FLOAT32 is twofold:
 *  - precision is fixed for S32 so it is easy to test boundary conditions
 *  - due to subtle precision errors FLOAT32 has a tendency to drift away which can cause false positives
 */

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const int KRepetitionsCount = 20;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RectTest : public TestBase
{
  protected:

    /*! Returns random number. 
     *  @param  scale Scale of the returned value.
     *  @return Generated random number.
     *  @note Returned number is in [-scale,scale] interval.
     */
    s32 random(s32 scale) const;
    /*! Tests whether given rectangle is NULL.
     *  @param  x       Rectangle X value.
     *  @param  y       Rectangle Y value.
     *  @param  width   Rectangle width.
     *  @param  height  Rectangle height.
     *  @returns Returns TRUE if rectangle is NULL rectangle.
     */
    bool isNull(s32 x, s32 y, s32 width, s32 height) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 RectTest::random(s32 scale) const
{
  return (rand() % (scale << 1)) - scale + 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool RectTest::isNull(s32 x, s32 y, s32 width, s32 height) const
{
  return (0 > width) || (0 > height);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(RectTest, SetValue)
{
  static const s32 KPositionRange = 100;
  static const s32 KSizeRange     = 100;

  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const s32 x       = random(KPositionRange);
    const s32 y       = random(KPositionRange);
    const s32 width   = random(KSizeRange) + KSizeRange + 1;   // +1 to be sure > 0
    const s32 height  = random(KSizeRange) + KSizeRange + 1;   // +1 to be sure > 0

    // setting via constructors
    const Recti rect1(x, y, width, height);
    
    EXPECT_EQ(rect1.x,      x);
    EXPECT_EQ(rect1.y,      y);
    EXPECT_EQ(rect1.width,  width);
    EXPECT_EQ(rect1.height, height);

    const Recti rect2(rect1);
    
    EXPECT_EQ(rect2.x,      x);
    EXPECT_EQ(rect2.y,      y);
    EXPECT_EQ(rect2.width,  width);
    EXPECT_EQ(rect2.height, height);

    // setting via operators
    Recti rect3;
    rect3 = rect2;

    EXPECT_EQ(rect3.x,      x);
    EXPECT_EQ(rect3.y,      y);
    EXPECT_EQ(rect3.width,  width);
    EXPECT_EQ(rect3.height, height);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(RectTest, Empty)
{
  static const s32 KPositionRange = 100;
  static const s32 KSizeRange     = 100;

  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    // different position with 0 width and height
    s32 x       = random(KPositionRange);
    s32 y       = random(KPositionRange);
    s32 width   = 0;
    s32 height  = 0;

    Recti rect(x, y, width, height);
    EXPECT_TRUE(rect.isEmpty());

    // different position with 0 width and positive height
    x       = random(KPositionRange);
    y       = random(KPositionRange);
    width   = 0;
    height  = random(KSizeRange) + KSizeRange + 1;

    rect = Recti(x, y, width, height);
    EXPECT_TRUE(rect.isEmpty());

    // different position with 0 width and negative height
    x       = random(KPositionRange);
    y       = random(KPositionRange);
    width   = 0;
    height  = random(KSizeRange) - KSizeRange - 1;

    rect = Recti(x, y, width, height);
    EXPECT_TRUE(rect.isEmpty());

    // different position with positive width and 0 height
    x       = random(KPositionRange);
    y       = random(KPositionRange);
    width   = random(KSizeRange) + KSizeRange + 1;
    height  = 0;

    rect = Recti(x, y, width, height);
    EXPECT_TRUE(rect.isEmpty());

    // different position with negative width and 0 height
    x       = random(KPositionRange);
    y       = random(KPositionRange);
    width   = random(KSizeRange) - KSizeRange - 1;
    height  = 0;

    rect = Recti(x, y, width, height);
    EXPECT_TRUE(rect.isEmpty());

    // different position with positive width and height
    x       = random(KPositionRange);
    y       = random(KPositionRange);
    width   = random(KSizeRange) + KSizeRange + 1;
    height  = random(KSizeRange) + KSizeRange + 1;

    rect = Recti(x, y, width, height);
    EXPECT_FALSE(rect.isEmpty());

    // different position with negative width and height
    x       = random(KPositionRange);
    y       = random(KPositionRange);
    width   = random(KSizeRange) - KSizeRange - 1;
    height  = random(KSizeRange) - KSizeRange - 1;

    rect = Recti(x, y, width, height);
    EXPECT_FALSE(rect.isEmpty());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(RectTest, Null)
{
  static const s32 KPositionRange = 100;
  static const s32 KSizeRange     = 100;

  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    // different position with 0 width and height
    s32 x       = random(KPositionRange);
    s32 y       = random(KPositionRange);
    s32 width   = 0;
    s32 height  = 0;

    Recti rect(x, y, width, height);
    EXPECT_FALSE(rect.isNull());

    // different position with 0 width and positive height
    x       = random(KPositionRange);
    y       = random(KPositionRange);
    width   = 0;
    height  = random(KSizeRange) + KSizeRange + 1;

    rect = Recti(x, y, width, height);
    EXPECT_FALSE(rect.isNull());

    // different position with 0 width and negative height
    x       = random(KPositionRange);
    y       = random(KPositionRange);
    width   = 0;
    height  = random(KSizeRange) - KSizeRange - 1;

    rect = Recti(x, y, width, height);
    EXPECT_TRUE(rect.isNull());

    // different position with positive width and 0 height
    x       = random(KPositionRange);
    y       = random(KPositionRange);
    width   = random(KSizeRange) + KSizeRange + 1;
    height  = 0;

    rect = Recti(x, y, width, height);
    EXPECT_FALSE(rect.isNull());

    // different position with negative width and 0 height
    x       = random(KPositionRange);
    y       = random(KPositionRange);
    width   = random(KSizeRange) - KSizeRange - 1;
    height  = 0;

    rect = Recti(x, y, width, height);
    EXPECT_TRUE(rect.isNull());

    // different position with positive width and height
    x       = random(KPositionRange);
    y       = random(KPositionRange);
    width   = random(KSizeRange) + KSizeRange + 1;
    height  = random(KSizeRange) + KSizeRange + 1;

    rect = Recti(x, y, width, height);
    EXPECT_FALSE(rect.isNull());

    // different position with negative width and height
    x       = random(KPositionRange);
    y       = random(KPositionRange);
    width   = random(KSizeRange) - KSizeRange - 1;
    height  = random(KSizeRange) - KSizeRange - 1;

    rect = Recti(x, y, width, height);
    EXPECT_TRUE(rect.isNull());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(RectTest, RightEdge)
{
  static const s32 KPositionRange = 100;
  static const s32 KSizeRange     = 100;

  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const s32 x       = random(KPositionRange);
    const s32 y       = random(KPositionRange);
    const s32 width   = random(KSizeRange) + KSizeRange + 1;
    const s32 height  = random(KSizeRange) + KSizeRange + 1;

    Recti rect(x, y, width, height);
    EXPECT_EQ(x + width - 1, rect.right());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(RectTest, LeftEdge)
{
  static const s32 KPositionRange = 100;
  static const s32 KSizeRange     = 100;

  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const s32 x       = random(KPositionRange);
    const s32 y       = random(KPositionRange);
    const s32 width   = random(KSizeRange) + KSizeRange + 1;
    const s32 height  = random(KSizeRange) + KSizeRange + 1;

    Recti rect(x, y, width, height);
    EXPECT_EQ(x, rect.left());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(RectTest, TopEdge)
{
  static const s32 KPositionRange = 100;
  static const s32 KSizeRange     = 100;

  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const s32 x       = random(KPositionRange);
    const s32 y       = random(KPositionRange);
    const s32 width   = random(KSizeRange) + KSizeRange + 1;
    const s32 height  = random(KSizeRange) + KSizeRange + 1;

    Recti rect(x, y, width, height);
    EXPECT_EQ(y, rect.top());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(RectTest, BottomEdge)
{
  static const s32 KPositionRange = 100;
  static const s32 KSizeRange     = 100;

  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const s32 x       = random(KPositionRange);
    const s32 y       = random(KPositionRange);
    const s32 width   = random(KSizeRange) + KSizeRange + 1;
    const s32 height  = random(KSizeRange) + KSizeRange + 1;

    Recti rect(x, y, width, height);
    EXPECT_EQ(y + height - 1, rect.bottom());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(RectTest, Contains)
{
  const s32 x       = 0;
  const s32 y       = 0;
  const s32 width   = 100;
  const s32 height  = 100;

  const Recti rect(x, y, width, height);

  // RECT vs POINT

  // point lying inside
  EXPECT_TRUE(rect.contains(50, 50));
  EXPECT_TRUE(rect.contains(Vector2i(50, 50)));

  // point lying exactly at top-left corner
  EXPECT_TRUE(rect.contains(0, 0));
  EXPECT_TRUE(rect.contains(Vector2i(0, 0)));

  // point lying exactly at top-right corner
  EXPECT_TRUE(rect.contains(width - 1, 0));
  EXPECT_TRUE(rect.contains(Vector2i(width - 1, 0)));

  // point lying exactly at bottom-left corner
  EXPECT_TRUE(rect.contains(0, height - 1));
  EXPECT_TRUE(rect.contains(Vector2i(0, height - 1)));

  // point lying exactly at bottom-right corner
  EXPECT_TRUE(rect.contains(width - 1, height - 1));
  EXPECT_TRUE(rect.contains(Vector2i(width - 1, height - 1)));

  // point lying just outside top-left corner
  EXPECT_FALSE(rect.contains(-1, 0));
  EXPECT_FALSE(rect.contains(-1, -1));
  EXPECT_FALSE(rect.contains(0, -1));
  EXPECT_FALSE(rect.contains(Vector2i(-1, 0)));
  EXPECT_FALSE(rect.contains(Vector2i(-1, -1)));
  EXPECT_FALSE(rect.contains(Vector2i(0, -1)));

  // point lying just outside top-right corner
  EXPECT_FALSE(rect.contains(width, 0));
  EXPECT_FALSE(rect.contains(width - 1, -1));
  EXPECT_FALSE(rect.contains(width, -1));
  EXPECT_FALSE(rect.contains(Vector2i(width, 0)));
  EXPECT_FALSE(rect.contains(Vector2i(width - 1, -1)));
  EXPECT_FALSE(rect.contains(Vector2i(width, -1)));

  // point lying just outside bottom-left corner
  EXPECT_FALSE(rect.contains(0, height));
  EXPECT_FALSE(rect.contains(-1, height));
  EXPECT_FALSE(rect.contains(-1, height - 1));
  EXPECT_FALSE(rect.contains(Vector2i(0, height)));
  EXPECT_FALSE(rect.contains(Vector2i(-1, height)));
  EXPECT_FALSE(rect.contains(Vector2i(-1, height - 1)));

  // point lying just outside bottom-right corner
  EXPECT_FALSE(rect.contains(width, height));
  EXPECT_FALSE(rect.contains(width, height - 1));
  EXPECT_FALSE(rect.contains(width - 1, height));
  EXPECT_FALSE(rect.contains(Vector2i(width, height)));
  EXPECT_FALSE(rect.contains(Vector2i(width, height - 1)));
  EXPECT_FALSE(rect.contains(Vector2i(width - 1, height)));

  // RECT vs RECT

  // same rectangles
  Recti rect2(x, y, width, height);
  EXPECT_TRUE(rect.contains(rect2));
  EXPECT_TRUE(rect2.contains(rect));

  // rectangle wider on the left hand side
  rect2 = Recti(x - 1, y, width, height);
  EXPECT_FALSE(rect.contains(rect2));
  EXPECT_FALSE(rect2.contains(rect));

  // rectangle wider on the right hand side
  rect2 = Recti(x, y, width + 1, height);
  EXPECT_FALSE(rect.contains(rect2));
  EXPECT_TRUE(rect2.contains(rect));

  // rectangle taller on the top side
  rect2 = Recti(x, y - 1, width, height);
  EXPECT_FALSE(rect.contains(rect2));
  EXPECT_FALSE(rect2.contains(rect));

  // rectangle taller on the bottom side
  rect2 = Recti(x, y, width, height + 1);
  EXPECT_FALSE(rect.contains(rect2));
  EXPECT_TRUE(rect2.contains(rect));

  // smaller inside
  rect2 = Recti(x + 1, y + 1, width / 2, height / 2);
  EXPECT_TRUE(rect.contains(rect2));
  EXPECT_FALSE(rect2.contains(rect));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(RectTest, Intersect)
{
  const s32 x       = 0;
  const s32 y       = 0;
  const s32 width   = 100;
  const s32 height  = 100;

  const Recti rect(x, y, width, height);

  // same rectangles
  Recti rect2(x, y, width, height);
  EXPECT_EQ(x, rect.intersect(rect2).x);
  EXPECT_EQ(y, rect.intersect(rect2).y);
  EXPECT_EQ(width, rect.intersect(rect2).width);
  EXPECT_EQ(height, rect.intersect(rect2).height);
  EXPECT_EQ(x, rect2.intersect(rect).x);
  EXPECT_EQ(y, rect2.intersect(rect).y);
  EXPECT_EQ(width, rect2.intersect(rect).width);
  EXPECT_EQ(height, rect2.intersect(rect).height);

  // no intersection (NULL)
  rect2 = Recti(-100, -100, 50, 50);
  EXPECT_FALSE(rect.intersect(rect2).isEmpty());
  EXPECT_FALSE(rect2.intersect(rect).isEmpty());
  EXPECT_TRUE(rect.intersect(rect2).isNull());
  EXPECT_TRUE(rect2.intersect(rect).isNull());

  // no intersection (EMPTY)
  rect2 = Recti(-40, -40, 40, 40);
  EXPECT_TRUE(rect.intersect(rect2).isEmpty());
  EXPECT_TRUE(rect2.intersect(rect).isEmpty());
  EXPECT_FALSE(rect.intersect(rect2).isNull());
  EXPECT_FALSE(rect2.intersect(rect).isNull());

  // regular intersection
  rect2 = Recti(-50, -50, 100, 100);
  EXPECT_EQ(0, rect.intersect(rect2).x);
  EXPECT_EQ(0, rect.intersect(rect2).y);
  EXPECT_EQ(50, rect.intersect(rect2).width);
  EXPECT_EQ(50, rect.intersect(rect2).height);
  EXPECT_EQ(0, rect2.intersect(rect).x);
  EXPECT_EQ(0, rect2.intersect(rect).y);
  EXPECT_EQ(50, rect2.intersect(rect).width);
  EXPECT_EQ(50, rect2.intersect(rect).height);

  // intersection on the left edge
  rect2 = Recti(-29, 20, 30, 40);
  EXPECT_EQ(0, rect.intersect(rect2).x);
  EXPECT_EQ(20, rect.intersect(rect2).y);
  EXPECT_EQ(1, rect.intersect(rect2).width);
  EXPECT_EQ(40, rect.intersect(rect2).height);
  EXPECT_EQ(0, rect2.intersect(rect).x);
  EXPECT_EQ(20, rect2.intersect(rect).y);
  EXPECT_EQ(1, rect2.intersect(rect).width);
  EXPECT_EQ(40, rect2.intersect(rect).height);

  // intersection on the right edge
  rect2 = Recti(99, 20, 30, 40);
  EXPECT_EQ(99, rect.intersect(rect2).x);
  EXPECT_EQ(20, rect.intersect(rect2).y);
  EXPECT_EQ(1, rect.intersect(rect2).width);
  EXPECT_EQ(40, rect.intersect(rect2).height);
  EXPECT_EQ(99, rect2.intersect(rect).x);
  EXPECT_EQ(20, rect2.intersect(rect).y);
  EXPECT_EQ(1, rect2.intersect(rect).width);
  EXPECT_EQ(40, rect2.intersect(rect).height);

  // intersection on the top edge
  rect2 = Recti(20, -29, 30, 30);
  EXPECT_EQ(20, rect.intersect(rect2).x);
  EXPECT_EQ(0, rect.intersect(rect2).y);
  EXPECT_EQ(30, rect.intersect(rect2).width);
  EXPECT_EQ(1, rect.intersect(rect2).height);
  EXPECT_EQ(20, rect2.intersect(rect).x);
  EXPECT_EQ(0, rect2.intersect(rect).y);
  EXPECT_EQ(30, rect2.intersect(rect).width);
  EXPECT_EQ(1, rect2.intersect(rect).height);

  // intersection on the bottom edge
  rect2 = Recti(20, 99, 30, 30);
  EXPECT_EQ(20, rect.intersect(rect2).x);
  EXPECT_EQ(99, rect.intersect(rect2).y);
  EXPECT_EQ(30, rect.intersect(rect2).width);
  EXPECT_EQ(1, rect.intersect(rect2).height);
  EXPECT_EQ(20, rect2.intersect(rect).x);
  EXPECT_EQ(99, rect2.intersect(rect).y);
  EXPECT_EQ(30, rect2.intersect(rect).width);
  EXPECT_EQ(1, rect2.intersect(rect).height);

  // intersection with inner rectangle
  rect2 = Recti(10, 10, 50, 50);
  EXPECT_EQ(10, rect.intersect(rect2).x);
  EXPECT_EQ(10, rect.intersect(rect2).y);
  EXPECT_EQ(50, rect.intersect(rect2).width);
  EXPECT_EQ(50, rect.intersect(rect2).height);
  EXPECT_EQ(10, rect2.intersect(rect).x);
  EXPECT_EQ(10, rect2.intersect(rect).y);
  EXPECT_EQ(50, rect2.intersect(rect).width);
  EXPECT_EQ(50, rect2.intersect(rect).height);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(RectTest, Unite)
{
  const s32 x       = 0;
  const s32 y       = 0;
  const s32 width   = 100;
  const s32 height  = 100;

  const Recti rect(x, y, width, height);

  // same rectangles
  Recti rect2(x, y, width, height);
  EXPECT_EQ(x, rect.unite(rect2).x);
  EXPECT_EQ(y, rect.unite(rect2).y);
  EXPECT_EQ(width, rect.unite(rect2).width);
  EXPECT_EQ(height, rect.unite(rect2).height);
  EXPECT_EQ(x, rect2.unite(rect).x);
  EXPECT_EQ(y, rect2.unite(rect).y);
  EXPECT_EQ(width, rect2.unite(rect).width);
  EXPECT_EQ(height, rect2.unite(rect).height);

  // regular union
  rect2 = Recti(-50, -20, 100, 100);
  EXPECT_EQ(-50, rect.unite(rect2).x);
  EXPECT_EQ(-20, rect.unite(rect2).y);
  EXPECT_EQ(150, rect.unite(rect2).width);
  EXPECT_EQ(120, rect.unite(rect2).height);
  EXPECT_EQ(-50, rect2.unite(rect).x);
  EXPECT_EQ(-20, rect2.unite(rect).y);
  EXPECT_EQ(150, rect2.unite(rect).width);
  EXPECT_EQ(120, rect2.unite(rect).height);

  // with inner rectangle
  rect2 = Recti(10, 10, 50, 50);
  EXPECT_EQ(0, rect.unite(rect2).x);
  EXPECT_EQ(0, rect.unite(rect2).y);
  EXPECT_EQ(100, rect.unite(rect2).width);
  EXPECT_EQ(100, rect.unite(rect2).height);
  EXPECT_EQ(0, rect2.unite(rect).x);
  EXPECT_EQ(0, rect2.unite(rect).y);
  EXPECT_EQ(100, rect2.unite(rect).width);
  EXPECT_EQ(100, rect2.unite(rect).height);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(RectTest, DISABLED_Combine)
{
  // TAGE - rethink this functionality
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(RectTest, Equality)
{
  const Recti rect1(0, 1, 100, 102);

  // same rectangles
  Recti rect2(rect1);
  EXPECT_TRUE(rect1 == rect2);
  EXPECT_TRUE(rect2 == rect1);
  EXPECT_FALSE(rect1 != rect2);
  EXPECT_FALSE(rect2 != rect1);

  // different X coordinate only
  rect2 = Recti(1, 1, 100, 102);
  EXPECT_FALSE(rect1 == rect2);
  EXPECT_FALSE(rect2 == rect1);
  EXPECT_TRUE(rect1 != rect2);
  EXPECT_TRUE(rect2 != rect1);

  // different Y coordinate only
  rect2 = Recti(0, 0, 100, 102);
  EXPECT_FALSE(rect1 == rect2);
  EXPECT_FALSE(rect2 == rect1);
  EXPECT_TRUE(rect1 != rect2);
  EXPECT_TRUE(rect2 != rect1);

  // different WIDTH only
  rect2 = Recti(0, 1, 101, 102);
  EXPECT_FALSE(rect1 == rect2);
  EXPECT_FALSE(rect2 == rect1);
  EXPECT_TRUE(rect1 != rect2);
  EXPECT_TRUE(rect2 != rect1);

  // different HEIGHT only
  rect2 = Recti(0, 1, 100, 101);
  EXPECT_FALSE(rect1 == rect2);
  EXPECT_FALSE(rect2 == rect1);
  EXPECT_TRUE(rect1 != rect2);
  EXPECT_TRUE(rect2 != rect1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------