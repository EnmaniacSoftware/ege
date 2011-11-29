#ifndef EGE_CORE_VECTOR3_H
#define EGE_CORE_VECTOR3_H

/**
  This class represents 3D vector.
*/

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include <EGETypes.h>
#include "Core/Math/Math.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class TVector3 
{
	public:

    TVector3();
    TVector3(T x, T y, T z);
    TVector3(const TVector3& vector);

		inline void       operator += (const TVector3& vector);
		inline void       operator -= (const TVector3& vector);
    inline bool       operator == (const TVector3& vector) const;
    inline bool       operator != (const TVector3& vector) const;
    inline TVector3&  operator *= (const T scalar);
    inline TVector3   operator - () const;

    /* Sets vector data. */
    inline void set(T x, T y, T z);
    
    /* Returns vector length. */
    inline T length() const;
    /* Returns vector sequared length. */
    inline T lengthSquared() const;
    
    /* Normalizes vector. */
 	  inline void normalize();
    /* Returns normalized vector. */
 	  inline TVector3 normalized() const;
    
    /* Returns dot product between current and given vectors. */
    inline T dotProduct(const TVector3& vector) const;
    /* Returns cross product between current and given vectors. */
    inline TVector3 crossProduct(const TVector3& vector) const;

    /* Returns distance between this and given points. */
    inline T distanceTo(const TVector3& vector) const;
    /* Returns squared distance between this and given points. */
    inline T distanceSquaredTo(const TVector3& vector) const;

    /* Returns 2D vector consisting of X and Y values of current one. */
    inline TVector2<T> xy() const;

    /* Returns vector perpendicular to current one. */
    inline TVector3 perpendicular() const;

  //  // helper methods
  //  CQuaternion getRotationTo( const CVector3& cDest,                                         // gets shortest rotation to given vector
  //                             const CVector3& cFallbackAxis = CVector3::ZERO ) const;

  //  float getAngleBetween( const CVector3& cVector ) const;                                   // returns angle (in radians) between vectors

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
/*! Sets vector components. */
template <typename T>
void TVector3<T>::set(T x, T y, T z) 
{ 
  this->x = x; 
  this->y = y;
  this->z = z;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns vector length. */
template <typename T>
T TVector3<T>::length() const
{
  return Math::Sqrt((x * x) + (y * y) + (z * z)); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns vector sequared length. */
template <typename T>
T TVector3<T>::lengthSquared() const
{
  return (x * x) + (y * y) + (z * z);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Normalizes vector. */
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
/*! Returns normalized vector. */
template <typename T>
TVector3<T> TVector3<T>::normalized() const
{
  TVector3<T> out = *this;
  out.normalize();
  
  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns cross product between current and given vectors. */
template <typename T>
TVector3<T> TVector3<T>::crossProduct(const TVector3<T>& vector) const 
{ 
  return TVector3<T>((y * vector.z) - (z * vector.y), (z * vector.x) - (x * vector.z), (x * vector.y) - (y * vector.x)); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns dot product between current and given vectors. */
template <typename T>
T TVector3<T>::dotProduct(const TVector3& vector) const 
{ 
  return (x * vector.x) + (y * vector.y) + (z * vector.z); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns distance between this and given points. */
template <typename T>
T TVector3<T>::distanceTo(const TVector3& vector) const
{
  return Math::Sqrt((x - vector.x) * (x - vector.x) + (y - vector.y) * (y - vector.y) + (z - vector.z) * (z - vector.z));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns squared distance between this and given points. */
template <typename T>
T TVector3<T>::distanceSquaredTo(const TVector3& vector) const
{
  return (x - vector.x) * (x - vector.x) + (y - vector.y) * (y - vector.y) + (z - vector.z) * (z - vector.z);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns vector perpendicular to current one. */
template <typename T>
TVector3<T> TVector3<T>::perpendicular() const
{
  TVector3 perp = crossProduct(TVector3::UNIT_X);

  // check length
  if (Math::EPSILON_SQUARED > perp.lengthSquared())
  {
    // this vector is the Y axis multiplied by a scalar, so we have to use another axis
    perp = crossProduct(TVector3::UNIT_Y);
  }

  // normalize
  perp.normalize();
  return perp;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns 2D vector consisting of X and Y values of current one. */
template <typename T>
TVector2<T> TVector3<T>::xy() const
{
  return TVector2<T>(x, y);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline TVector3<T> operator + (const TVector3<T>& left, const TVector3<T>& right)
{
  return TVector3<T>(left.x + right.x, left.y + right.y, left.z + right.z);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline TVector3<T> operator - (const TVector3<T>& left, const TVector3<T>& right)
{
  return TVector3<T>(left.x - right.x, left.y - right.y, left.z - right.z);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline TVector3<T> operator * (const TVector3<T>& left, T scalar)
{
  return TVector3<T>(left.x * scalar, left.y * scalar, left.z * scalar);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline TVector3<T> operator * (const TVector3<T>& left, const TVector3<T>& right)
{
  return TVector3<T>(left.x * right.x, left.y * right.y, left.z * right.z);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_VECTOR3_H