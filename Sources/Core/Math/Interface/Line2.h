#ifndef EGE_CORE_MATH_LINE2_H
#define EGE_CORE_MATH_LINE2_H

/*! This class represents 2D line.
 */

#include "EGETypes.h"
#include "EGEDebug.h"
#include "EGEVector2.h"
#include "EGEMath.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
class Line2
{
	public:

    Line2();
    Line2(const TVector2<T>& point1, const TVector2<T>& point2);
		Line2(const Line2& other);

  public:

    /*! Calculates point on intersection of this and another lines.
     *  @param  out           Calculated point of intersection.
     *  @param  other         Line to check for intersections with.
     *  @return Returns TRUE if point of intersection was found. Otherwise FALSE.
     *  @note If return value is FALSE, out value is undefined.
     */
    bool intersects(TVector2<T>& out, const Line2& other) const;

  public:

    /*! Offset. Point through which line passes. */
    TVector2<T> offset;
    /*! Direction. */
    TVector2<T> dir;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
Line2<T>::Line2()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
Line2<T>::Line2(const TVector2<T>& point1, const TVector2<T>& point2)
{
  offset = point1;

  dir = point2 - point1;
  dir.normalize();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
Line2<T>::Line2(const Line2<T>& other) : offset(other.offset)
                                       , dir(other.dir)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool Line2<T>::intersects(TVector2<T>& out, const Line2& other) const
{
  bool result = false;

  // TAGE - perhaps it can be optimized
  const TVector2<T> line1PointA = offset;
  const TVector2<T> line1PointB = offset + dir;
  const TVector2<T> line2PointA = other.offset;
  const TVector2<T> line2PointB = other.offset + other.dir;

  // calculate denotmiator
  const T denominator = (line1PointA.x - line1PointB.x) * (line2PointA.y - line2PointB.y) - 
                        (line1PointA.y - line1PointB.y) * (line2PointA.x - line2PointB.x);
 
  if (Math::EPSILON <= Math::Abs(denominator))
  {  
    // calculate some coefficient for optimizaton purposes
    const T nominatorCoe1  = (line1PointA.x * line1PointB.y - line1PointA.y * line1PointB.x);
    const T nominatorCoe2  = (line2PointA.x * line2PointB.y - line2PointA.y * line2PointB.x);
    const T invDenominator = 1.0f / denominator;

    out = (nominatorCoe1 * (line2PointA - line2PointB) - (line1PointA - line1PointB) * nominatorCoe2) * invDenominator;
  
    // done
    result = true;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
Debug operator << (Debug debug, const Line2<T>& obj)
{
  debug.nospace() << "Line2(" << obj.offset << ", " << obj.dir << ")";
  return debug.space();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_LINE2_H