#include "Core/Math/Tweeners/BackTweener.h"
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BackTweener::BackTweener() : m_amplitude(1.0f)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BackTweener::~BackTweener()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets amplitude. */
void BackTweener::setAmplitude(float32 amplitude)
{
  m_amplitude = amplitude;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Calculates eased-in value for a given normalized time. 
    @param time Normalized time for which ease value is to be computed.
    @return  Returns calculated ease value.
 */
float32 BackTweener::easeIn(float32 time)
{
  EGE_ASSERT(false && "Implement");
  return time;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Calculates eased-out value for a given normalized time. 
    @param time Normalized time for which ease value is to be computed.
    @return  Returns calculated ease value.
 */
float32 BackTweener::easeOut(float32 time)
{
  EGE_ASSERT(false && "Implement");
  return time;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Calculates eased-in-out value for a given normalized time. 
    @param time Normalized time for which ease value is to be computed.
    @return  Returns calculated ease value.
 */
float32 BackTweener::easeInOut(float32 time)
{
  EGE_ASSERT(false && "Implement");
  return time;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END