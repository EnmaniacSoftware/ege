#ifndef EGE_CORE_PLANE_H
#define EGE_CORE_PLANE_H

#include <EGETypes.h>
#include "Core/Math/Vector4.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
class TPlane
{
  public:

    TPlane();
    TPlane(const TPlane& plane);
    TPlane(const TVector4<T>& normal, T d);

  public:

    /*! Available sides. */
    enum Side
    {
      SIDE_NONE = 0,
      SIDE_POSITIVE,
      SIDE_NEGATIVE,
      SIDE_BOTH
    };

  public:

    TPlane<T>&  operator=(const TPlane<T>& plane);
    bool        operator==(const TPlane<T>& plane) const;

    void create(const TVector4<T>& normal, T d);
    void create(const TPlane<T>& plane);
    
    Side side(const TVector4<T>& point) const;
    //ESide getSide ( const CAxisAlignedBox* pcBox ) const;                         // gets plane side where given AAB lies
    //ESide getSide( const CVector3* pcCentre, const CVector3* pcHalfSize ) const;  // gets plane side where given AAB lies (given by center and half size)
    
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
TPlane<T>::TPlane() : m_normal(TVector4<T>::ZERO), m_d(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TPlane<T>::TPlane(const TPlane& plane) : m_normal(plane.m_normal), m_d(plane.m_d) 
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TPlane<T>::TPlane(const TVector4<T>& normal, T d) : m_normal(normal), m_d(d)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline bool TPlane<T>::operator==(const TPlane<T>& plane) const
{
  return ! ((m_d != plane.m_d) || (m_normal != plane.m_normal));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline TPlane<T>& TPlane<T>::operator=(const TPlane<T>& plane)
{
  create(plane);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline void TPlane<T>::create(const TVector4<T>& normal, T d)
{
  m_normal = normal;
  m_d = d;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline void TPlane<T>::create(const TPlane<T>& plane)
{
  create(plane.m_normal, plane.m_d);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline void TPlane<T>::normalize()
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
inline T TPlane<T>::distance(const TVector4<T>& point) const
{
  return m_normal.dotProduct(point) + m_d;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline typename TPlane<T>::Side TPlane<T>::side(const TVector4<T>& point) const
{
  // get distance from plane
  T curDistance = distance(point);

  if (0 < curDistance)
  {
    return SIDE_POSITIVE;
  }
  else if (0 > curDistance)
  {
    return SIDE_NEGATIVE;
  }

  // on plane
  return SIDE_NONE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef TPlane<float32> Planef;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PLANE_H