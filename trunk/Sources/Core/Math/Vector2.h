#ifndef EGE_CORE_VECTOR2_H
#define EGE_CORE_VECTOR2_H

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class TVector2
{
	public:

    TVector2();
    TVector2(T x, T y);
		TVector2(const TVector2& vector);
		TVector2(const TVector3<T>& vector);
		TVector2(const TVector4<T>& vector);

		//CVector2 operator=( const CVector2& cVector );
		//CVector2 operator+( const CVector2& cVector ) const;
		//CVector2 operator-( const CVector2& cVector ) const;
		//CVector2 operator*( float fNumber ) const;
		//CVector2 operator/( float fNumber ) const;

		//void operator+=( const CVector2& cVector );
		//void operator-=( const CVector2& cVector );

    inline bool operator==(const TVector2 vector) const;
    inline bool operator!=(const TVector2 vector) const;

    T distanceTo(const TVector2& vector) const;
    T squaredDistanceTo(const TVector2& vector) const;

  //  // create related methods
    inline void set(T x, T y);
 
  //  // magnitude related methods
  //  float getMagnitude( void ) const;                                             // returns length of the vector
  //  float getSquaredMagnitude( void ) const;                                      // returns squared length of the vector

  //  // normalization related methods
  //  CVector2 getNormalized( void );                                               // returns normalized vector
  //  void     normalize( void );                                                   // normalizes the vector

  //  // helper methods
  //  inline float getMax( void ) const { return _MAX( x, y ); }                    // gets max value of its components
  //  inline float getMin( void ) const { return _MIN( x, y ); }                    // gets min value of its components

  //  inline CVector2 getPerpendicular( void ) const { return CVector2( -y, x ); }  // gets perpendicular vector

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
template <typename T>
void TVector2<T>::set(T x, T y) 
{ 
  this->x = x; 
  this->y = y; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TVector2<T>::distanceTo(const TVector2& vector) const
{
  return Math::Sqrt((x - vector.x) * (x - vector.x) + (y - vector.y) * (y - vector.y));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TVector2<T>::squaredDistanceTo(const TVector2& vector) const
{
  return (x - vector.x) * (x - vector.x) + (y - vector.y) * (y - vector.y);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TVector2<T>::operator==(const TVector2 vector) const
{
  return x == vector.x && y == vector.y;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TVector2<T>::operator!=(const TVector2 vector) const
{
  return !(*this == vector);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_VECTOR2_H