#ifndef EGE_CORE_QUATERNION_H
#define EGE_CORE_QUATERNION_H

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
  
template <typename T>
class TQuaternion
{
  public:

    TQuaternion();
    TQuaternion(T x, T y, T z, T w);
    TQuaternion(const TQuaternion& cQuat);
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

  //  // creation methods
  //  void create( const CVector3* pcAxis, float fRadians );                                    // creates from rotation along arbitrary axis
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
  //  void convertTo( CVector3* pcAxis, float& fRadians ) const;                                // converts into rotation axis and angle
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


    T x;    // imaginary axis
    T y;    // imaginary axis
    T z;    // imaginary axis
    T w;    // real axis

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
TQuaternion<T>::TQuaternion(const TQuaternion& cQuat) : x(cQuat.x), y(cQuat.y), z(cQuat.z), w(cQuat.w)
{
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_QUATERNION_H