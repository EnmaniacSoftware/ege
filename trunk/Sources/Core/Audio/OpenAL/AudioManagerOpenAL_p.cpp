#ifdef EGE_AUDIO_OPENAL
#include "Core/Application/Application.h"
#include "Core/Audio/OpenAL/AudioManagerOpenAL_p.h"
#include "Core/Audio/OpenAL/SoundOpenAL_p.h"
#include <EGEAudio.h>
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define CHANNELS_COUNT 24
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(AudioManagerPrivate)
EGE_DEFINE_DELETE_OPERATORS(AudioManagerPrivate)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerPrivate::AudioManagerPrivate(AudioManager* base) : m_device(NULL),
                                                               m_context(NULL)
{
  m_device = alcOpenDevice(NULL);
  if (m_device)
  {
    m_context = alcCreateContext(m_device, NULL);
    alcMakeContextCurrent(m_context);

    // preallocate channels
    for (s32 i = 0; i < CHANNELS_COUNT; ++i)
    {
      // generate OpenAL source
      ALuint channel;
      alGenSources(1, &channel);
      if (AL_NO_ERROR == alGetError())
      {
        // add to pool
        m_channels.push_back(channel);
      }
      else
      {
        EGE_PRINT("AudioManagerPrivate::AudioManagerPrivate - Could not generate source.");
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerPrivate::~AudioManagerPrivate()
{
  alcMakeContextCurrent(NULL);
  
  // deallocate sources
  for (ChannelsArray::const_iterator it = m_channels.begin(); it != m_channels.end(); ++it)
  {
    alDeleteSources(1, &(*it));
  }
  m_channels.clear();

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
  return (NULL != m_device) && (NULL != m_context) && (CHANNELS_COUNT == m_channels.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates manager. */
void AudioManagerPrivate::update(const Time& time)
{
  // remove finished sounds
  for (ChannelsMap::const_iterator it = m_activeSounds.begin(); it != m_activeSounds.end();)
  {
    ALuint channel = it->first;
    const PSound& sound = it->second;

    ++it;

    ALint state;
		alGetSourcei(channel, AL_SOURCE_STATE, &state);

		// check if done
		if (AL_STOPPED == state)
    {
      // detach sources from channel
	    alSourcei(channel, AL_BUFFER, 0);

      // remove from pool
      m_activeSounds.erase(channel);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Plays given sound. */
EGEResult AudioManagerPrivate::play(const PSound& sound)
{
  ALuint channel = availableChannel();
  if (0 < channel)
  {
    // clean up source
	  alSourcei(channel, AL_BUFFER, 0);

	  // attached sound buffer to source
	  alSourcei(channel, AL_BUFFER, sound->p_func()->bufferId());
	
	  // set the pitch and gain of the source
	  alSourcef(channel, AL_PITCH, sound->pitch());
	  alSourcef(channel, AL_GAIN, sound->gain());
	
	  // set the looping value
		alSourcei(channel, AL_LOOPING, sound->looping() ? AL_TRUE : AL_FALSE);
		
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
    m_activeSounds.insert(channel, sound);

    return EGE_SUCCESS;
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns first available channel. */
ALuint AudioManagerPrivate::availableChannel() const
{
  // go thru all channels
  for (ChannelsArray::const_iterator it = m_channels.begin(); it != m_channels.end(); ++it)
  {
    ALint state;
		alGetSourcei(*it, AL_SOURCE_STATE, &state);

		// if this source is not playing then return it
		if (AL_PLAYING != state)
    {
      return *it;
    }
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif // EGE_AUDIO_OPENAL