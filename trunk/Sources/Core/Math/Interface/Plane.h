#ifndef EGE_CORE_MATH_PLANE_H
#define EGE_CORE_MATH_PLANE_H

/*! Class representing plane in 3D.
 */

#include "EGETypes.h"
#include "Core/Math/Interface/Vector3.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Available sides. */
enum PlaneSide
{
  EInvalid = 0,       /*!< Invalid. */
  EPositive,          /*!< Positive side. Side toward which plane normal faces. */
  ENegative,          /*!< Negative side. Side opposite to plane normal direction. */
  EBoth               /*!< Plane itself. */
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
class TPlane
{
  public:

    TPlane();
    TPlane(const TPlane& other);
    TPlane(const TVector3<T>& normal, T displacement);

  operators:

    TPlane<T>&  operator  = (const TPlane<T>& other);
    bool        operator == (const TPlane<T>& other) const;
    bool        operator != (const TPlane<T>& other) const;

  public:
    
    /*! Returns side of plane on which given point lies. 
     *  @param  point Point under test.
     *  @return Side of the place where point lies.
     */
    PlaneSide side(const TVector3<T>& point) const;
    
    /*! Returns distance to given point.
     *  @param  point Point to which distance is being calculated.
     *  @return Calculated distance to given point.
     */
    T distance(const TVector3<T>& point) const;

    /*! Normalizes the plane. */
    void normalize();

  public:

    /*! Normal vector. */
    TVector3<T> normal;
    /*! Displacement. */
    T displacement;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TPlane<T>::TPlane() : normal(TVector3<T>::ZERO)
                    , displacement(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TPlane<T>::TPlane(const TPlane& other) : normal(other.normal)
                                       , displacement(other.displacement) 
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TPlane<T>::TPlane(const TVector3<T>& normal, T displacement) : normal(normal)
                                                             , displacement(displacement)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TPlane<T>::operator == (const TPlane<T>& other) const
{
  return ! ((displacement != other.displacement) || (normal != other.normal));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TPlane<T>::operator != (const TPlane<T>& other) const
{
  return ! operator == (other);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TPlane<T>& TPlane<T>::operator = (const TPlane<T>& other)
{
  normal        = other.normal;
  displacement  = other.displacement;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TPlane<T>::normalize()
{
  // get normal magnitude
  T length = normal.length();

  // check if can be done
  if (Math::EPSILON <= length)
  {
    // normalize normal
    normal.normalize();

    // normalize displacement
    displacement /= length;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TPlane<T>::distance(const TVector3<T>& point) const
{
  return normal.dotProduct(point) + displacement;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
PlaneSide TPlane<T>::side(const TVector3<T>& point) const
{
  // get distance from plane
  T curDistance = distance(point);

  if (0 < curDistance)
  {
    return EPositive;
  }
  else if (0 > curDistance)
  {
    return ENegative;
  }

  // on plane
  return EBoth;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_PLANE_H