#include "Core/Math/Interface/Angle.h"
#include "Core/Math/Interface/Math.h"
#include "EGEDebug.h"

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
const Angle& Angle::operator += (const Angle& other)
{ 
  m_radians += other.m_radians; 
  return *this; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const Angle& Angle::operator -= (const Angle& other)
{ 
  m_radians -= other.m_radians; 
  return *this; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const Angle& Angle::operator *= (float32 scalar) 
{ 
  m_radians *= scalar; 
  return *this; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Angle::operator > (const Angle& other) const 
{ 
  return m_radians > other.m_radians; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Angle::operator < (const Angle& other) const 
{ 
  return m_radians < other.m_radians; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Angle::operator > (float32 radians) const 
{ 
  return m_radians > radians; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Angle::operator < (float32 radians) const 
{ 
  return m_radians < radians; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Angle Angle::operator - () const 
{ 
  return Angle::FromRadians(-m_radians); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Angle::operator = (float32 radians) 
{ 
  m_radians = radians; 
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
  m_radians = m_radians - Math::TWO_PI * Math::Floor((m_radians + Math::PI - center) / Math::TWO_PI);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Angle Angle::normalized() const
{
  Angle angle = *this;
  angle.normalize(Math::PI);
  return angle;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Angle Angle::distanceTo(const Angle& angle, AngleDirection direction)
{
  Angle out;

  // normalize to [0-2PI) intervals
  Angle angle1 = normalized();
  Angle angle2 = angle.normalized();

  // process according to direction
  // NOTE: for SHORTEST we just do a lucky guess and post process later
  switch (direction)
  {
    case EShortest:
    case EClockwise:
      
      if (angle1.radians() >= angle2.radians())
      {
        out.m_radians = angle1.radians() - angle2.radians();
      }
      else
      {
        out.m_radians = Math::TWO_PI - (angle2.radians() - angle1.radians());
      }
      break;

    case ECounterClockwise:
      
      if (angle1.radians() >= angle2.radians())
      {
        out.m_radians = Math::TWO_PI - (angle1.radians() - angle2.radians());
      }
      else
      {
        out.m_radians = angle2.radians() - angle1.radians();
      }
      break;
  }

  // in case of shortest angle distance...
  if (EShortest == direction)
  {
    // ...check if this is not shortest...
    if (out.m_radians > Math::PI)
    {
      // ...correct it
      out.m_radians = Math::TWO_PI - out.m_radians;
    }
  }

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Angle::fromRadians(float32 radians) 
{ 
  m_radians = radians; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 Angle::radians() const 
{ 
  return m_radians; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Debug& operator << (Debug& debug, const Angle& obj)
{
  debug.nospace() << "Angle(" << obj.degrees() << ")";
  return debug.space();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END