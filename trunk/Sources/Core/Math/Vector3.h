#ifndef EGE_CORE_VECTOR3_H
#define EGE_CORE_VECTOR3_H

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class TVector3 
{
	public:

    TVector3();
    TVector3(T x, T y, T z);
    TVector3(const TVector3& cVector);

		//CVector3 operator=( const CVector3& cVector );
		//CVector3 operator+( const CVector3& cVector ) const;
		//CVector3 operator-( const CVector3& cVector ) const;
		//CVector3 operator*( const CVector3& cVector ) const;
		//CVector3 operator*( float fNumber ) const;
		//CVector3 operator/( float fNumber ) const;
  //  CVector3 operator-( void ) const;

		inline void operator+=(const TVector3& cVector);
		inline void operator-=(const TVector3& cVector);
		//void operator*=( float fNumber );
		//void operator/=( float fNumber );

  //  bool operator==( const CVector3& cVector ) const;
  //  bool operator!=( const CVector3& cVector ) const;

   inline void set(T x, T y, T z);

   inline T getLength() const;
   inline T getLengthSquare() const;

  //  // normalization related methods
		//CVector3 getNormalized( void ) const;                                                     // gets normalized vector
	 inline void normalize();

  //  // DOT product related methods
  //  inline float getDotProduct( const CVector3& cVector ) const                               // returns DOT product between vectors
  //  {
  //    return x*cVector.x+y*cVector.y+z*cVector.z;
  //  }

  //  inline float absDotProduct( const CVector3& cVector ) const                               // returns absolute DOT product between vectors
  //  {
  //    return Math::FastAbs( x*cVector.x )+
  //           Math::FastAbs( y*cVector.y )+
  //           Math::FastAbs( z*cVector.z );
  //  }

  //  // CROSS product related methods
  //  inline CVector3 CVector3::getCrossProduct( const CVector3& cVector ) const                // gets cross product of vectors
  //  {
  //    return CVector3( ( y*cVector.z )-( z*cVector.y ), 
  //                     ( z*cVector.x )-( x*cVector.z ),
  //                     ( x*cVector.y )-( y*cVector.x ) );
  //  }

  //  // helper methods
  //  CQuaternion getRotationTo( const CVector3& cDest,                                         // gets shortest rotation to given vector
  //                             const CVector3& cFallbackAxis = CVector3::ZERO ) const;

  //  CVector3 getRandomDeviant( const float& fAngle,                                           // gets vector randomly deviant from itself
  //                             const CVector3& cUp = CVector3::ZERO ) const;

  //  CVector3 getPerpendicular( void ) const;                                                  // gets perpendicular vector to itself

  //  float getAngleBetween( const CVector3& cVector ) const;                                   // returns angle (in radians) between vectors

  //  inline void makeFloor( const CVector3& cVector )                                          // sets this vector's components to the minimum
  //  {                                                                                         // of its own and the ones of the passed in vector
  //    if ( cVector.x < x ) x = cVector.x;
  //    if ( cVector.y < y ) y = cVector.y;
  //    if ( cVector.z < z ) z = cVector.z;
  //  }

  //  inline void makeCeil( const CVector3& cVector )                                           // sets this vector's components to the maximum
  //  {                                                                                         // of its own and the ones of the passed in vector
  //    if ( cVector.x > x ) x = cVector.x;
  //    if ( cVector.y > y ) y = cVector.y;
  //    if ( cVector.z > z ) z = cVector.z;
  //  }

  //  inline bool isZeroLength( void ) const                                                    // returns TRUE if vector is zero in length
  //  { 
  //    return getSquaredMagnitude() < Math::Pow2( Math::DELTA ); 
  //  }

  //  inline float getMax( void ) const { return _MAX( _MAX( x, y ), z ); }                     // gets max value of its components
  //  inline float getMin( void ) const { return _MIN( _MIN( x, y ), z ); }                     // gets min value of its components


		T x;
		T y;
		T z;

    static const TVector3<T> ZERO;
    static const TVector3<T> UNIT_X;
    static const TVector3<T> UNIT_Y;
    static const TVector3<T> UNIT_Z;
    static const TVector3<T> NEGATIVE_UNIT_X;
    static const TVector3<T> NEGATIVE_UNIT_Y;
    static const TVector3<T> NEGATIVE_UNIT_Z;
    static const TVector3<T> ONE;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
const TVector3<T> TVector3<T>::ZERO            = TVector3<T>(0, 0, 0);

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

template <typename T>
const TVector3<T> TVector3<T>::ONE             = TVector3<T>(1, 1, 1);
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
TVector3<T>::TVector3(const TVector3& cVector) : x(cVector.x), y(cVector.y), z(cVector.z)
{
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
T TVector3<T>::getLength() const 
{ 
  return Math::Sqrt(getLengthSquare()); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TVector3<T>::getLengthSquare() const 
{ 
  return (x * x) + (y * y) + (z * z); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TVector3<T>::normalize()
{
  // get length
  T length = getLength();

  // check if can be done
  if (Math::DELTA <= length)
  {
    // normalize
	  x /= length;
	  y /= length;
	  z /= length;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TVector3<T>::operator+=(const TVector3<T>& cVector)
{
  x += cVector.x;
  y += cVector.y;
  z += cVector.z;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TVector3<T>::operator-=(const TVector3<T>& cVector)
{
  x -= cVector.x;
  y -= cVector.y;
  z -= cVector.z;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_VECTOR3_H