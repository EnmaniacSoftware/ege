#ifndef EGE_CORE_MATH_TWEENERS_LINEARTWEENER_H
#define EGE_CORE_MATH_TWEENERS_LINEARTWEENER_H

/** Linear tweener. The formula used for this function is the following:
    f(t) = t
 */

#include "Core/Math/Interface/Tweeners/ITweener.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class LinearTweener : public ITweener
{
  public:

    LinearTweener();
   ~LinearTweener();

    /*! @see ITweener::easeIn. */
    float32 easeIn(float32 time) override;
    /*! @see ITweener::easeOut. */
    float32 easeOut(float32 time) override;
    /*! @see ITweener::easeInOut. */
    float32 easeInOut(float32 time) override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_TWEENERS_LINEARTWEENER_H