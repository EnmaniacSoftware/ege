#ifndef EGE_CORE_MATH_TWEENERS_SINETWEENER_H
#define EGE_CORE_MATH_TWEENERS_SINETWEENER_H

/** Sine tweener.
    @note Resources used: http://theinstructionlimit.com/flash-style-tweeneasing-functions-in-c
 */

#include "Core/Math/Interface/Tweeners/ITweener.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SineTweener : public ITweener
{
  public:

    SineTweener();
   ~SineTweener();

    /*! @see ITweener::easeIn. */
    float32 easeIn(float32 time) override;
    /*! @see ITweener::easeOut. */
    float32 easeOut(float32 time) override;
    /*! @see ITweener::easeInOut. */
    float32 easeInOut(float32 time) override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_TWEENERS_SINETWEENER_H