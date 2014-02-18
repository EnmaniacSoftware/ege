#ifndef EGE_CORE_MATH_VECTOR3_H
#define EGE_CORE_MATH_VECTOR3_H

/**
  This class represents 3D vector.
*/

#include "EGETypes.h"
#include "EGEDebug.h"
#include "Core/Math/Interface/Math.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
class TVector3 
{
	public:

    TVector3();
    TVector3(T x, T y, T z);
    TVector3(const TVector3& vector);

  operators:

		void       operator += (const TVector3& vector);
		void       operator -= (const TVector3& vector);
    bool       operator == (const TVector3& vector) const;
    bool       operator != (const TVector3& vector) const;
    TVector3&  operator *= (const T scalar);
    TVector3   operator - () const;

  public:

    /*! Sets vector data. */
    void set(T x, T y, T z);
    
    /*! Returns vector length. */
    T length() const;
    /*! Returns vector sequared length. */
    T lengthSquared() const;
    
    /*! Normalizes vector. */
 	  void normalize();
    /*! Returns normalized vector. */
 	  TVector3 normalized() const;
    
    /*! Returns dot product between current and given vectors. */
    T dotProduct(const TVector3& vector) const;
    /*! Returns cross product between current and given vectors. */
    TVector3 crossProduct(const TVector3& vector) const;

    /*! Returns distance between this and given points. */
    T distanceTo(const TVector3& vector) const;
    /*! Returns squared distance between this and given points. */
    T distanceSquaredTo(const TVector3& vector) const;

    /*! Returns vector perpendicular to current one. 
     *  @note Returned vector is not normalized.
     */
    TVector3 perpendicular() const;

    /*! Performs linear interpolation between this and given vectors. 
     *  @param  to            Vector to which interpolation is to be performed.
     *  @param  parameter     Scalar in range [0..1] describing relative distance between vectors for which interpolation is to be calculated.
     *  @return Calculated vector.
     */
    TVector3 lerp(const TVector3& to, float32 parameter) const;

    /*! Calculates result of projecting self on the given segment.
     *  @param  segmentPoint1 First point defining line segment.
     *  @param  segmentPoint2 Second point defining line segment.
     *  @return Calculated point.
     *  @note Resulting point always lies on the line segment defined by segmentPoint1 and segmentPoint2.
     */
    TVector3 project(const TVector3& segmentPoint1, const TVector3& segmentPoint2) const;

  public:

		T x;
		T y;
		T z;

  public:

    static const TVector3<T> ZERO;
    static const TVector3<T> ONE;
    static const TVector3<T> UNIT_X;
    static const TVector3<T> UNIT_Y;
    static const TVector3<T> UNIT_Z;
    static const TVector3<T> NEGATIVE_UNIT_X;
    static const TVector3<T> NEGATIVE_UNIT_Y;
    static const TVector3<T> NEGATIVE_UNIT_Z;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
const TVector3<T> TVector3<T>::ZERO            = TVector3<T>(0, 0, 0);
template <typename T>
const TVector3<T> TVector3<T>::ONE             = TVector3<T>(1, 1, 1);
template <typename T>
const TVector3<T> TVector3<T>::UNIT_X          = TVector3<T>(1, 0, 0);
template <typename T>
const TVector3<T> TVector3<T>::UNIT_Y          = TVector3<T>(0, 1, 0);
template <typename T>
const TVector3<T> TVector3<T>::UNIT_Z          = TVector3<T>(0, 0, 1);
template <typename T>
const TVector3<T> TVector3<T>::NEGATIVE_UNIT_X = TVector3<T>(-1, 0, 0);
template <typename T>
const TVector3<T> TVector3<T>::NEGATIVE_UNIT_Y = TVector3<T>(0, -1, 0);
template <typename T>
const TVector3<T> TVector3<T>::NEGATIVE_UNIT_Z = TVector3<T>(0, 0, -1);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector3<T>::TVector3() : x(0), y(0), z(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector3<T>::TVector3(T x, T y, T z) : x(x), y(y), z(z)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector3<T>::TVector3(const TVector3& vector) : x(vector.x), y(vector.y), z(vector.z)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TVector3<T>::operator += (const TVector3<T>& vector)
{
  x += vector.x;
  y += vector.y;
  z += vector.z;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TVector3<T>::operator -= (const TVector3<T>& vector)
{
  x -= vector.x;
  y -= vector.y;
  z -= vector.z;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TVector3<T>::operator == (const TVector3& vector) const
{
  return (x == vector.x) && (y == vector.y) && (z == vector.z);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TVector3<T>::operator != (const TVector3& vector) const
{
  return (x != vector.x) || (y != vector.y) || (z != vector.z);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector3<T>& TVector3<T>::operator *= (const T scalar)
{
  x *= scalar;
  y *= scalar;
  z *= scalar;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector3<T> TVector3<T>::operator - () const
{
  return TVector3<T>(-x, -y, -z);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TVector3<T>::set(T x, T y, T z) 
{ 
  this->x = x; 
  this->y = y;
  this->z = z;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TVector3<T>::length() const
{
  return Math::Sqrt(lengthSquared()); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TVector3<T>::lengthSquared() const
{
  return (x * x) + (y * y) + (z * z);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TVector3<T>::normalize()
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
	  z *= invLength;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector3<T> TVector3<T>::normalized() const
{
  TVector3<T> out = *this;
  out.normalize();
  
  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector3<T> TVector3<T>::crossProduct(const TVector3<T>& vector) const 
{ 
  return TVector3<T>((y * vector.z) - (z * vector.y), (z * vector.x) - (x * vector.z), (x * vector.y) - (y * vector.x)); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TVector3<T>::dotProduct(const TVector3& vector) const 
{ 
  return (x * vector.x) + (y * vector.y) + (z * vector.z); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TVector3<T>::distanceTo(const TVector3& vector) const
{
  return Math::Sqrt((x - vector.x) * (x - vector.x) + (y - vector.y) * (y - vector.y) + (z - vector.z) * (z - vector.z));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TVector3<T>::distanceSquaredTo(const TVector3& vector) const
{
  return (x - vector.x) * (x - vector.x) + (y - vector.y) * (y - vector.y) + (z - vector.z) * (z - vector.z);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector3<T> TVector3<T>::perpendicular() const
{
  TVector3 out = crossProduct(TVector3::UNIT_X);

  // check length
  if (Math::EPSILON_SQUARED > out.lengthSquared())
  {
    // this vector is the Y axis multiplied by a scalar, so we have to use another axis
    out = crossProduct(TVector3::UNIT_Y);
  }

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector3<T> TVector3<T>::lerp(const TVector3& to, float32 parameter) const
{
  return (1.0f - parameter) * (*this) + parameter * to;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector3<T> TVector3<T>::project(const TVector3& segmentPoint1, const TVector3& segmentPoint2) const
{
  TVector3 out;

  // calculate vector describing distance between given segment points
  TVector3 lineSegment = segmentPoint2 - segmentPoint1;

  // calculate length of the line segment
  T segmentLength = lineSegment.length();

  // normalize line segment
  // NOTE: by doing so, dot-product of any vector and normalized vector will result on projection of this vector onto normalized one.
  //       Such projection is a scalar describing the length of projected vector and will range from [-X,+X] where X is the length of not normalized vector.
  lineSegment.normalize();

  // calculate vector describing distance between begnining of line segment and current point
  const TVector3 pointSegment = (*this) - segmentPoint1;

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
TVector3<T> operator * (T scalar, const TVector3<T>& right)
{
  return TVector3<T>(right.x * scalar, right.y * scalar, right.z * scalar);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector3<T> operator * (const TVector3<T>& left, T scalar)
{
  return TVector3<T>(left.x * scalar, left.y * scalar, left.z * scalar);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector3<T> operator + (const TVector3<T>& left, const TVector3<T>& right)
{
  return TVector3<T>(left.x + right.x, left.y + right.y, left.z + right.z);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector3<T> operator - (const TVector3<T>& left, const TVector3<T>& right)
{
  return TVector3<T>(left.x - right.x, left.y - right.y, left.z - right.z);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector3<T> operator * (const TVector3<T>& left, const TVector3<T>& right)
{
  return TVector3<T>(left.x * right.x, left.y * right.y, left.z * right.z);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
Debug operator << (Debug debug, const TVector3<T>& obj)
{
  debug.nospace() << "Vector3(" << obj.x << ", " << obj.y << ", " << obj.z << ")";
  return debug.space();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_VECTOR3_H