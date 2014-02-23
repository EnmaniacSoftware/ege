#ifndef EGE_CORE_MATH_ANGLE_H
#define EGE_CORE_MATH_ANGLE_H

#include "EGETypes.h"

/** Class represents arbitrary angle. The following picture represents angle directions:
  *
  *                              90 degrees
  *                             -----------
  *                          ---     |     ---
  *                       ---        |        ---
  *                     --           |           --
  *                    -             |             - 
  *                    -             |             - 
  *        180 degrees --------------*--------------  0/360 degrees
  *                    -             |             - 
  *                    -             |             - 
  *                     --           |           --
  *                       ---        |        ---
  *                          ---     |     ---
  *                             -----------
  *                             270 degrees
  */

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Available direction values. */
enum AngleDirection
{
  EClockwise,              /*!< Clockwise direction (decreasing angle). */
  ECounterClockwise,       /*!< Counterclockwise direction (increasing angle). */ 
  EShortest                /*!< Shortest. Can be clockwise or counterclockwise. */
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Debug;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Angle
{
  public:

    Angle();
    Angle(const Angle& angle);
    Angle(float32 radians);

  operators:

    const Angle& operator += (const Angle& other);
    const Angle& operator -= (const Angle& other);
    const Angle& operator *= (float32 scalar);
    bool         operator > (const Angle& other) const;
    bool         operator < (const Angle& other) const;
    bool         operator > (float32 radians) const;
    bool         operator < (float32 radians) const;
    bool         operator == (const Angle& other) const;
    bool         operator != (const Angle& other) const;
    Angle        operator - () const;
    void         operator = (float32 radians);

  public:

    /*! Returns angle object from degrees. */
    static Angle FromDegrees(float32 degrees);
    /*! Returns angle object from radians. */
    static Angle FromRadians(float32 radians);

  public:

    /*! Sets angle from degrees. */
    void fromDegrees(float32 degrees);
    /*! Returns angle in degrees. */
    float32 degrees() const;
    /*! Sets angle from radians. */
    void fromRadians(float32 radians);
    /*! Returns angle in radians. */
    float32 radians() const;
    /*! Normalizes angle to [-PI..+PI] interval. 
     *  @param center  Center point around which angle is spread. If set to 0, result interval after normalization will be [-PI..+PI].
     *  @note  For normalization resulting in [0..2PI] interval use PI as center point.
     *  @note  Due to numerical inaccuracies intervals are closed on both sides rather than only at one.
     */
    void normalize(float32 center = 0.0f);
    /*! Returns normalized angle in [0-2PI) degrees interval. */
    Angle normalized() const;
    /*! Returns angle distance from current angle to given one. */
    Angle distanceTo(const Angle& angle, AngleDirection direction);

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

#endif // EGE_CORE_MATH_ANGLE_H
