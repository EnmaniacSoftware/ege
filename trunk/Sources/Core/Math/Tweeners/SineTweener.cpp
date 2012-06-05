#include "Core/Math/Tweeners/SineTweener.h"
#include <EGEMath.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SineTweener::SineTweener()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SineTweener::~SineTweener()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Calculates eased-in value for a given normalized time. 
    @param time Normalized time for which ease value is to be computed.
    @return  Returns calculated ease value.
 */
float32 SineTweener::easeIn(float32 time)
{
  return Math::Sin(time * EGEMath::PI_HALF - EGEMath::PI_HALF) + 1.0f;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Calculates eased-out value for a given normalized time. 
    @param time Normalized time for which ease value is to be computed.
    @return  Returns calculated ease value.
 */
float32 SineTweener::easeOut(float32 time)
{
  return Math::Sin(time * EGEMath::PI_HALF);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Calculates eased-in-out value for a given normalized time. 
    @param time Normalized time for which ease value is to be computed.
    @return  Returns calculated ease value.
 */
float32 SineTweener::easeInOut(float32 time)
{
  return (Math::Sin(time * EGEMath::PI - EGEMath::PI_HALF) + 1.0f) * 0.5f;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END