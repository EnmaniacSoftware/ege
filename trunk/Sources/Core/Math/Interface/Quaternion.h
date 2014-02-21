#ifndef EGE_CORE_MATH_QUATERNION_H
#define EGE_CORE_MATH_QUATERNION_H

/** Class representing quaternion. Quaternion is defined as following: q = xi + yj + zk + w.
 *  While using unit quaternions for rotations in 3D space the following holds:
 *  if           q =   xi + yj + zk + w rotates from A to B
 *  then       - q = - xi - yj - zk - w rotates from A to B (different path)
 *  then   conj(q) = - xi - yj - zk + w rotates from B to A
 *  then - conj(q) = + xi + yj + zk - w rotates from B to A (different path)
 */

#include "EGETypes.h"
#include "Core/Math/Interface/Math.h"
#include "Core/Math/Interface/Vector3.h"
#include "Core/Math/Interface/Matrix4.h"
#include "Core/Math/Interface/Angle.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
class TQuaternion
{
  public:

    TQuaternion();
    TQuaternion(T x, T y, T z, T w);
    TQuaternion(const TQuaternion& other);
    TQuaternion(const TVector3<T>& axis, const Angle& angle);
    TQuaternion(const TMatrix4<T>& matrix);

  operators:

    bool        operator == (const TQuaternion& other) const;
    TQuaternion operator - () const;

  public:

    /*! Creates quaternion from rotation along arbitrary axis. */
    void create(const TVector3<T>& axis, const Angle& angle);

    /*! Converts into rotation axis and angle. */
    void convertTo(TVector3<T>& axis, Angle& angle) const;
    /*! Converts into rotation matrix. */
    void convertTo(TMatrix4<T>& matrix) const;

    /*! Returns length. */
    T length() const;
    /*! Returns sequared length. */
    T lengthSquared() const;

    /*! Normalizes quaternion. */
    void normalize();

    /*! Returns quaternion angle representation. */
    Angle angle() const;

    /*! Multiplies current quaternion by given one. */
		TQuaternion multiply(const TQuaternion& quat) const;
    /*! Returns dot product between this and given quaternion. */
    T dotProduct(const TQuaternion& quat) const;

    /*! Returns quaternion representing conjugate of this one. */ 
    TQuaternion conjugated() const;
    /*! Conjugates (inverses) self. */ 
    void conjugate();

    /*! Performs spherical linear interpolation between this and given quaternions. 
     *  @param  to            Quaternion to which interpolation is to be performed.
     *  @param  parameter     Scalar in range [0..1] describing relative distance between quaternions for which interpolation is to be calculated.
     *  @param  shortestPath  TRUE if shortest path (if possible) is to be used for interpolation.
     *  @return Calculated quaternion.
     */
    TQuaternion slerp(const TQuaternion& to, float32 parameter, bool shortestPath = false) const;

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
TQuaternion<T>::TQuaternion() : x(0)
                              , y(0)
                              , z(0)
                              , w(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TQuaternion<T>::TQuaternion(T x, T y, T z, T w) : x(x)
                                                , y(y)
                                                , z(z)
                                                , w(w)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TQuaternion<T>::TQuaternion(const TQuaternion& other) : x(other.x)
                                                      , y(other.y)
                                                      , z(other.z)
                                                      , w(other.w)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TQuaternion<T>::TQuaternion(const TVector3<T>& axis, const Angle& angle)
{
  create(axis, angle);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TQuaternion<T>::TQuaternion(const TMatrix4<T>& matrix)
{
  // Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
  // article "Quaternion Calculus and Fast Animation".

  float32 trace = matrix[0][0] + matrix[1][1] + matrix[2][2];
  float32 root;

  if (0 < trace)
  {
    // |w| > 1/2, may as well choose w > 1/2
    root = Math::Sqrt(trace + 1.0f);  // 2w

    w = 0.5f * root;
    root = 0.5f / root;  // 1/(4w)
    x = (matrix[2][1] - matrix[1][2]) * root;
    y = (matrix[0][2] - matrix[2][0]) * root;
    z = (matrix[1][0] - matrix[0][1]) * root;
  }
  else
  {
    static s32 KIndexing[3] = { 1, 2, 0 };

    // |w| <= 1/2
    s32 i = 0;
    if (matrix[1][1] > matrix[0][0])
    {
      i = 1;
    }

    if (matrix[2][2] > matrix[i][i])
    {
      i = 2;
    }

    s32 j = KIndexing[i];
    s32 k = KIndexing[j];

    root = Math::Sqrt(matrix[i][i]- matrix[j][j] - matrix[k][k] + 1.0f);

    float32* apkQuat[3] = { &x, &y, &z };

    *apkQuat[i] = 0.5f * root;
    root = 0.5f / root;

    w = (matrix[k][j] - matrix[j][k]) * root;
    
    *apkQuat[j] = (matrix[j][i] + matrix[i][j]) * root;
    *apkQuat[k] = (matrix[k][i] + matrix[i][k]) * root;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TQuaternion<T>::operator == (const TQuaternion<T>& other) const
{
  return (x == other.x) && (y == other.y) && (z == other.z) && (w == other.w);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TQuaternion<T> TQuaternion<T>::operator - () const
{
  return TQuaternion(-x, -y, -z, -w);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
template <typename T>
void TQuaternion<T>::convertTo(TVector3<T>& axis, Angle& angle) const
{
  // calculate inverse length of imaginary axes
  T invLength = 1.0f / (x * x + y * y + z * z);

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
template <typename T>
void TQuaternion<T>::convertTo(TMatrix4<T>& matrix) const
{
  // first column
	matrix[0][0] = 1.0f - 2.0f * (y * y + z * z); 
	matrix[0][1] = 2.0f * (x * y - z * w);
	matrix[0][2] = 2.0f * (x * z + y * w);

	// second column
	matrix[1][0] = 2.0f * (x * y + z * w);  
	matrix[1][1] = 1.0f - 2.0f * (x * x + z * z); 
	matrix[1][2] = 2.0f * (z * y - x * w);  

	// third column
	matrix[2][0] = 2.0f * (x * z - y * w);
	matrix[2][1] = 2.0f * (y * z + x * w);
	matrix[2][2] = 1.0f - 2.0f * (x * x + y * y);  
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TQuaternion<T> TQuaternion<T>::multiply(const TQuaternion<T>& quat) const
{
  return TQuaternion(w * quat.x + x * quat.w + y * quat.z - z * quat.y, w * quat.y + y * quat.w + z * quat.x - x * quat.z,
                     w * quat.z + z * quat.w + x * quat.y - y * quat.x, w * quat.w - x * quat.x - y * quat.y - z * quat.z);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
Angle TQuaternion<T>::angle() const 
{ 
  return Angle::FromRadians(2.0f * Math::ACos(w)); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TQuaternion<T>::dotProduct(const TQuaternion<T>& quat) const
{
  return (x * quat.x) + (y * quat.y) + (z * quat.z) + (w * quat.w);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TQuaternion<T>::normalize()
{
  T length = this->length();

  // check if can be done
  if (Math::EPSILON <= length)
  {
    T invLength = static_cast<T>(1.0) / length;
    
    x *= invLength;
    y *= invLength;
    z *= invLength;
    w *= invLength;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TQuaternion<T>::length() const
{
  return Math::Sqrt((x * x) + (y * y) + (z * z) + (w * w));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TQuaternion<T>::lengthSquared() const
{
  return (x * x) + (y * y) + (z * z) + (w * w);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TQuaternion<T> TQuaternion<T>::conjugated() const
{
  // NOTE: must be normalized
  return TQuaternion<T>(-x, -y, -z, w);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TQuaternion<T>::conjugate()
{
  x = -x;
  y = -y;
  z = -z;

  // NOTE: w stays the same
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TQuaternion<T> TQuaternion<T>::slerp(const TQuaternion<T>& to, float32 paramter, bool shortestPath) const
{
  TQuaternion<T> out;

  //
  // p     - source quaternion
  // q     - destination quaternion
  // t     - time
  // omega - angle between p and q
  //
  //                    p*sin( ( 1-t )*omega )+q*sin( t*omega )
  // slerp( p, q, t ) = ---------------------------------------
  //                                 sin( omega )
  //

  // calculate cosine omega (dot product of 2 quaternions)
  T cosOmega = dotProduct(to);

  // adjust the signs
  TQuaternion<T> tmp;
  if ((0 > cosOmega) && shortestPath)
  {
    cosOmega = -cosOmega;

    // NOTE: q and -q rotates from the same start point and to the same end point but thru opposite direction
    tmp = -to;
  }
  else
  {
    tmp = to;
  }

  // calculate coefficients
  if ((1.0f - Math::EPSILON) > Math::Abs(cosOmega))
  {
    // standard case (slerp)
    T sinOmega    = Math::Sqrt(1 - cosOmega * cosOmega);
    EGE_ASSERT(0 != sinOmega);
    T invSinOmega = 1 / sinOmega;

    T angle = Math::ATan2(sinOmega, cosOmega);

    T coeff0 = Math::Sin((1 - paramter) * angle) * invSinOmega;
    T coeff1 = Math::Sin(paramter * angle) * invSinOmega;

    out = coeff0 * (*this) + coeff1 * tmp;
  }
  else
  {
    // There are two situations:
    // 1. "from" and "to" are very close (cos ~= +1), so we can do a linear interpolation safely.
    // 2. "from" and "to" are almost inverse of each other (cos ~= -1), there are an infinite number of possibilities interpolation. 
    //    Do linear interpolation here as well as no other way to fix it yet.
    out = (1 - paramter) * (*this) + paramter * tmp;

    // taking the complement requires renormalisation
    out.normalize();
  }

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TQuaternion<T> operator * (const TQuaternion<T>& left, const TQuaternion<T>& right)
{
  return left.multiply(right);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TQuaternion<T> operator * (T scalar, const TQuaternion<T>& right)
{
  return TQuaternion<T>(right.x * scalar, right.y * scalar, right.z * scalar, right.w * scalar);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TQuaternion<T> operator * (const TQuaternion<T>& left, T scalar)
{
  return TQuaternion<T>(left.x * scalar, left.y * scalar, left.z * scalar, left.w * scalar);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TQuaternion<T> operator + (const TQuaternion<T>& left, const TQuaternion<T>& right)
{
  return TQuaternion<T>(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_QUATERNION_H