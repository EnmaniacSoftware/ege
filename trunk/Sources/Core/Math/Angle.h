#ifndef EGE_CORE_ANGLE_H
#define EGE_CORE_ANGLE_H

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Angle
{
  public:

    Angle();
    Angle(const Angle& angle);

    const Angle& operator += (const Angle& other) { m_radians += other.m_radians; return *this; }

    /* Returns angle object from degrees. */
    static Angle FromDegrees(float32 degrees);
    /* Returns angle object from radians. */
    static Angle FromRadians(float32 radians);

    /* Sets angle from degrees. */
    void fromDegrees(float32 degrees);
    /* Returns angle in degrees. */
    float32 degrees() const;
    /* Sets angle from radians. */
    inline void fromRadians(float32 radians) { m_radians = radians; }
    /*! Returns angle in radians. */
    inline float32 radians() const { return m_radians; }
    /* Normalizes angle to [0-2PI) degrees interval. */
    void normalize();

  private:

    /*! Angle value in radians. */
    float32 m_radians;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline Angle operator+(const Angle& left, const Angle& right)
{
  return Angle::FromRadians(left.radians() + right.radians());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_ANGLE_H
