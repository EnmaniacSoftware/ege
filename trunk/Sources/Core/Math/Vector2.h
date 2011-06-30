#ifndef EGE_CORE_VECTOR2_H
#define EGE_CORE_VECTOR2_H

/**
  This class represents 2D vector.
*/

#include <EGE.h>
#include <EGEMath.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class TVector2
{
	public:

    TVector2();
    TVector2(T x, T y);
		TVector2(const TVector2& vector);
		TVector2(const TVector4<T>& vector);
		TVector2(const TVector3<T>& vector);

    inline void operator*=(T scalar);
    inline bool operator==(const TVector2& vector) const;
    inline bool operator!=(const TVector2& vector) const;

    /* Sets vector components. */
    inline void set(T x, T y);

    /* Returns vector length. */
    inline T length() const;
    /* Returns vector sequared length. */
    inline T lengthSquared() const;
    
    /* Normalizes vector. */
 	  inline void normalize();

    /* Returns distance between this and given points. */
    inline T distanceTo(const TVector2& vector) const;
    /* Returns squared distance between this and given points. */
    inline T distanceSquaredTo(const TVector2& vector) const;

  public:

    T x;
		T y;

  public:

    static const TVector2<T> ZERO;
    static const TVector2<T> ONE;
    static const TVector2<T> UNIT_X;
    static const TVector2<T> UNIT_Y;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
const TVector2<T> TVector2<T>::ZERO   = TVector2<T>(0, 0);

template <typename T>
const TVector2<T> TVector2<T>::ONE    = TVector2<T>(1, 1);

template <typename T>
const TVector2<T> TVector2<T>::UNIT_X = TVector2<T>(1, 0);

template <typename T>
const TVector2<T> TVector2<T>::UNIT_Y = TVector2<T>(0, 1);
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
TVector2<T>::TVector2(const TVector3<T>& vector) : x(vector.x), y(vector.y)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector2<T>::TVector2(const TVector4<T>& vector) : x(vector.x), y(vector.y)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets vector components. */
template <typename T>
void TVector2<T>::set(T x, T y) 
{ 
  this->x = x; 
  this->y = y; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns vector length. */
template <typename T>
T TVector2<T>::length() const
{
  return Math::Sqrt((x * x) + (y * y)); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns vector sequared length. */
template <typename T>
T TVector2<T>::lengthSquared() const
{
  return (x * x) + (y * y);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Normalizes vector. */
template <typename T>
void TVector2<T>::normalize()
{
  T length = this->length();

  // check if can be done
  if (Math::DELTA <= length)
  {
    // get inverse of length
    T invLength = static_cast<T>(1.0) / length;

    // normalize
	  x *= invLength;
	  y *= invLength;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns distance between this and given points. */
template <typename T>
T TVector2<T>::distanceTo(const TVector2& vector) const
{
  return Math::Sqrt((x - vector.x) * (x - vector.x) + (y - vector.y) * (y - vector.y));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns squared distance between this and given points. */
template <typename T>
T TVector2<T>::distanceSquaredTo(const TVector2& vector) const
{
  return (x - vector.x) * (x - vector.x) + (y - vector.y) * (y - vector.y);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TVector2<T>::operator==(const TVector2& vector) const
{
  return (x == vector.x) && (y == vector.y);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TVector2<T>::operator!=(const TVector2& vector) const
{
  return (x != vector.x) || (y != vector.y);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TVector2<T>::operator*=(T scalar)
{
  x *= scalar;
  y *= scalar;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_VECTOR2_H