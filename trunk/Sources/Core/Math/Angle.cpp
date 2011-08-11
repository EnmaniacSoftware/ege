#include "Core/Math/Angle.h"
#include "Core/Math/Math.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Angle::Angle() : m_radians(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Angle::Angle(const Angle& angle) : m_radians(angle.radians())
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns angle object from degrees. */
Angle Angle::FromDegrees(float32 degrees)
{
  Angle angle;
  angle.fromDegrees(degrees);
  return angle;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns angle object from radians. */
Angle Angle::FromRadians(float32 radians)
{
  Angle angle;
  angle.fromRadians(radians);
  return angle;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets angle from degrees. */
void Angle::fromDegrees(float32 degrees) 
{ 
  m_radians = Math::DegreesToRadians(degrees); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns angle in degrees. */
float32 Angle::degrees() const 
{ 
  return Math::RadiansToDegrees(m_radians); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Normalizes angle to 2PI interval around center. 
 * @param center  Center point around around which angle is spread. If 0 result interval after normalization will be [-PI..+PI].
 * @note  For normalization resulting in [0..2PI] interval use PI as center point.
 * @note  Due to numerical inaccuracies intervals are closed on both sides rather than only at one.
 */
void Angle::normalize(float32 center)
{
  m_radians = m_radians - EGEMath::TWO_PI * Math::Floor((m_radians + EGEMath::PI - center) / EGEMath::TWO_PI);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns normalized angle in [0-2PI) degrees interval. */
Angle Angle::normalized() const
{
  Angle angle = *this;
  angle.normalize();
  return angle;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns angle distance from current angle to given one. */
Angle Angle::distanceTo(const Angle& angle, EGEAngle::Direction direction)
{
  Angle out;

  // normalize to [0-2PI) intervals
  Angle angle1 = normalized();
  Angle angle2 = angle.normalized();

  // process according to direction
  // NOTE: for SHORTEST we just do a lucky guess and post process later
  switch (direction)
  {
    case EGEAngle::DIRECTION_SHORTEST:
    case EGEAngle::DIRECTION_CLOCKWISE:
      
      if (angle1.radians() >= angle2.radians())
      {
        out.m_radians = angle1.radians() - angle2.radians();
      }
      else
      {
        out.m_radians = EGEMath::TWO_PI - (angle2.radians() - angle1.radians());
      }
      break;

    case EGEAngle::DIRECTION_COUNTERCLOCKWISE:
      
      if (angle1.radians() >= angle2.radians())
      {
        out.m_radians = EGEMath::TWO_PI - (angle1.radians() - angle2.radians());
      }
      else
      {
        out.m_radians = angle2.radians() - angle1.radians();
      }
      break;
  }

  // in case of shortest angle distance...
  if (EGEAngle::DIRECTION_SHORTEST == direction)
  {
    // ...check if this is not shortest...
    if (out.m_radians > EGEMath::PI)
    {
      // ...correct it
      out.m_radians = EGEMath::TWO_PI - out.m_radians;
    }
  }

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
