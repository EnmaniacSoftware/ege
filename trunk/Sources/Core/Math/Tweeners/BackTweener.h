#ifndef EGE_CORE_MATH_TWEENER_BACK_H
#define EGE_CORE_MATH_TWEENER_BACK_H

/** Back tweener. The formula used for this function is the following:
    f(t) = t ^ 3 - t * amplitude * Sin(t * PI)

    @note Resources used: http://msdn.microsoft.com/en-us/library/system.windows.media.animation.backease
 */

#include "Core/Math/Tweeners/ITweener.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class BackTweener : public ITweener
{
  public:

    BackTweener();
   ~BackTweener();
   
    /*! Sets amplitude. */
    void setAmplitude(float32 amplitude);

    /*! @see ITweener::easeIn. */
    float32 easeIn(float32 time) override;
    /*! @see ITweener::easeOut. */
    float32 easeOut(float32 time) override;
    /*! @see ITweener::easeInOut. */
    float32 easeInOut(float32 time) override;

  private:

    /*! Amplitude. */
    float32 m_amplitude;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_TWEENER_BACK_H