#ifndef EGE_CORE_ANGLE_H
#define EGE_CORE_ANGLE_H

//----------------------------------------------------------------------------------------------------------------------

#include "EGE.h"
#include "Core/Math/Math.h"

EGE_NAMESPACE_BEGIN

//----------------------------------------------------------------------------------------------------------------------

class Angle
{
  public:

    Angle();
    Angle(const Angle& angle);

    static Angle FromDegrees(float32 degrees);
    static Angle FromRadians(float32 radians);

    inline void fromDegrees(float32 degrees) { m_radians = Math::DegreesToRadians(degrees); }
    inline void fromRadians(float32 radians) { m_radians = radians; }

    inline float32 radians() const { return m_radians; }
    inline float32 degrees() const { return Math::RadiansToDegrees(m_radians); }

    const Angle& operator += (const Angle& other) { m_radians += other.m_radians; return *this; }

  private:

    /*! Angle value in radians. */
    float32 m_radians;
};

//----------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_ANGLE_H
