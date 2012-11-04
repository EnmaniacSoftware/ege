#ifdef EGE_AUDIO_OPENAL
#include "Core/Application/Application.h"
#include "Core/Audio/OpenAL/AudioManagerOpenAL_p.h"
#include "Core/Audio/OpenAL/SoundOpenAL_p.h"
#include <EGEAudio.h>
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static ALenum l_result;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define IS_AL_ERROR() (AL_NO_ERROR != (l_result = alGetError()))
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(AudioManagerPrivate)
EGE_DEFINE_DELETE_OPERATORS(AudioManagerPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerPrivate::AudioManagerPrivate(AudioManager* base) : m_d(base),
                                                               m_device(NULL),
                                                               m_context(NULL)
{
  EGE_MEMSET(m_channels, 0, sizeof (m_channels));

  m_device = alcOpenDevice(NULL);
  if (NULL != m_device)
  {
    m_context = alcCreateContext(m_device, NULL);
    alcMakeContextCurrent(m_context);

    // preallocate channels
    alGenSources(CHANNELS_COUNT, m_channels);
    if (IS_AL_ERROR())
    {
      egeCritical() << l_result << "Could not generate sources.";
      EGE_MEMSET(m_channels, 0, sizeof (m_channels));
    }
  }
  else
  {
    egeCritical() << "Could not open device";
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerPrivate::~AudioManagerPrivate()
{
  alcMakeContextCurrent(NULL);
  
  if (isValid())
  {
    alDeleteSources(CHANNELS_COUNT, m_channels);
    EGE_MEMSET(m_channels, 0, sizeof (m_channels));
  }

  // destroy context
  if (m_context)
  {
    alcDestroyContext(m_context);
    m_context = NULL;
  }

  // destroy audio device
  if (m_device)
  {
    alcCloseDevice(m_device);
    m_device = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool AudioManagerPrivate::isValid() const
{
  // check all channels
  for (s32 i = 0; i < CHANNELS_COUNT; ++i)
  {
    if (0 == m_channels[i])
    {
      // not valid
      return false;
    }
  }

  return (NULL != m_device) && (NULL != m_context);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates manager. */
void AudioManagerPrivate::update(const Time& time)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Plays given sound.
 * @param sound Sound to play.
 * @return  Returns EGE_SUCCESS if sound is sucessfully started or EGE_ERROR if sound could not be started.
 */
EGEResult AudioManagerPrivate::play(const PSound& sound)
{
  // check if paused
  if (isPaused(sound))
  {
    // resume playback
    return sound->p_func()->resume();
  }

  // try to find available channel
  ALuint channel = availableChannel();
  if (0 < channel)
  {
    SoundPrivate* soundPrivate = sound->p_func();

    // start playback
    return soundPrivate->play(channel);
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns first available channel. */
ALuint AudioManagerPrivate::availableChannel() const
{
  // go thru all channels
  for (s32 i = 0; i < CHANNELS_COUNT; ++i)
  {
    ALint state;
		alGetSourcei(m_channels[i], AL_SOURCE_STATE, &state);
    if (IS_AL_ERROR())
    {
      // error!
      egeCritical() << l_result << "Could not retrieve channel state:" << m_channels[i] << "at index" << i;
      continue;
    }

		// if this source is not playing/paused then return it
		if ((AL_PLAYING != state) && (AL_PAUSED != state))
    {
      return m_channels[i];
    }
  }

  return 0;
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

#endif // EGE_AUDIO_OPENAL