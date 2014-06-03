#include "Core/Audio/Interface/Effects/SoundEffectFadeOut.h"
#include "Core/Audio/Interface/Sound.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(SoundEffectFadeOut)
EGE_DEFINE_DELETE_OPERATORS(SoundEffectFadeOut)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundEffectFadeOut::SoundEffectFadeOut(const Time& duration) : SoundEffect(EGE_OBJECT_UID_SOUND_EFFECT_FADE_OUT)
                                                             , m_duration(duration)
                                                             , m_time(0.0f)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundEffectFadeOut::~SoundEffectFadeOut()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
    emit finished();
    emit finishedWithSelfAndSound(this, sound);
  }

  return done;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SoundEffectFadeOut::setDuration(const Time& time)
{
  m_duration = time;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SoundEffectFadeOut::reset()
{
  m_time = 0.0f;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END