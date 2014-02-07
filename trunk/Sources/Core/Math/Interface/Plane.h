#ifndef EGE_CORE_MATH_PLANE_H
#define EGE_CORE_MATH_PLANE_H

#include "EGETypes.h"
#include "Core/Math/Interface/Vector4.h"

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
    TPlane(const TVector4<T>& normal, T d);

  operators:

    TPlane<T>&  operator  = (const TPlane<T>& other);
    bool        operator == (const TPlane<T>& other) const;

  public:

    void create(const TVector4<T>& normal, T d);
    void create(const TPlane<T>& plane);
    
    PlaneSide side(const TVector4<T>& point) const;
    
    T distance(const TVector4<T>& point) const;

    void normalize();

  public:

    /*! Normal vector. */
    TVector4<T> m_normal;
    /*! Displacement. */
    T m_d;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TPlane<T>::TPlane() : m_normal(TVector4<T>::ZERO)
                    , m_d(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TPlane<T>::TPlane(const TPlane& other) : m_normal(other.m_normal)
                                       , m_d(other.m_d) 
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TPlane<T>::TPlane(const TVector4<T>& normal, T d) : m_normal(normal)
                                                  , m_d(d)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TPlane<T>::operator==(const TPlane<T>& other) const
{
  return ! ((m_d != other.m_d) || (m_normal != other.m_normal));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TPlane<T>& TPlane<T>::operator=(const TPlane<T>& other)
{
  create(other);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TPlane<T>::create(const TVector4<T>& normal, T d)
{
  m_normal = normal;
  m_d = d;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TPlane<T>::create(const TPlane<T>& plane)
{
  create(plane.m_normal, plane.m_d);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TPlane<T>::normalize()
{
  // get normal magnitude
  T length = m_normal.length();

  // check if can be done
  if (Math::EPSILON <= length)
  {
    // normalize normal
    m_normal.normalize();

    // normalize displacement
    m_d /= length;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TPlane<T>::distance(const TVector4<T>& point) const
{
  return m_normal.dotProduct(point) + m_d;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
PlaneSide TPlane<T>::side(const TVector4<T>& point) const
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
typedef TPlane<float32> Planef;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_PLANE_H