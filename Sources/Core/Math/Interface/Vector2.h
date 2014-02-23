#ifndef EGE_CORE_MATH_VECTOR2_H
#define EGE_CORE_MATH_VECTOR2_H

/**
  This class represents 2D vector.
*/

#include "EGETypes.h"
#include "EGEDebug.h"
#include "Core/Math/Interface/Math.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
class TVector2
{
	public:

    TVector2();
    TVector2(T x, T y);
		TVector2(const TVector2& other);

  operators:

    void operator      *= (T scalar);
    bool operator      == (const TVector2& other) const;
    bool operator      != (const TVector2& other) const;
    TVector2& operator += (const TVector2& other);
    TVector2& operator -= (const TVector2& other);
    TVector2  operator  - () const;

  public:

    /*! Sets vector components. */
    void set(T x, T y);

    /*! Returns vector length. */
    T length() const;
    /*! Returns vector sequared length. */
    T lengthSquared() const;
    
    /*! Normalizes vector. */
 	  void normalize();
    /*! Returns normalized vector. */
 	  TVector2 normalized() const;

    /*! Returns dot product between current and given vectors. */
    T dotProduct(const TVector2& vector) const;

    /*! Returns distance between this and given points. */
    float32 distanceTo(const TVector2& vector) const;
    /*! Returns squared distance between this and given points. */
    T distanceSquaredTo(const TVector2& vector) const;

    /*! Returns vector perpendicular to current one. 
     *  @note Returned vector is not normalized.
     */
    TVector2 perpendicular() const;

    /*! Performs linear interpolation between this and given vectors. 
     *  @param  to            Vector to which interpolation is to be performed.
     *  @param  parameter     Scalar in range [0..1] describing relative distance between vectors for which interpolation is to be calculated.
     *  @return Calculated vector.
     */
    TVector2 lerp(const TVector2& to, float32 parameter) const;

    /*! Calculates result of projecting self on the given segment.
     *  @param  segmentPoint1 First point defining line segment.
     *  @param  segmentPoint2 Second point defining line segment.
     *  @return Calculated point.
     *  @note Resulting point always lies on the line segment defined by segmentPoint1 and segmentPoint2.
     */
    TVector2 project(const TVector2& segmentPoint1, const TVector2& segmentPoint2) const;

  public:

    T x;
		T y;

  public:

    static const TVector2<T> ZERO;
    static const TVector2<T> ONE;
    static const TVector2<T> UNIT_X;
    static const TVector2<T> UNIT_Y;
    static const TVector2<T> NEGATIVE_UNIT_X;
    static const TVector2<T> NEGATIVE_UNIT_Y;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
const TVector2<T> TVector2<T>::ZERO             = TVector2<T>(0, 0);
template <typename T>
const TVector2<T> TVector2<T>::ONE              = TVector2<T>(1, 1);
template <typename T>
const TVector2<T> TVector2<T>::UNIT_X           = TVector2<T>(1, 0);
template <typename T>
const TVector2<T> TVector2<T>::UNIT_Y           = TVector2<T>(0, 1);
template <typename T>
const TVector2<T> TVector2<T>::NEGATIVE_UNIT_X  = TVector2<T>(-1, 0);
template <typename T>
const TVector2<T> TVector2<T>::NEGATIVE_UNIT_Y  = TVector2<T>(0, -1);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector2<T>::TVector2() : x(0)
                        , y(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector2<T>::TVector2(T x, T y) : x(x)
                                , y(y)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector2<T>::TVector2(const TVector2<T>& other) : x(other.x)
                                                , y(other.y)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TVector2<T>::set(T x, T y) 
{ 
  this->x = x; 
  this->y = y; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TVector2<T>::length() const
{
  return Math::Sqrt((x * x) + (y * y)); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TVector2<T>::lengthSquared() const
{
  return (x * x) + (y * y);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TVector2<T>::normalize()
{
  T length = this->length();

  // check if can be done
  if (Math::EPSILON <= length)
  {
    // get inverse of length
    T invLength = static_cast<T>(1.0) / length;

    // normalize
	  x *= invLength;
	  y *= invLength;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector2<T> TVector2<T>::normalized() const
{
  TVector2<T> out = *this;
  out.normalize();
  
  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TVector2<T>::dotProduct(const TVector2& vector) const 
{ 
  return (x * vector.x) + (y * vector.y); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
float32 TVector2<T>::distanceTo(const TVector2& vector) const
{
  return Math::Sqrt((x - vector.x) * (x - vector.x) + (y - vector.y) * (y - vector.y));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TVector2<T>::distanceSquaredTo(const TVector2& vector) const
{
  return (x - vector.x) * (x - vector.x) + (y - vector.y) * (y - vector.y);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector2<T> TVector2<T>::perpendicular() const
{
  return TVector2<T>(y, -x);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TVector2<T>::operator == (const TVector2& vector) const
{
  return (x == vector.x) && (y == vector.y);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TVector2<T>::operator != (const TVector2& vector) const
{
  return (x != vector.x) || (y != vector.y);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TVector2<T>::operator *= (T scalar)
{
  x *= scalar;
  y *= scalar;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector2<T>& TVector2<T>::operator += (const TVector2& vector)
{
  x += vector.x;
  y += vector.y;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector2<T>& TVector2<T>::operator -= (const TVector2& vector)
{
  x -= vector.x;
  y -= vector.y;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector2<T> TVector2<T>::operator - () const
{
  return TVector2<T>(-x, -y);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector2<T> TVector2<T>::lerp(const TVector2& to, float32 parameter) const
{
  return (1.0f - parameter) * (*this) + parameter * to;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector2<T> TVector2<T>::project(const TVector2& segmentPoint1, const TVector2& segmentPoint2) const
{
  TVector2 out;

  // calculate vector describing distance between given segment points
  TVector2 lineSegment = segmentPoint2 - segmentPoint1;

  // calculate length of the line segment
  T segmentLength = lineSegment.length();

  // normalize line segment
  // NOTE: by doing so, dot-product of any vector and normalized vector will result on projection of this vector onto normalized one.
  //       Such projection is a scalar describing the length of projected vector and will range from [-X,+X] where X is the length of not normalized vector.
  lineSegment.normalize();

  // calculate vector describing distance between begnining of line segment and current point
  const TVector2 pointSegment = (*this) - segmentPoint1;

  // calculate projection of vector containing our point on given line segment
  T projectedLength = lineSegment.dotProduct(pointSegment);

  if (0 > projectedLength)
  {
    out = segmentPoint1;
  }
  else if (projectedLength >= segmentLength)
  {
    out = segmentPoint2;
  }
  else
  {
    out = segmentPoint1 + lineSegment * projectedLength;
  }

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector2<T> operator * (T scalar, const TVector2<T>& right)
{
  return TVector2<T>(right.x * scalar, right.y * scalar);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector2<T> operator * (const TVector2<T>& left, T scalar)
{
  return TVector2<T>(left.x * scalar, left.y * scalar);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector2<T> operator + (const TVector2<T>& left, const TVector2<T>& right)
{
  return TVector2<T>(left.x + right.x, left.y + right.y);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector2<T> operator - (const TVector2<T>& left, const TVector2<T>& right)
{
  return TVector2<T>(left.x - right.x, left.y - right.y);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector2<T> operator * (const TVector2<T>& left, const TVector2<T>& right)
{
  return TVector2<T>(left.x * right.x, left.y * right.y);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
Debug operator << (Debug debug, const TVector2<T>& obj)
{
  debug.nospace() << "Vector2(" << obj.x << ", " << obj.y << ")";
  return debug.space();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_VECTOR2_H