#include "Core/Math/Tweeners/LinearTweener.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
LinearTweener::LinearTweener()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
LinearTweener::~LinearTweener()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Calculates eased-in value for a given normalized time. 
    @param time Normalized time for which ease value is to be computed.
    @return  Returns calculated ease value.
 */
float32 LinearTweener::easeIn(float32 time)
{
  return time;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Calculates eased-out value for a given normalized time. 
    @param time Normalized time for which ease value is to be computed.
    @return  Returns calculated ease value.
 */
float32 LinearTweener::easeOut(float32 time)
{
  return time;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Calculates eased-in-out value for a given normalized time. 
    @param time Normalized time for which ease value is to be computed.
    @return  Returns calculated ease value.
 */
float32 LinearTweener::easeInOut(float32 time)
{
  return time;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END