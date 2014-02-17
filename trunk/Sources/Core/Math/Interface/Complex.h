#ifndef EGE_CORE_MATH_COMPLEX_H
#define EGE_CORE_MATH_COMPLEX_H

/** Class representing complex number in the following form: x + i*y 
 *  where:
 *  x - represents real part value
 *  y - represents imaginary part value
 */

#include "EGETypes.h"
#include "Core/Math/Interface/Math.h"
#include "Core/Math/Interface/Angle.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
class TComplex
{
  public:

    TComplex();
    TComplex(T x, T y);
    TComplex(const Angle& angle);
    TComplex(const TComplex& other);

  operators:

    TComplex& operator *= (const TComplex& other);
    TComplex& operator  = (const TComplex& other);

  public:

    /*! Creates number from given angle. */
    void create(const Angle& angle);

    /*! Returns length. */
    T length() const;
    /*! Returns sequared length. */
    T lengthSquared() const;

    /*! Normalizes number. */
    void normalize();

    /*! Returns dot product between current and given number. */
    T dotProduct(const TComplex& other) const;

    /*! Returns angle representation. */
    Angle angle() const;

    /*! Performs spherical linear interpolation between this and given complex numbers. 
     *  @param  to        Complex number to which interpolation is to be performed.
     *  @param  parameter Scalar in range [0..1] describing relative distance between numbers for which interpolation is to be calculated.
     */
    TComplex<T> slerp(const TComplex<T>& to, float32 parameter) const;

  public:

    T x;
    T y;

  public:

    static const TComplex<T> ZERO;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
const TComplex<T> TComplex<T>::ZERO = TComplex<T>(0, 0);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TComplex<T>::TComplex()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TComplex<T>::TComplex(T x, T y) : x(x)
                                , y(y)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TComplex<T>::TComplex(const Angle& angle)
{
  create(angle);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TComplex<T>::TComplex(const TComplex& other) : x(other.x)
                                             , y(other.y)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TComplex<T>& TComplex<T>::operator *= (const TComplex& other)
{
  T newX = x * other.x + y * other.y;
  T newY = y * other.x + x * other.y;

  x = newX;
  y = newY;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TComplex<T>& TComplex<T>::operator = (const TComplex& other)
{
  x = other.x;
  y = other.y;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TComplex<T>::length() const
{
  return Math::Sqrt((x * x) + (y * y)); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TComplex<T>::lengthSquared() const
{
  return (x * x) + (y * y);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TComplex<T>::normalize()
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
T TComplex<T>::dotProduct(const TComplex<T>& other) const
{
  return (x * other.x) + (y * other.y);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TComplex<T>::create(const Angle& angle)
{
  x = Math::Cos(angle.radians());
  y = Math::Sin(angle.radians());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
Angle TComplex<T>::angle() const
{
  return Angle(Math::ATan2(y, x));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TComplex<T> TComplex<T>::slerp(const TComplex<T>& to, float32 parameter) const
{
  TComplex<T> out;

  // calculate cosine omega
  T cosOmega = dotProduct(to);

  // calculate coefficients
  T scale0;
  T scale1;

  if (Math::EPSILON < (1 - cosOmega))
  {
    // standard case
    T omega    = Math::ACos(cosOmega);
    T sinOmega = Math::Sin(omega);

    scale0 = Math::Sin(( 1.0f - parameter) * omega) / sinOmega;
    scale1 = Math::Sin(parameter * omega) / sinOmega;
  }
  else
  {
    // source and destination quaternions are very close so we perform linear interpolation
    scale0 = 1.0f - parameter;
    scale1 = parameter;
  }

	// calculate final values
	out.x = scale0 * x + scale1 * to.x;
	out.y = scale0 * y + scale1 * to.y;

  // normalize
  out.normalize();

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_COMPLEX_H