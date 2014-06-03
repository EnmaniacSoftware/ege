#include "Core/Audio/Interface/Effects/SoundEffectFadeIn.h"
#include "Core/Audio/Interface/Sound.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(SoundEffectFadeIn)
EGE_DEFINE_DELETE_OPERATORS(SoundEffectFadeIn)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundEffectFadeIn::SoundEffectFadeIn(const Time& duration) : SoundEffect(EGE_OBJECT_UID_SOUND_EFFECT_FADE_IN)
                                                           , m_duration(duration)
                                                           , m_time(0.0f)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundEffectFadeIn::~SoundEffectFadeIn()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
    emit finished();
    emit finishedWithSelfAndSound(this, sound);
  }

  return done;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SoundEffectFadeIn::setDuration(const Time& time)
{
  m_duration = time;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SoundEffectFadeIn::reset()
{
  m_time = 0.0f;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END