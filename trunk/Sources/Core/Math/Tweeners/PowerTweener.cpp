#include "Core/Math/Tweeners/PowerTweener.h"
#include <EGEMath.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PowerTweener::PowerTweener() : m_power(2.0f)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PowerTweener::~PowerTweener()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets power coefficient. */
void PowerTweener::setPowerCoefficient(float32 power)
{
  m_power = power;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Calculates eased-in value for a given normalized time. 
    @param time Normalized time for which ease value is to be computed.
    @return  Returns calculated ease value.
 */
float32 PowerTweener::easeIn(float32 time)
{
  return Math::Pow(time, m_power);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Calculates eased-out value for a given normalized time. 
    @param time Normalized time for which ease value is to be computed.
    @return  Returns calculated ease value.
 */
float32 PowerTweener::easeOut(float32 time)
{
  s32 sign = (static_cast<s32>(m_power) & 0x1) ? 1 : -1;

  return sign * (Math::Pow(time - 1.0f, m_power) + sign);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Calculates eased-in-out value for a given normalized time. 
    @param time Normalized time for which ease value is to be computed.
    @return  Returns calculated ease value.
 */
float32 PowerTweener::easeInOut(float32 time)
{
  time *= 2;

  if (1.0f > time)
  {
    return easeIn(time) * 0.5f;
  }

  s32 sign = (static_cast<s32>(m_power) & 0x1) ? 1 : -1;

  return sign * 0.5f * (Math::Pow(time - 2.0f, m_power) + sign * 2.0f);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END