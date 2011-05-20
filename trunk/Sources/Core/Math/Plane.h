#ifndef EGE_CORE_PLANE_H
#define EGE_CORE_PLANE_H

#include "EGE.h"
#include "Core/Math/Vector4.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class TPlane
{
  public:

    TPlane() : m_normal(TVector4<T>::ZERO), m_d(0) {}
    TPlane(const TPlane& plane) : m_normal(plane.m_normal), m_d(plane.m_d) {}
    TPlane(const TVector4<T>& normal, T d) : m_normal(normal), m_d(d) {}

    enum ESide
    {
      NO_SIDE,
      POSITIVE_SIDE,
      NEGATIVE_SIDE,
      BOTH_SIDE
    };

    inline bool       operator==(const TPlane<T>& plane) const;
    inline TPlane<T>& operator=(const TPlane<T>& plane);

    inline void create(const TVector4<T>& normal, T d);
    inline void create(const TPlane<T>& plane);
    
    inline ESide side(const TVector4<T>& point) const;
    //ESide getSide ( const CAxisAlignedBox* pcBox ) const;                         // gets plane side where given AAB lies
    //ESide getSide( const CVector3* pcCentre, const CVector3* pcHalfSize ) const;  // gets plane side where given AAB lies (given by center and half size)
    
    inline T distance(const TVector4<T>& point) const;

    inline void normalize();


    TVector4<T> m_normal;   // normal vector

    T m_d;                  // displacement
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
bool TPlane<T>::operator==(const TPlane<T>& plane) const
{
  return !(m_d != plane.m_d || m_normal != plane.m_normal);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
TPlane<T>& TPlane<T>::operator=(const TPlane<T>& plane)
{
  create(plane);
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
  T mag = m_normal.length();

  // normalize normal
  m_normal.normalize();

  // normalize displacement
  m_d /= mag;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
T TPlane<T>::distance(const TVector4<T>& point) const
{
  return Math::DotProduct(m_normal, point) + m_d;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
typename TPlane<T>::ESide TPlane<T>::side(const TVector4<T>& point) const
{
  // get distance from plane
  T curDistance = distance(point);

  if (curDistance > 0)
  {
    return POSITIVE_SIDE;
  }
  else if (curDistance < 0)
  {
    return NEGATIVE_SIDE;
  }

  // on plane
  return NO_SIDE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PLANE_H