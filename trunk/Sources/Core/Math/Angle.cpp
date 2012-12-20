#include "Core/Math/Angle.h"
#include "Core/Math/Math.h"
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Angle::Angle() : m_radians(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Angle::Angle(const Angle& angle) : m_radians(angle.radians())
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Angle::Angle(float32 radians) : m_radians(radians)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Angle Angle::FromDegrees(float32 degrees)
{
  Angle angle;
  angle.fromDegrees(degrees);
  return angle;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Angle Angle::FromRadians(float32 radians)
{
  Angle angle;
  angle.fromRadians(radians);
  return angle;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Angle::fromDegrees(float32 degrees) 
{ 
  m_radians = Math::DegreesToRadians(degrees); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 Angle::degrees() const 
{ 
  return Math::RadiansToDegrees(m_radians); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Angle::normalize(float32 center)
{
  m_radians = m_radians - EGEMath::TWO_PI * Math::Floor((m_radians + EGEMath::PI - center) / EGEMath::TWO_PI);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Angle Angle::normalized() const
{
  Angle angle = *this;
  angle.normalize();
  return angle;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
Debug& operator << (Debug& debug, const Angle& obj)
{
  debug.nospace() << "Angle(" << obj.degrees() << ")";
  return debug.space();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END