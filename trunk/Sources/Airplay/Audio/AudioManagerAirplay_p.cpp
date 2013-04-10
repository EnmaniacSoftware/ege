#if EGE_AUDIO_AIRPLAY && !EGE_AIRPLAY_AUDIO_SOFTWARE

#include "Core/Application/Application.h"
#include "Airplay/Audio/AudioManagerAirplay_p.h"
#include "Airplay/Audio/SoundAirplay_p.h"
#include <s3eSound.h>
#include <s3eAudio.h>
#include <s3e.h>
#include <s3eIOSAudioRoute.h>
#include <s3eIOSBackgroundAudio.h>
#include <EGEAudio.h>
#include <EGEDebug.h>
#include <EGEMath.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static s16 l_emptySoundSampleData[1];
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(AudioManagerPrivate)
EGE_DEFINE_DELETE_OPERATORS(AudioManagerPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerPrivate::AudioManagerPrivate(AudioManager* base) : m_d(base)
{
  // NOTE: we set an empty sound to be played here so Airplay sound system gets initialized
  l_emptySoundSampleData[0] = 0;

  // get free channel
  s32 channel = s3eSoundGetFreeChannel();
  if (-1 != channel)
  {
    s3eSoundChannelSetInt(channel, S3E_CHANNEL_RATE, 22050);
    s3eSoundChannelPlay(channel, l_emptySoundSampleData, 1, 1, 0);
  }

  // check if audio routing is available
  if (s3eIOSAudioRouteAvailable())
  {
    s3eIOSAudioRouteSetCategory(S3E_IOSAUDIOROUTE_CATEGORY_AMBIENT);
    s3eIOSBackgroundAudioSetMix(true);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerPrivate::~AudioManagerPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Constructs object. */
EGEResult AudioManagerPrivate::construct()
{
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerPrivate::update(const Time& time)
{
  // go thru all sounds
  // NOTE: no const iterator due to GCC compiler complaints
  for (AudioManager::SoundList::iterator it = d_func()->m_sounds.begin(); it != d_func()->m_sounds.end();)
  {
    const PSound& sound = *it;

    //egeDebug()  << "Updating sound" << sound->name();

    // check if sound is stopped
    if (isStopped(sound))
    {
      d_func()->m_sounds.erase(it++);
      continue;
    }

    // update sound
    sound->update(time);
    ++it;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult AudioManagerPrivate::play(const PSound& sound)
{
  EGEResult result = EGE_ERROR;

  // try to start
  if (EGE_SUCCESS == (result = sound->p_func()->play()))
  {
    // connect
    ege_connect(sound, stopped, d_func(), AudioManager::onStopped);

    // add to pool
    d_func()->m_sounds.push_back(sound);
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult AudioManagerPrivate::stop(const PSound& sound)
{
  return sound->p_func()->stop();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AudioManagerPrivate::isPlaying(const PSound& sound) const
{
  return sound->p_func()->isPlaying();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AudioManagerPrivate::isPlaying(const String& soundName) const
{
  EGE_UNUSED(soundName);

  // nothing to do, all is already done in AudioManager
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult AudioManagerPrivate::pause(const PSound& sound)
{
  return sound->p_func()->pause();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AudioManagerPrivate::isPaused(const PSound& sound) const
{
  return sound->p_func()->isPaused();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AudioManagerPrivate::isStopped(const PSound& sound) const
{
  return sound->p_func()->isStopped();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerPrivate::shutDown() 
{ 
  d_func()->m_state = AudioManager::STATE_CLOSED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_AUDIO_AIRPLAY && !EGE_AIRPLAY_AUDIO_SOFTWARE
