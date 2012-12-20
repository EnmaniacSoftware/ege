#ifndef EGE_CORE_SOUNDEFFECTFADEIN_H
#define EGE_CORE_SOUNDEFFECTFADEIN_H

/** Sound effect responsible for fading volume out.
*/

#include <EGE.h>
#include "Core/Audio/SoundEffect.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Sound;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SoundEffectFadeIn : public SoundEffect
{
  public:

    SoundEffectFadeIn(const Time& duration);
   ~SoundEffectFadeIn();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! @see SoundEffect::update. */
    bool update(const Time& time, PSound sound) override;
    /*! @see SoundEffect::reset. */
    void reset() override;
    /*! Sets fade duration. */
    void setDuration(const Time& time);

  private:

    /*! Total duration of fade effect. */
    Time m_duration;
    /*! Currently passed time. */
    Time m_time;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SOUNDEFFECTFADEIN_H