#ifndef EGE_CORE_VECTOR2_H
#define EGE_CORE_VECTOR2_H

/**
  This class represents 2D vector.
*/

#include "EGETypes.h"
#include "EGEDebug.h"
#include "Core/Math/Math.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
class TVector2
{
	public:

    TVector2();
    TVector2(T x, T y);
		TVector2(const TVector2& vector);

    void operator      *= (T scalar);
    bool operator      == (const TVector2& vector) const;
    bool operator      != (const TVector2& vector) const;
    TVector2& operator += (const TVector2& vector);
    TVector2& operator -= (const TVector2& vector);

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

    /*! Returns vector perpendicular to current one. */
    TVector2 perpendicular() const;

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
TVector2<T>::TVector2() : x(0), y(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector2<T>::TVector2(T x, T y) : x(x), y(y)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector2<T>::TVector2(const TVector2<T>& vector) : x(vector.x), y(vector.y)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline void TVector2<T>::set(T x, T y) 
{ 
  this->x = x; 
  this->y = y; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline T TVector2<T>::length() const
{
  return Math::Sqrt((x * x) + (y * y)); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline T TVector2<T>::lengthSquared() const
{
  return (x * x) + (y * y);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline void TVector2<T>::normalize()
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
inline TVector2<T> TVector2<T>::normalized() const
{
  TVector2<T> out = *this;
  out.normalize();
  
  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline T TVector2<T>::dotProduct(const TVector2& vector) const 
{ 
  return (x * vector.x) + (y * vector.y); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline float32 TVector2<T>::distanceTo(const TVector2& vector) const
{
  return Math::Sqrt((x - vector.x) * (x - vector.x) + (y - vector.y) * (y - vector.y));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline T TVector2<T>::distanceSquaredTo(const TVector2& vector) const
{
  return (x - vector.x) * (x - vector.x) + (y - vector.y) * (y - vector.y);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline TVector2<T> TVector2<T>::perpendicular() const
{
  return TVector2<T>(y, -x);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline bool TVector2<T>::operator == (const TVector2& vector) const
{
  return (x == vector.x) && (y == vector.y);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline bool TVector2<T>::operator != (const TVector2& vector) const
{
  return (x != vector.x) || (y != vector.y);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline void TVector2<T>::operator *= (T scalar)
{
  x *= scalar;
  y *= scalar;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline TVector2<T>& TVector2<T>::operator += (const TVector2& vector)
{
  x += vector.x;
  y += vector.y;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline TVector2<T>& TVector2<T>::operator -= (const TVector2& vector)
{
  x -= vector.x;
  y -= vector.y;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline TVector2<T> operator * (T scalar, const TVector2<T>& right)
{
  return TVector2<T>(right.x * scalar, right.y * scalar);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline TVector2<T> operator * (const TVector2<T>& left, T scalar)
{
  return TVector2<T>(left.x * scalar, left.y * scalar);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline TVector2<T> operator + (const TVector2<T>& left, const TVector2<T>& right)
{
  return TVector2<T>(left.x + right.x, left.y + right.y);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline TVector2<T> operator - (const TVector2<T>& left, const TVector2<T>& right)
{
  return TVector2<T>(left.x - right.x, left.y - right.y);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline TVector2<T> operator * (const TVector2<T>& left, const TVector2<T>& right)
{
  return TVector2<T>(left.x * right.x, left.y * right.y);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline Debug operator << (Debug debug, const TVector2<T>& obj)
{
  debug.nospace() << "Vector2(" << obj.x << ", " << obj.y << ")";
  return debug.space();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_VECTOR2_H