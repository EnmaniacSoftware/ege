#ifndef EGE_CORE_VECTOR4_H
#define EGE_CORE_VECTOR4_H

/**
  This class represents 3D vector in homogenous space.
*/

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include <EGETypes.h>
#include "Core/Math/Math.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
  
template <typename T>
class TVector4 
{
	public:

    TVector4();
    TVector4(T x, T y, T z, T w = 1);
    TVector4(const TVector4& vector);

		inline TVector4 operator = (const TVector4& vector);
		inline void     operator += (const TVector4& vector);
		inline void     operator -= (const TVector4& vector);
    inline bool     operator == (const TVector4& vector) const;
    inline bool     operator != (const TVector4& vector) const;

    /* Sets vector components. */
    inline void set(T x, T y, T z, T w);

    /* Returns vector length. */
    inline T length() const;
    /* Returns vector sequared length. */
    inline T lengthSquared() const;
 
    /* Normalizes the vector. */
    inline void normalize();
    /* Returns normalized vector. */
 	  inline TVector4 normalized() const;

    /* Returns dot product between current and given vectors. */
    inline T dotProduct(const TVector4& vector) const;

    /* Returns distance between this and given points. */
    inline T distanceTo(const TVector4& vector) const;
    /* Returns squared distance between this and given points. */
    inline T distanceSquaredTo(const TVector4& vector) const;

    /* Returns 2D vector consisting of X and Y values of current one. */
    inline TVector2<T> xy() const;
    /* Returns 3D vector consisting of X, Y and Z values of current one. */
    inline TVector3<T> xyz() const;

  public:

    T x;
		T y;
    T z;
    T w;

  public:

    static const TVector4<T> ZERO;
    static const TVector4<T> ONE;
    static const TVector4<T> UNIT_X;
    static const TVector4<T> UNIT_Y;
    static const TVector4<T> UNIT_Z;
    static const TVector4<T> UNIT_W;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
const TVector4<T> TVector4<T>::ZERO   = TVector4<T>(0, 0, 0, 0);
template <typename T>
const TVector4<T> TVector4<T>::ONE    = TVector4<T>(1, 1, 1, 1);
template <typename T>
const TVector4<T> TVector4<T>::UNIT_X = TVector4<T>(1, 0, 0, 1);
template <typename T>
const TVector4<T> TVector4<T>::UNIT_Y = TVector4<T>(0, 1, 0, 1);
template <typename T>
const TVector4<T> TVector4<T>::UNIT_Z = TVector4<T>(0, 0, 1, 1);
template <typename T>
const TVector4<T> TVector4<T>::UNIT_W = TVector4<T>(0, 0, 0, 1);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector4<T>::TVector4() : x(0), y(0), z(0), w(1)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector4<T>::TVector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector4<T>::TVector4(const TVector4<T>& vector) : x(vector.x), y(vector.y), z(vector.z), w(vector.w)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector4<T> TVector4<T>::operator = (const TVector4<T>& vector)
{
  this->x = vector.x;
  this->y = vector.y;
  this->z = vector.z;
  this->w = vector.w;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TVector4<T>::operator += (const TVector4<T>& vector)
{
  x += vector.x;
  y += vector.y;
  z += vector.z;
  w += vector.w;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TVector4<T>::operator -= (const TVector4<T>& vector)
{
  x -= vector.x;
  y -= vector.y;
  z -= vector.z;
  w -= vector.w;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TVector4<T>::operator == (const TVector4& vector) const
{
  return (x == vector.x) && (y == vector.y) && (z == vector.z) && (w == vector.w);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TVector4<T>::operator != (const TVector4& vector) const
{
  return (x != vector.x) || (y != vector.y) || (z != vector.z) || (w != vector.w);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets vector components. */
template <typename T>
void TVector4<T>::set(T x, T y, T z, T w) 
{ 
  this->x = x; 
  this->y = y; 
  this->z = z; 
  this->w = w; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns vector length. */
template <typename T>
T TVector4<T>::length() const
{
  return Math::Sqrt(lengthSquared()); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns vector sequared length. */
template <typename T>
T TVector4<T>::lengthSquared() const
{
  T invW = static_cast<T>(1.0) / w;

  return ((x * x) + (y * y) + (z * z)) * invW * invW;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Normalizes the vector. */
template <typename T>
void TVector4<T>::normalize()
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
    w = 1;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns normalized vector. */
template <typename T>
TVector4<T> TVector4<T>::normalized() const
{
  TVector4<T> out = *this;
  out.normalize();
  
  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns dot product between current and given vectors. */
template <typename T>
T TVector4<T>::dotProduct(const TVector4& vector) const 
{ 
  return (x * vector.x) + (y * vector.y) + (z * vector.z) + (w * vector.w); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns distance between this and given points. */
template <typename T>
T TVector4<T>::distanceTo(const TVector4& vector) const
{
  return Math::Sqrt((x - vector.x) * (x - vector.x) + (y - vector.y) * (y - vector.y) + (z - vector.z) * (z - vector.z) + (w - vector.w) * (w - vector.w));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns squared distance between this and given points. */
template <typename T>
T TVector4<T>::distanceSquaredTo(const TVector4& vector) const
{
  return (x - vector.x) * (x - vector.x) + (y - vector.y) * (y - vector.y) + (z - vector.z) * (z - vector.z) + (w - vector.w) * (w - vector.w);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns 2D vector consisting of X and Y values of current one. */
template <typename T>
TVector2<T> TVector4<T>::xy() const
{
  return TVector2<T>(x, y);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns 3D vector consisting of X, Y and Z values of current one. */
template <typename T>
TVector3<T> TVector4<T>::xyz() const
{
  return TVector3<T>(x, y, z);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector4<T> operator * (const TVector4<T>& left, T scalar)
{
  return TVector4<T>(left.x * scalar, left.y * scalar, left.z * scalar, left.w * scalar);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector4<T> operator * (T scalar, const TVector4<T>& right)
{
  return TVector4<T>(right.x * scalar, right.y * scalar, right.z * scalar, right.w * scalar);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector4<T> operator / (const TVector4<T>& left, T scalar)
{
  scalar = 1.0f / scalar;

  return TVector4<T>(left.x * scalar, left.y * scalar, left.z * scalar, left.w * scalar);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector4<T> operator + (const TVector4<T>& left, const TVector4<T>& right)
{
  return TVector4<T>(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector4<T> operator - (const TVector4<T>& left, const TVector4<T>& right)
{
  return TVector4<T>(left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_VECTOR4_H