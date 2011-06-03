#ifndef EGE_CORE_VECTOR3_H
#define EGE_CORE_VECTOR3_H

#include "EGE.h"
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

		inline void operator+=(const TVector3& vector);
		inline void operator-=(const TVector3& vector);

    /*! Sets vector data. */
    inline void set(T x, T y, T z) { this->x = x; this->y = y; this->z = z; }
    /*! Returns vector length. */
    inline T length() const { return Math::Sqrt(lengthSquared()); }
    /*! Returns vector sequared length. */
    inline T lengthSquared() const { return (x * x) + (y * y) + (z * z); }
    /* Normalizes vector. */
 	  void normalize();
    /*! Returns DOT product between current and given vectors. */
    inline T dotProduct(const TVector3& vector) const { return x * vector.x + y * vector.y + z * vector.z; }
    /* Returns CROSS product between current and given vectors. */
    TVector3 crossProduct(const TVector3& vector) const;

  //  // normalization related methods
		//CVector3 getNormalized( void ) const;                                                     // gets normalized vector

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
TVector3<T>::TVector3(const TVector3& vector) : x(vector.x), y(vector.y), z(vector.z)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Normalizes vector. */
template <typename T>
void TVector3<T>::normalize()
{
  // get length
  T length = this->length();

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
void TVector3<T>::operator+=(const TVector3<T>& vector)
{
  x += vector.x;
  y += vector.y;
  z += vector.z;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TVector3<T>::operator-=(const TVector3<T>& vector)
{
  x -= vector.x;
  y -= vector.y;
  z -= vector.z;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns CROSS product between current and given vectors. */
template <typename T>
TVector3<T> TVector3<T>::crossProduct(const TVector3<T>& vector) const 
{ 
  return TVector3<T>((y * vector.z) - (z * vector.y), (z * vector.x) - (x * vector.z), (x * vector.y) - (y * vector.x)); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline TVector3<T> operator+(const TVector3<T>& left, const TVector3<T>& right)
{
  return TVector3<T>(left.x + right.x, left.y + right.y, left.z + left.z);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline TVector3<T> operator*(const TVector3<T>& left, T scalar)
{
  return TVector3<T>(left.x * scalar, left.y * scalar, left.z * scalar);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_VECTOR3_H