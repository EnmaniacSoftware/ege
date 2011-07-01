#ifndef EGE_CORE_QUATERNION_H
#define EGE_CORE_QUATERNION_H

/** Class representing quaternion.
*/

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include <EGETypes.h>
#include "Core/Math/Math.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Angle.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
  
template <typename T>
class TQuaternion
{
  public:

    TQuaternion();
    TQuaternion(T x, T y, T z, T w);
    TQuaternion(const TQuaternion& quat);
//    TQuaternion(const TVector3<T>& cAxis, float fRadians);
//    CQuaternion( const CMatrix3* pcMatrix );
//    TQuaternion( const CVector3* pcAxisX, const CVector3* pcAxisY, const CVector3* pcAxisZ );

		//CQuaternion operator+( const CQuaternion& cQuaternion ) const;
		//CQuaternion operator-( const CQuaternion& cQuaternion ) const;
		//CQuaternion operator/( const CQuaternion& cQuaternion ) const;

  //  CQuaternion operator*( float fNumber ) const;
		//CQuaternion operator/( float fNumber ) const;

		//void operator+=( const CQuaternion& cQuaternion );
		//void operator-=( const CQuaternion& cQuaternion );
		//void operator*=( const CQuaternion& cQuaternion );
		//void operator/=( const CQuaternion& cQuaternion );

  //  void operator*=( float fNumber );
		//void operator/=( float fNumber );

    /* Creates quaternion from rotation along arbitrary axis. */
    void create(const TVector3<T>& axis, const Angle& angle);
    /* Converts into rotation axis and angle. */
    void convertTo(TVector3<T>& axis, Angle& angle) const;
    /* Returns quaternion angle representation. */
    inline Angle angle() const;
    /* Multiplies current quaternion by given one. */
		TQuaternion multiply(const TQuaternion& quat) const;

    //  void create( const CMatrix3* pcMatrix );                                                  // creates from rotation matrix
  //  void create( const CVector3* pcAxisX, const CVector3* pcAxisY, const CVector3* pcAxisZ ); // creates from given axes

  //  // transformation related methods
  //  CVector3 transform( const CVector3* pcVector ) const;                                     // multiplies vector by quaternion

  //  // concatenate related methods
		//inline CQuaternion concatenate( const CQuaternion& cQuaternion ) const                    // multiplies quaternions
  //  {
  //    CVector3 cvOut;
  //    CVector3 cvVec1( x, y, z );
  //    CVector3 cvVec2( cQuaternion.x, cQuaternion.y, cQuaternion.z );

  //    // calculate all coordinates
  //    cvOut = cvVec2*w+cvVec1*cQuaternion.w+cvVec1.getCrossProduct( cvVec2 ); 

  //    return CQuaternion( cvOut.x, cvOut.y, cvOut.z, 
  //                        w*cQuaternion.w-cvVec1.getDotProduct( cvVec2 ) );
  //  }

  //  // DOT product related methods
  //  float getDotProduct( const CQuaternion& cQuaternion ) const;                              // returns DOT product between quaternions

  //  // conversion methods
  //  void convertTo( CMatrix3* pcMatrix ) const;                                               // converts into rotation matrix
  //  void convertTo( CVector3* pcAxes ) const;                                                 // converts into rotation axis and angle

  //  // manipulation methods
  //  float getMagnitude( void ) const;                                                         // gets magnitude
  //  float getSquaredMagnitude( void ) const;                                                  // gets squared magnitude

  //  // normalization related methods
  //  void        normalize( void );                                                            // normalizes itself
  //  CQuaternion getNormalized( void );                                                        // gets normalized quaternion

  //  // inversion related methods
  //  void        inverse( void );                                                              // inverses quaternion (conjugates)
  //  CQuaternion getInverse( void )  const;                                                    // gets inverted quaternion (conjugated)

  //  // helper methods
  //  CQuaternion performSlerp( const CQuaternion& cDestination, float fTime );                 // performs SLERP between quaternions
  //                                                                                            // fTime should be in [0..1] interval

  //  CQuaternion performLerp( const CQuaternion& cDestination, float fTime );                  // performs LERP between quaternions
  //                                                                                            // fTime should be in [0..1] interval

  public:

    T x;
    T y;
    T z;
    T w;

  public:

    static const TQuaternion<T> IDENTITY;
    static const TQuaternion<T> ZERO;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
const TQuaternion<T> TQuaternion<T>::IDENTITY = TQuaternion<T>(0, 0, 0, 1);
template <typename T>
const TQuaternion<T> TQuaternion<T>::ZERO     = TQuaternion<T>(0, 0, 0, 0);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TQuaternion<T>::TQuaternion() : x(0), y(0), z(0), w(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TQuaternion<T>::TQuaternion(T x, T y, T z, T w) : x(x), y(y), z(z), w(w)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TQuaternion<T>::TQuaternion(const TQuaternion& quat) : x(quat.x), y(quat.y), z(quat.z), w(quat.w)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates quaternion from rotation along arbitrary axis. */
template <typename T>
void TQuaternion<T>::create(const TVector3<T>& axis, const Angle& angle)
{
  // axis is unit length
  // angle = A
  // The quaternion representing the rotation is
  //   q = cos(A / 2) + sin(A / 2) * (x * i + y * j + z * k)

  // get half angle
  float32 halfAngle = angle.radians() * 0.5f;

  // calculate the sin(halfAngle) once for optimization
	float32 sin = Math::Sin(halfAngle);
		
	// calculate the x, y and z of the quaternion
	x = axis.x * sin;
	y = axis.y * sin;
	z = axis.z * sin;
	w = Math::Cos(halfAngle);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts into rotation axis and angle. */
template <typename T>
void TQuaternion<T>::convertTo(TVector3<T>& axis, Angle& angle) const
{
  // calculate inverse length of imaginary axes
  T invLength = 1.0 / (x * x + y * y + z * z);

  // check if length is greater than error thershold
  if (Math::DELTA < invLength)
  {
    // calculate axes
    axis.x = x * invLength;
    axis.y = y * invLength;
    axis.z = z * invLength;

    angle.fromRadians(2.0f * Math::ACos(w));
  }
  else
  {
    // length is 0 or errorous
    axis.x = 0;
    axis.y = 0;
    axis.z = 0;

    angle.fromRadians(0);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Multiplies current quaternion by given one. */
template <typename T>
TQuaternion<T> TQuaternion<T>::multiply(const TQuaternion<T>& quat) const
{
  TVector3<T> out;
  TVector3<T> vec1(x, y, z);
  TVector3<T> vec2(quat.x, quat.y, quat.z);

  // calculate all coordinates
  out = vec2 * w + vec1 * quat.w + vec1.crossProduct(vec2); 

  return TQuaternion(out.x, out.y, out.z, w * quat.w - vec1.dotProduct(vec2));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns quaternion angle representation. */
template <typename T>
Angle TQuaternion<T>::angle() const 
{ 
  return Angle::FromRadians(2.0f * Math::ACos(w)); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline TQuaternion<T> operator * (const TQuaternion<T>& left, const TQuaternion<T>& right)
{
  return left.multiply(right);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_QUATERNION_H