#ifndef EGE_CORE_MATH_TWEENER_SINE_H
#define EGE_CORE_MATH_TWEENER_SINE_H

/** Sine tweener.
    @note Resources used: http://theinstructionlimit.com/flash-style-tweeneasing-functions-in-c
 */

#include "Core/Math/Tweeners/ITweener.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SineTweener : public ITweener
{
  public:

    SineTweener();
   ~SineTweener();

    /* Calculates eased-in value for a given normalized time. 
       @param time  Normalized time for which ease value is to be computed.
       @return  Returns calculated ease value.
     */
    float32 easeIn(float32 time) override;
    /* Calculates eased-out value for a given normalized time. 
       @param time  Normalized time for which ease value is to be computed.
       @return  Returns calculated ease value.
     */
    float32 easeOut(float32 time) override;
    /* Calculates eased-in-out value for a given normalized time. 
       @param time  Normalized time for which ease value is to be computed.
       @return  Returns calculated ease value.
     */
    float32 easeInOut(float32 time) override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_TWEENER_SINE_H