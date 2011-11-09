#ifdef EGE_AUDIO_OPENAL
#include "Core/Application/Application.h"
#include "Core/Audio/OpenAL/AudioManagerOpenAL_p.h"
#include "Core/Audio/OpenAL/SoundOpenAL_p.h"
#include <EGEAudio.h>
#include <EGEDebug.h>

EGE_NAMESPACE

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
  if (m_device)
  {
    m_context = alcCreateContext(m_device, NULL);
    alcMakeContextCurrent(m_context);

    // preallocate channels
    alGenSources(CHANNELS_COUNT, m_channels);
    if (AL_NO_ERROR != alGetError())
    {
      EGE_PRINT("AudioManagerPrivate::AudioManagerPrivate - Could not generate sources.");
      EGE_MEMSET(m_channels, 0, sizeof (m_channels));
    }
  }
  else
  {
    EGE_LOG("AudioManagerPrivate::AudioManagerPrivate - Could not open device");
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
  // move thru all active channels
  for (ChannelsMap::const_iterator it = m_activeChannels.begin(); it != m_activeChannels.end();)
  {
    ALuint channel = it->first;
    const PSound& sound = it->second;

    // updates buffers for given channel
    sound->p_func()->updateBuffers(channel);

    // move iterator to next entry before potential removal
    ++it;

    // check if channel stopped
    ALint value;
		alGetSourcei(channel, AL_SOURCE_STATE, &value);
    if (AL_PLAYING != value)
    {
      // check if there are any queued buffers. If so it means audio was starved and needs to be resumed
			alGetSourcei(channel, AL_BUFFERS_QUEUED, &value);
			if (value)
			{
        // resume playback
				alSourcePlay(channel);
			}
			else
			{
				// finished
				alSourceStop(channel);
				alSourcei(channel, AL_BUFFER, 0);			

        // remove from pool
        m_activeChannels.erase(channel);
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Plays given sound.
 * @param sound       Sound to play.
 * @param repeatCount Number of times sound should be repeated.
 * @return  Returns EGE_SUCCESS if sound is sucessfully started or EGE_ERROR if sound could not be started.
 * @note  When repeatCount is set to zero the sound is going to be played exactly once. For negative values sound will be played forever.
 */
EGEResult AudioManagerPrivate::play(const PSound& sound, s32 repeatCount)
{
  EGE_UNUSED(repeatCount);

  ALuint channel = availableChannel();
  if (0 < channel)
  {
    SoundPrivate* soundPrivate = sound->p_func();

    // clean up source
	  alSourcei(channel, AL_BUFFER, 0);

    // initially update all buffers
    soundPrivate->updateBuffers(channel);

	  // set the pitch and gain of the source
	  alSourcef(channel, AL_PITCH, sound->pitch());
	  alSourcef(channel, AL_GAIN, sound->gain());
	
	  // set the looping value
		//alSourcei(channel, AL_LOOPING, sound->looping() ? AL_TRUE : AL_FALSE);
		
	  // play the sound
	  alSourcePlay(channel);

	  // check to see if there were any errors
	  ALenum error = alGetError();
	  if (AL_NO_ERROR != error)
    {
      // error!
      EGE_PRINT("AudioManagerPrivate::play - could not start playback!");
      return EGE_ERROR;
	  }

    // add to channels map
    m_activeChannels.insert(channel, sound);

    return EGE_SUCCESS;
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

		// if this source is not playing then return it
		if (AL_PLAYING != state)
    {
      return m_channels[i];
    }
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Stops playback of the sound with a given name. */
void AudioManagerPrivate::stop(const String& soundName)
{
  // go thru all sounds
  for (ChannelsMap::iterator it = m_activeChannels.begin(); it != m_activeChannels.end(); ++it)
  {
    PSound& sound = it->second;
    if (sound->name() == soundName)
    {
      // stop sound
      alSourceStop(it->first);
			alSourcei(it->first, AL_BUFFER, 0);			

      // remove from pools
      m_activeChannels.erase(it);
      return;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if sound of a given name is being played. */
bool AudioManagerPrivate::isPlaying(const String& soundName) const
{
  // look for the sound in the active pool
  for (ChannelsMap::const_iterator it = m_activeChannels.begin(); it != m_activeChannels.end(); ++it)
  {
    const PSound& sound = it->second;
    if (sound->name() == soundName)
    {
      // found
      return true;
    }
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif // EGE_AUDIO_OPENAL