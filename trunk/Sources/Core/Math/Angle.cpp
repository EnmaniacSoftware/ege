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
  m_radians = m_radians - Math::TWO_PI * Math::Floor((m_radians + Math::PI - center) / Math::TWO_PI);
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
