#if EGE_AUDIO_AIRPLAY && !EGE_AIRPLAY_AUDIO_SOFTWARE

#include "Core/Application/Application.h"
#include "Airplay/Audio/AudioManagerAirplay_p.h"
#include "Airplay/Audio/SoundAirplay_p.h"
#include <s3eSound.h>
#include <s3eAudio.h>
#include <s3e.h>
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
/*! Updates manager. */
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
/*! Plays given sound.
 * @param sound Sound to play.
 * @return  Returns EGE_SUCCESS if sound is sucessfully started or EGE_ERROR if sound could not be started.
 */
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
/*! Stops playback of the given sound. */
EGEResult AudioManagerPrivate::stop(const PSound& sound)
{
  return sound->p_func()->stop();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if given sound is being played. */
bool AudioManagerPrivate::isPlaying(const PSound& sound) const
{
  return sound->p_func()->isPlaying();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Pauses given sound.
 *  @param sound Sound to pause.
 *  @return  Returns EGE_SUCCESS if sound is sucessfully paused or EGE_ERROR if sound could not be paused.
 */
EGEResult AudioManagerPrivate::pause(const PSound& sound)
{
  return sound->p_func()->pause();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if given sound is paused. */
bool AudioManagerPrivate::isPaused(const PSound& sound) const
{
  return sound->p_func()->isPaused();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if given sound is stopped. */
bool AudioManagerPrivate::isStopped(const PSound& sound) const
{
  return sound->p_func()->isStopped();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Shuts manager down. */
void AudioManagerPrivate::shutDown() 
{ 
  d_func()->m_state = AudioManager::STATE_CLOSED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_AUDIO_AIRPLAY && !EGE_AIRPLAY_AUDIO_SOFTWARE
