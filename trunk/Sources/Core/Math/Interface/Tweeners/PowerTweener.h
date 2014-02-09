#ifndef EGE_CORE_MATH_TWEENERS_POWERTWEENER_H
#define EGE_CORE_MATH_TWEENERS_POWERTWEENER_H

/** Power tweener. 
    @note Resources used: http://theinstructionlimit.com/flash-style-tweeneasing-functions-in-c
 */

#include "Core/Math/Interface/Tweeners/ITweener.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PowerTweener : public ITweener
{
  public:

    PowerTweener();
   ~PowerTweener();

    /*! Sets power coefficient. */
    void setPowerCoefficient(float32 power);

    /*! @see ITweener::easeIn. */
    float32 easeIn(float32 time) override;
    /*! @see ITweener::easeOut. */
    float32 easeOut(float32 time) override;
    /*! @see ITweener::easeInOut. */
    float32 easeInOut(float32 time) override;

  private:

    /*! Power coefficient. */
    float32 m_power;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_TWEENERS_POWERTWEENER_H