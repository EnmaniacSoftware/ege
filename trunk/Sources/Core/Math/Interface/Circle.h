#ifndef EGE_CORE_MATH_CIRCLE_H
#define EGE_CORE_MATH_CIRCLE_H

/** Class representing circle.
 */

#include "EGETypes.h"
#include "EGEVector.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
class TCircle
{
  public:

    TCircle();
    TCircle(T centerX, T centerY, T radius);
    TCircle(const TVector2<T>& center, T radius);
    TCircle(const TCircle& other);

    /*! Returns TRUE if given point is contained within circle. */
    bool contains(T pointX, T pointY) const;
    /*! Returns TRUE if given point is contained within circle. */
    bool contains(const TVector2<T>& point) const;

  public:

    T x;        /*!< Center X position. */
    T y;        /*!< Center Y position. */
    T r;        /*!< Radius. */
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TCircle<T>::TCircle()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TCircle<T>::TCircle(T centerX, T centerY, T radius) : x(centerX), y(centerY), r(radius)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TCircle<T>::TCircle(const TCircle& other) : x(other.x), y(other.y), r(other.t)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TCircle<T>::TCircle(const TVector2<T>& center, T radius) : x(center.x), y(center.y), r(radius)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TCircle<T>::contains(T pointX, T pointY) const
{
  return (pointX >= (x - r)) && (pointX <= (x + r)) && (pointY >= (y - r)) && (pointY <= (y + r));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TCircle<T>::contains(const TVector2<T>& point) const
{
  return (point.x >= (x - r)) && (point.x <= (x + r)) && (point.y >= (y - r)) && (point.y <= (y + r));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_CIRCLE_H