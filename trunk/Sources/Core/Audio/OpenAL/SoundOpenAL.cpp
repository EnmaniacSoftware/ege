#include "Core/Audio/OpenAL/SoundOpenAL.h"
#include "Core/Audio/OpenAL/AudioManagerOpenAL.h"
#include "EGEMath.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KSoundOpenALDebugName = "EGESoundOpenAL";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(SoundOpenAL)
EGE_DEFINE_DELETE_OPERATORS(SoundOpenAL)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundOpenAL::SoundOpenAL(AudioManagerOpenAL* manager, const String& name, PDataBuffer& data) : Sound(manager->app(), name, data),
                                                                                               m_manager(manager),
                                                                                               m_repeatsLeft(0),
                                                                                               m_state(StateNone),
                                                                                               m_format(0),
                                                                                               m_channel(0),
                                                                                               m_pitch(1.0f),
                                                                                               m_volume(1.0f)
{
  EGE_MEMSET(m_buffers, 0, sizeof (m_buffers));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundOpenAL::~SoundOpenAL()
{
  alDeleteBuffers(BUFFERS_COUNT, m_buffers);
  OAL_CHECK()
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SoundOpenAL::construct()
{
  // call base class
  EGEResult result = Sound::construct();
  if (EGE_SUCCESS == result)
  {
    // generate buffers
    alGenBuffers(BUFFERS_COUNT, m_buffers);
    if (IS_OAL_ERROR())
    {
      // error!
      egeCritical(KSoundOpenALDebugName) << "[OAL] Could not generate audio buffers.";
      EGE_MEMSET(m_buffers, 0, sizeof (m_buffers));
      return EGE_ERROR;
    }
  
 	  // determine OpenAL buffer format
    switch (m_codec->bitsPerSample())
    {
      case 8:

        m_format = (1 == m_codec->channels()) ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
        break;

      case 16:

        m_format = (1 == m_codec->channels()) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
        break;

      default:

        EGE_ASSERT_X(false, "Invalid bits per sample!");
        return EGE_ERROR_NOT_SUPPORTED;
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SoundOpenAL::update(const Time& time)
{
  // call base class
  Sound::update(time);

  // check if playing
  if (StatePlaying == state())
  {
    // updates sound buffers
    updateSoundBuffers();

    // check if channel stopped
    ALint state;
	  alGetSourcei(m_channel, AL_SOURCE_STATE, &state);
    if (IS_OAL_ERROR())
    {
      // error!
      egeCritical(KSoundOpenALDebugName) << "[OAL] Could not retrieve state.";
      return;
    }

    if (AL_PLAYING != state)
    {
      // check if not paused
      if (AL_PAUSED != state)
      {
        // check if there are any queued buffers. If so it means audio was starved and needs to be resumed
        ALint value;
		    alGetSourcei(m_channel, AL_BUFFERS_QUEUED, &value);
        if (IS_OAL_ERROR())
        {
          // error!
          egeCritical(KSoundOpenALDebugName) << "[OAL] Could not query queued buffers.";
          return;
        }

		    if (0 < value)
		    {
          // resume playback
			    alSourcePlay(m_channel);
          if (IS_OAL_ERROR())
          {
            // error!
            egeCritical(KSoundOpenALDebugName) << "[OAL] Could not start channel.";
          }
		    }
		    else
		    {
			    // finished
          notifyFinished();

          // stop
          setState(StateAboutToStop);
          doStop();
        }
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SoundOpenAL::setPitch(float32 value)
{
  if (value != m_pitch)
  {
    m_pitch = value;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 SoundOpenAL::pitch() const
{
  return m_pitch;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SoundOpenAL::setVolume(float32 volume)
{
  float32 oldVolume = m_volume;

  // clamp to valid range
  m_volume = Math::Bound(volume, 0.0f, 1.0f);

  // notify
  notifyVolumeChanged(oldVolume);

  // check if volume can be set
  if ((0 != m_channel) && (isPlaying() || (StatePaused == state())))
  {
    alSourcef(m_channel, AL_GAIN, volume);
	  if (IS_OAL_ERROR())
    {
      // error!
      egeCritical(KSoundOpenALDebugName) << "[OAL] Could not set volume! @" << name();
	  }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 SoundOpenAL::volume() const
{
  return m_volume;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SoundOpenAL::play(s32 repeatCount)
{
  EGEResult result = EGE_ERROR_ALREADY_EXISTS;

  // check if manager is enabled
  if (m_manager->isEnabled())
  {
    // check if not playing
    if ( ! isPlaying())
    {
      // check if not paused
      if ( ! isPaused())
      {
        // store repeat counter
        m_repeatsLeft = repeatCount;

        // set state
        setState(StateAboutToPlay);
      }

      // schedule playback
      result = m_manager->requestPlay(this); 
    }
  }
  else
  {
    // manager is disabled, so quitely treat it as success
    result = EGE_SUCCESS;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SoundOpenAL::isPlaying() const
{
  return (StatePlaying == state()) || (StateAboutToPlay == state());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SoundOpenAL::isPaused() const
{
  return (StatePaused == state()) || (StateAboutToPause == state());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SoundOpenAL::stop()
{
  // check if manager is enabled
  if (m_manager->isEnabled())
  {
    // check if not stopped
    if ((StateStopped != state()) && (StateNone != state()) && (StateAboutToStop != state()))
    {
      // schedule stop
      m_manager->requestStop(this);

      // set state
      setState(StateAboutToStop);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SoundOpenAL::pause()
{
  // check if manager is enabled
  if (m_manager->isEnabled())
  {
    // check if playing
    if (isPlaying())
    {
      // schedule pause
      m_manager->requestPause(this);

      // set state
      setState(StateAboutToPause);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SoundOpenAL::setState(State state)
{
  if (m_state != state)
  {
    m_state = state;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundOpenAL::State SoundOpenAL::state() const
{
  return m_state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SoundOpenAL::doPlay(ALuint channel)
{
  // store channel
  m_channel = channel;

  // initially update all buffers
  updateSoundBuffers();

	// set the pitch of the source
	alSourcef(m_channel, AL_PITCH, pitch());
	
  // set volume
  alSourcef(m_channel, AL_GAIN, volume());

	// disable looping
	alSourcei(m_channel, AL_LOOPING, AL_FALSE);
	
	// play the sound
	alSourcePlay(m_channel);
	if (IS_OAL_ERROR())
  {
    // error!
    egeCritical(KSoundOpenALDebugName) << "[OAL] Could not start playback!";
    return false;
	}

  ALint state;
	alGetSourcei(m_channel, AL_SOURCE_STATE, &state);
  if (IS_OAL_ERROR())
  {
    // error!
    egeCritical(KSoundOpenALDebugName) << "[OAL] Could not retrieve channel state.";
  }
  else
  {
    egeDebug(KSoundOpenALDebugName) << "[OAL] Starting sound" << name() << "at channel" << channel;
  }

  // set state
  setState(StatePlaying);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SoundOpenAL::doStop()
{
  // check if can be stopped
  if (StateAboutToStop == state())
  {
    alSourceStop(m_channel);
    alSourcei(m_channel, AL_BUFFER, 0);

    // unqueue all buffers
    ALint value;
	  alGetSourcei(m_channel, AL_BUFFERS_QUEUED, &value);
    for (s32 i = 0; i < value; ++i)
    {
      ALuint sink;
      alSourceUnqueueBuffers(m_channel, 1, &sink);
    }

	  // check to see if there were any errors
	  if (IS_OAL_ERROR())
    {
      // error!
      egeCritical(KSoundOpenALDebugName) << "[OAL] Could not stop playback.";
      return false;
	  }

    egeDebug(KSoundOpenALDebugName) << "[OAL] Stopped" << name() << "at channel" << m_channel;

    // reset data
    m_channel = 0;
    m_volume  = 1.0f;
    m_codec->reset();

    // notify stopped
    notifyStopped();
  }
  else
  {
    egeDebug(KSoundOpenALDebugName) << "[OAL] Could not stop" << name() << "at channel" << m_channel;
  }

  // set state
  setState(StateStopped);
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SoundOpenAL::doPause()
{
  // pause channel
  alSourcePause(m_channel);
	if (IS_OAL_ERROR())
  {
    // error!
    egeCritical(KSoundOpenALDebugName) << "[OAL] Could not pause playback!";
    return false;
	}

  // set state
  setState(StatePaused);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SoundOpenAL::doResume()
{
  return doPlay(m_channel);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SoundOpenAL::updateSoundBuffers()
{
	ALint buffersProcessed;

  // get channel type
  ALint channelType;
  alGetSourcei(m_channel, AL_SOURCE_TYPE, &channelType);
  if (IS_OAL_ERROR())
  {
    // error!
    egeCritical(KSoundOpenALDebugName) << "[OAL] Could not retrive channel type.";
    return;
  }

  // check if channel is not defined yet (uninitialized)
  if (AL_UNDETERMINED == channelType)
  {
    // make sure all buffers are updated initially
    buffersProcessed = BUFFERS_COUNT;
  }
  else
  {
  	// request the number of OpenAL buffers that have been processed (played and finished) for this channel
	  alGetSourcei(m_channel, AL_BUFFERS_PROCESSED, &buffersProcessed);
    if (IS_OAL_ERROR())
    {
      // error!
      egeCritical(KSoundOpenALDebugName) << "[OAL] Could not retrive number of processed buffers.";
      return;
    }
  }

  // for each processed buffer, remove it from the channel queue, read next chunk of audio data, fill buffer with it, and add it to the channel queue again
  ALuint index = 0;
  while (buffersProcessed)
  {
		ALuint bufferId = m_buffers[index];

    // remove the buffer from the channel queue
    // NOTE: do this only if channel is already initialized (ie. not during initial setup)
    if (AL_UNDETERMINED != channelType)
    {
      // NOTE: bufferId will hold id of processed buffer
		  alSourceUnqueueBuffers(m_channel, 1, &bufferId);
      if (IS_OAL_ERROR())
      {
        // error!
        egeCritical(KSoundOpenALDebugName) << "[OAL] Could not get processed buffers.";
      }
    }

    // upload 250ms audio data to buffer
    DataBuffer data;
    s32 samplesDecoded = 0;
    bool endOfData = m_codec->decode(data, m_codec->frequency() >> 2, samplesDecoded);

    if (0 < samplesDecoded)
    {
      // copy data into buffer
      alBufferData(bufferId, m_format, data.data(), static_cast<ALsizei>(data.size()), m_codec->frequency());
      if (IS_OAL_ERROR())
      {
        // error!
        egeCritical(KSoundOpenALDebugName) << "[OAL] Could not bind data to buffer at channel" << m_channel;
      }

			// add buffer to channel
			alSourceQueueBuffers(m_channel, 1, &bufferId);
      if (IS_OAL_ERROR())
      {
        // error!
        egeCritical(KSoundOpenALDebugName) << "[OAL] Could not queue buffer to channel @" << m_channel;
      }
    }
  
    // check if no more data in codec
    if (endOfData)
    {
      // check if any pending repeats
      if ((0 > m_repeatsLeft) || (1 <= m_repeatsLeft))
      {
        // update repeat counter
        if (0 < m_repeatsLeft)
        {
          --m_repeatsLeft;
        }

        // reset codec
        m_codec->reset();
      }
    }

    // proceed to next buffer
		--buffersProcessed;
    ++index;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerOpenAL* SoundOpenAL::manager() const
{
  return m_manager;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 SoundOpenAL::repeatCount() const
{
  return m_repeatsLeft;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

