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

    TQuaternion operator-() const;

    /* Creates quaternion from rotation along arbitrary axis. */
    void create(const TVector3<T>& axis, const Angle& angle);

    /* Converts into rotation axis and angle. */
    void convertTo(TVector3<T>& axis, Angle& angle) const;

    /* Returns length. */
    inline T length() const;
    /* Returns sequared length. */
    inline T lengthSquared() const;

    /* Normalizes quaternion. */
    inline void normalize();

    /* Returns quaternion angle representation. */
    inline Angle angle() const;

    /* Multiplies current quaternion by given one. */
		TQuaternion multiply(const TQuaternion& quat) const;
    /* Returns dot product between this and given quaternion. */
    inline T dotProduct(const TQuaternion& quat) const;

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
template <typename T>
TQuaternion<T> TQuaternion<T>::operator-() const
{
  return TQuaternion(-x, -y, -z, -w);
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
  if (Math::EPSILON < invLength)
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
/*! Returns dot product between this and given quaternion. */
template <typename T>
T TQuaternion<T>::dotProduct(const TQuaternion<T>& quat) const
{
  return (x * quat.x) + (y * quat.y) + (z * quat.z) + (w * quat.w);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Normalizes quaternion. */
template <typename T>
void TQuaternion<T>::normalize()
{
  T factor = 1.0f / length();
  *this = *this * factor;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns length. */
template <typename T>
T TQuaternion<T>::length() const
{
  return Math::Sqrt((x * x) + (y * y) + (z * z) + (w * w));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns sequared length. */
template <typename T>
T TQuaternion<T>::lengthSquared() const
{
  return (x * x) + (y * y) + (z * z) + (w * w);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline TQuaternion<T> operator * (const TQuaternion<T>& left, const TQuaternion<T>& right)
{
  return left.multiply(right);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline TQuaternion<T> operator * (T scalar, const TQuaternion<T>& right)
{
  return TQuaternion<T>(right.x * scalar, right.y * scalar, right.z * scalar, right.w * scalar);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline TQuaternion<T> operator * (const TQuaternion<T>& left, T scalar)
{
  return TQuaternion<T>(left.x * scalar, left.y * scalar, left.z * scalar, left.w * scalar);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline TQuaternion<T> operator + (const TQuaternion<T>& left, const TQuaternion<T>& right)
{
  return TQuaternion<T>(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_QUATERNION_H