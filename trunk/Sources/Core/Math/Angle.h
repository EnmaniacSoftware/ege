#ifndef EGE_CORE_ANGLE_H
#define EGE_CORE_ANGLE_H

#include <EGETypes.h>
#include <EGEAngle.h>
#include <EGEMath.h>

EGE_NAMESPACE_BEGIN
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Debug;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Angle
{
  public:

    Angle();
    Angle(const Angle& angle);
    Angle(float32 radians);

    inline const Angle& operator += (const Angle& other) { m_radians += other.m_radians; return *this; }
    inline const Angle& operator -= (const Angle& other) { m_radians -= other.m_radians; return *this; }
    inline const Angle& operator *= (float32 scalar) { m_radians *= scalar; return *this; }
    inline bool         operator > (const Angle& other) const { return m_radians > other.m_radians; }
    inline bool         operator < (const Angle& other) const { return m_radians < other.m_radians; }
    inline bool         operator > (float32 radians) const { return m_radians > radians; }
    inline bool         operator < (float32 radians) const { return m_radians < radians; }
    inline Angle        operator - () const { return Angle::FromRadians(-m_radians); }
    inline void         operator = (float32 radians) { m_radians = radians; }

  public:

    /* Returns angle object from degrees. */
    static Angle FromDegrees(float32 degrees);
    /* Returns angle object from radians. */
    static Angle FromRadians(float32 radians);

  public:

    /* Sets angle from degrees. */
    void fromDegrees(float32 degrees);
    /* Returns angle in degrees. */
    float32 degrees() const;
    /* Sets angle from radians. */
    inline void fromRadians(float32 radians) { m_radians = radians; }
    /*! Returns angle in radians. */
    inline float32 radians() const { return m_radians; }
    /* Normalizes angle to 2PI interval around center. 
     * @param center  Center point around around which angle is spread. If 0 result interval after normalization will be [-PI..+PI].
     * @note  For normalization resulting in [0..2PI] interval use PI as center point.
     * @note  Due to numerical inaccuracies intervals are closed on both sides rather than only at one.
     */
    void normalize(float32 center = EGEMath::PI);
    /* Returns normalized angle in [0-2PI) degrees interval. */
    Angle normalized() const;
    /* Returns angle distance from current angle to given one. */
    Angle distanceTo(const Angle& angle, EGEAngle::Direction direction);

  private:

    /*! Angle value in radians. */
    float32 m_radians;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline Angle operator + (const Angle& left, const Angle& right)
{
  return Angle::FromRadians(left.radians() + right.radians());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline Angle operator - (const Angle& left, const Angle& right)
{
  return Angle::FromRadians(left.radians() - right.radians());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline bool operator > (float32 radians, const Angle& angle)
{
  return radians > angle.radians();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline bool operator < (float32 radians, const Angle& angle)
{
  return radians < angle.radians();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline Angle operator * (const Angle& left, float32 scalar)
{
  return Angle(left.radians() * scalar);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline Angle operator * (float32 scalar, const Angle& right)
{
  return Angle(right.radians() * scalar);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Debug& operator << (Debug& debug, const Angle& obj);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_ANGLE_H
