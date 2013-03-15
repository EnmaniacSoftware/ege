#ifdef EGE_AUDIO_NULL

#include "Core/Application/Application.h"
#include "Core/Audio/Null/AudioManagerNull_p.h"
#include "Core/Audio/AudioManager.h"
#include "EGEDebug.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(AudioManagerPrivate)
EGE_DEFINE_DELETE_OPERATORS(AudioManagerPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerPrivate::AudioManagerPrivate(AudioManager* base) : m_d(base)
{
  EGE_UNUSED(base)
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerPrivate::~AudioManagerPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult AudioManagerPrivate::construct()
{
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerPrivate::update(const Time& time)
{
  EGE_UNUSED(time)
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult AudioManagerPrivate::play(const PSound& sound)
{
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult AudioManagerPrivate::stop(const PSound& sound)
{
  EGE_UNUSED(sound)

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AudioManagerPrivate::isPlaying(const PSound& sound) const
{
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AudioManagerPrivate::isPlaying(const String& soundName) const
{
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult AudioManagerPrivate::pause(const PSound& sound)
{
  EGE_UNUSED(sound)

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AudioManagerPrivate::isPaused(const PSound& sound) const
{
  EGE_UNUSED(sound)

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AudioManagerPrivate::isStopped(const PSound& sound) const
{
  EGE_UNUSED(sound)

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerPrivate::shutDown() 
{
  d_func()->m_state = AudioManager::STATE_CLOSED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_AUDIO_NULL