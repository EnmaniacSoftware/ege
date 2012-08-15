#ifndef EGE_CORE_SOUNDEFFECTFADEOUT_H
#define EGE_CORE_SOUNDEFFECTFADEOUT_H

/** Sound effect responsible for fading volume out.
*/

#include <EGE.h>
#include "Core/Audio/SoundEffect.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Sound;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SoundEffectFadeOut : public SoundEffect
{
  public:

    SoundEffectFadeOut(const Time& duration);
   ~SoundEffectFadeOut();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /* SoundEffect override. Returns TRUE if object is valid. */
    bool isValid() const override;
    /* SoundEffect override. Updates object. 
     * @param time  Time frame for which update should be done.
     * @param sound Sound object for which effect is being run.
     * @return  Returns TRUE if effect came to completion. Otherwise, returns FALSE.
     */
    bool update(const Time& time, PSound sound) override;
    /* SoundEffect override. Resets effect. */
    void reset() override;
    /* Sets fade duration. */
    void setDuration(const Time& time);

  private:

    /*! Total duration of fade effect. */
    Time m_duration;
    /*! Currently passed time. */
    Time m_time;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SOUNDEFFECTFADEOUT_H