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

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
class TQuaternion
{
  public:

    TQuaternion();
    TQuaternion(const TQuaternion& other);
    TQuaternion(T x, T y, T z, T w);

  operators:

    bool        operator == (const TQuaternion& other) const;
    TQuaternion operator - () const;

  public:

    /*! Returns length. */
    T length() const;
    /*! Returns sequared length. */
    T lengthSquared() const;

    /*! Normalizes quaternion. */
    void normalize();

    /*! Returns quaternion angle in radians. */
    float32 radians() const;

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
TQuaternion<T> TQuaternion<T>::multiply(const TQuaternion<T>& quat) const
{
  return TQuaternion(w * quat.x + x * quat.w + y * quat.z - z * quat.y, w * quat.y + y * quat.w + z * quat.x - x * quat.z,
                     w * quat.z + z * quat.w + x * quat.y - y * quat.x, w * quat.w - x * quat.x - y * quat.y - z * quat.z);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
float32 TQuaternion<T>::radians() const 
{ 
  return 2.0f * static_cast<float32>(Math::ACos(w)); 
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