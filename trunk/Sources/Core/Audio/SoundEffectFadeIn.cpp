#include "Core/Audio/SoundEffectFadeIn.h"
#include "Core/Audio/Sound.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(SoundEffectFadeIn)
EGE_DEFINE_DELETE_OPERATORS(SoundEffectFadeIn)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundEffectFadeIn::SoundEffectFadeIn(const Time& duration) : SoundEffect(),
                                                             m_duration(duration),
                                                             m_time(0.0f)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundEffectFadeIn::~SoundEffectFadeIn()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! SoundEffect override. Returns TRUE if object is valid. */
bool SoundEffectFadeIn::isValid() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! SoundEffect override. Updates object. 
 * @param time  Time frame for which update should be done.
 * @param sound Sound object for which effect is being run.
 * @return  Returns TRUE if effect came to completion. Otherwise, returns FALSE.
 */
bool SoundEffectFadeIn::update(const Time& time, PSound sound)
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
  sound->setVolume(m_time.seconds() / m_duration.seconds());

  if (done)
  {
    // notify
    emit finished(this, sound);
  }

  return done;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets fade duration. */
void SoundEffectFadeIn::setDuration(const Time& time)
{
  m_duration = time;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! SoundEffect override. Resets effect. */
void SoundEffectFadeIn::reset()
{
  m_time = 0.0f;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
