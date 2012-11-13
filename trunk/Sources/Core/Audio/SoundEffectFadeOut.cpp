#include "Core/Audio/SoundEffectFadeOut.h"
#include "Core/Audio/Sound.h"
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(SoundEffectFadeOut)
EGE_DEFINE_DELETE_OPERATORS(SoundEffectFadeOut)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundEffectFadeOut::SoundEffectFadeOut(const Time& duration) : SoundEffect(EGE_OBJECT_UID_SOUND_EFFECT_FADE_OUT),
                                                               m_duration(duration),
                                                               m_time(0.0f)
{
  egeDebug() << "Creating sound effect: EGE_OBJECT_UID_SOUND_EFFECT_FADE_OUT" << duration.seconds() << this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundEffectFadeOut::~SoundEffectFadeOut()
{
  egeDebug() << "Destroying sound effect: EGE_OBJECT_UID_SOUND_EFFECT_FADE_OUT" << this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! SoundEffect override. Returns TRUE if object is valid. */
bool SoundEffectFadeOut::isValid() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! SoundEffect override. Updates object. 
 * @param time  Time frame for which update should be done.
 * @param sound Sound object for which effect is being run.
 * @return  Returns TRUE if effect came to completion. Otherwise, returns FALSE.
 */
bool SoundEffectFadeOut::update(const Time& time, PSound sound)
{
  bool done = false;

  // update time
  m_time += time;

  // check if done
  if (m_time >= m_duration)
  {
    // we r done
    m_time = m_duration;

    done = true;
  }

  // update volume
  sound->setVolume(1.0f - (m_time.seconds() / m_duration.seconds()));

  if (done)
  {
    // notify
    emit finished(this, sound);
  }

  return done;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets fade duration. */
void SoundEffectFadeOut::setDuration(const Time& time)
{
  m_duration = time;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! SoundEffect override. Resets effect. */
void SoundEffectFadeOut::reset()
{
  m_time = 0.0f;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END