#ifndef EGE_CORE_MATH_VECTOR4_H
#define EGE_CORE_MATH_VECTOR4_H

/*! This class represents 3D vector in homogenous space.
 */

#include "EGETypes.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
class TVector4 
{
	public:

    TVector4();
    TVector4(T x, T y, T z, T w = 1);
    TVector4(const TVector4& vector);

  operators:

		TVector4& operator  = (const TVector4& other);
		void      operator += (const TVector4& other);
		void      operator -= (const TVector4& other);
    bool      operator == (const TVector4& other) const;
    bool      operator != (const TVector4& other) const;

  public:

    /*! Sets vector components. */
    void set(T x, T y, T z, T w);

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
TVector4<T>::TVector4() : x(0)
                        , y(0)
                        , z(0)
                        , w(1)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector4<T>::TVector4(T x, T y, T z, T w) : x(x)
                                          , y(y)
                                          , z(z)
                                          , w(w)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector4<T>::TVector4(const TVector4<T>& other) : x(other.x)
                                                , y(other.y)
                                                , z(other.z)
                                                , w(other.w)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector4<T>& TVector4<T>::operator = (const TVector4<T>& other)
{
  this->x = other.x;
  this->y = other.y;
  this->z = other.z;
  this->w = other.w;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TVector4<T>::operator += (const TVector4<T>& other)
{
  x += other.x;
  y += other.y;
  z += other.z;
  w += other.w;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TVector4<T>::operator -= (const TVector4<T>& other)
{
  x -= other.x;
  y -= other.y;
  z -= other.z;
  w -= other.w;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TVector4<T>::operator == (const TVector4& other) const
{
  return (x == other.x) && (y == other.y) && (z == other.z) && (w == other.w);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TVector4<T>::operator != (const TVector4& other) const
{
  return (x != other.x) || (y != other.y) || (z != other.z) || (w != other.w);
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
template <typename T>
Debug operator << (Debug debug, const TVector4<T>& obj)
{
  debug.nospace() << "Vector4(" << obj.x << ", " << obj.y << ", " << obj.z << ", " << obj.w << ")";
  return debug.space();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_VECTOR4_H