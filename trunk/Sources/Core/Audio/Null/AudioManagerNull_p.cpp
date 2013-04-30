#ifdef EGE_AUDIO_NULL

#include "Core/Application/Application.h"
#include "Core/Audio/Null/AudioManagerNull_p.h"
#include "Core/Audio/Null/SoundNull_p.h"
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
void AudioManagerPrivate::stop(const String& soundName)
{
  EGE_UNUSED(soundName)
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
EGEResult AudioManagerPrivate::pause(const String& soundName)
{
  EGE_UNUSED(soundName)

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult AudioManagerPrivate::pause(const PSound& sound)
{
  EGE_UNUSED(sound)

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AudioManagerPrivate::isPaused(const String& soundName) const
{
  EGE_UNUSED(soundName)

  return false;
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
List<PSound> AudioManagerPrivate::sounds(const String& soundName) const
{
  EGE_UNUSED(soundName);

  List<PSound> list;
  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerPrivate::setEnable(bool set)
{
  EGE_UNUSED(set)
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_AUDIO_NULL