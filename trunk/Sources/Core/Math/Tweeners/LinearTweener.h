#ifndef EGE_CORE_MATH_TWEENER_LINEAR_H
#define EGE_CORE_MATH_TWEENER_LINEAR_H

/** Linear tweener. The formula used for this function is the following:
    f(t) = t
 */

#include "Core/Math/Tweeners/ITweener.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class LinearTweener : public ITweener
{
  public:

    LinearTweener();
   ~LinearTweener();

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

#endif // EGE_CORE_MATH_TWEENER_LINEAR_H