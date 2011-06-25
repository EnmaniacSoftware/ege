#ifndef EGE_CORE_VECTOR4_H
#define EGE_CORE_VECTOR4_H

// NOTE: this class represents 3D vector in homogenous space

#include "EGE.h"
#include "Core/Math/Math.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
  
template <typename T>
class TVector4 
{
	public:

    TVector4();
    TVector4(T x, T y, T z, T w);
    TVector4(const TVector4& vector);
    TVector4(const TVector3<T>& vector);
    TVector4(const TVector2<T>& vector);

		inline void operator+=(const TVector4& vector);
		inline void operator-=(const TVector4& vector);

    inline bool operator==(const TVector4 vector) const;
    inline bool operator!=(const TVector4 vector) const;

    inline void set(T x, T y, T z, T w);

    inline T length() const;
    inline T lengthSquare() const;
 
    inline void normalize();


    T x;
		T y;
    T z;
    T w;

    static const TVector4<T> ZERO;
    static const TVector4<T> ONE;
    static const TVector4<T> UNIT_X;
    static const TVector4<T> UNIT_Y;
    static const TVector4<T> UNIT_Z;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
const TVector4<T> TVector4<T>::ZERO   = TVector4<T>(0, 0, 0, 1);

template <typename T>
const TVector4<T> TVector4<T>::ONE    = TVector4<T>(1, 1, 1, 1);

template <typename T>
const TVector4<T> TVector4<T>::UNIT_X = TVector4<T>(1, 0, 0, 1);

template <typename T>
const TVector4<T> TVector4<T>::UNIT_Y = TVector4<T>(0, 1, 0, 1);

template <typename T>
const TVector4<T> TVector4<T>::UNIT_Z = TVector4<T>(0, 0, 1, 1);
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
TVector4<T>::TVector4(const TVector3<T>& vector) : x(vector.x), y(vector.y), z(vector.z), w(1)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector4<T>::TVector4(const TVector2<T>& vector) : x(vector.x), y(vector.y), z(0), w(1)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TVector4<T>::set(T x, T y, T z, T w) 
{ 
  this->x = x; 
  this->y = y; 
  this->z = z; 
  this->w = w; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TVector4<T>::operator+=(const TVector4<T>& vector)
{
  x += vector.x;
  y += vector.y;
  z += vector.z;
  w += vector.w;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TVector4<T>::operator-=(const TVector4<T>& vector)
{
  x -= vector.x;
  y -= vector.y;
  z -= vector.z;
  w -= vector.w;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TVector4<T>::operator==(const TVector4 vector) const
{
  return x == vector.x && y == vector.y && z == vector.z && w == vector.w;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TVector4<T>::operator!=(const TVector4 vector) const
{
  return !(*this == vector);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TVector4<T>::length() const
{
  return Math::Sqrt(lengthSquare()); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TVector4<T>::lengthSquare() const
{
  T invW = 1.0f / w;

  return ((x * x) + (y * y) + (z * z)) * invW * invW;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TVector4<T>::normalize()
{
  // get length
  T curLength = length();

  // check if can be done
  if (Math::DELTA <= curLength)
  {
    // normalize
	  x /= curLength;
	  y /= curLength;
	  z /= curLength;
    w = 1;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector4<T> operator*(const TVector4<T>& left, T scalar)
{
  return TVector4<T>(left.x * scalar, left.y * scalar, left.z * scalar, left.w * scalar);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector4<T> operator/(const TVector4<T>& left, T scalar)
{
  scalar = 1.0f / scalar;

  return TVector4<T>(left.x * scalar, left.y * scalar, left.z * scalar, left.w * scalar);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector4<T> operator+(const TVector4<T>& left, const TVector4<T>& right)
{
  return TVector4<T>(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector4<T> operator-(const TVector4<T>& left, const TVector4<T>& right)
{
  return TVector4<T>(left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_VECTOR4_H