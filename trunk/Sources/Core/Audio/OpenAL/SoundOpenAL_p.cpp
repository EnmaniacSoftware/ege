#ifdef EGE_AUDIO_OPENAL
#include "Core/Application/Application.h"
#include "Core/Audio/OpenAL/SoundOpenAL_p.h"
#include "Core/Audio/Sound.h"
#include "Core/Audio/AudioCodec.h"
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(SoundPrivate)
EGE_DEFINE_DELETE_OPERATORS(SoundPrivate)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundPrivate::SoundPrivate(Sound* base) : m_d(base), 
                                          m_format(0),
                                          m_channel(0),
                                          m_stopped(false)
{
  EGE_MEMSET(m_buffers, 0, sizeof (m_buffers));

  // generate buffers
  alGenBuffers(BUFFERS_COUNT, m_buffers);
  if (AL_NO_ERROR != alGetError())
  {
    // error!
    EGE_PRINT("ERROR: Could not generate audio buffers.");
    EGE_MEMSET(m_buffers, 0, sizeof (m_buffers));
  }
  
 	// determine OpenAL buffer format
  ALenum format = AL_FORMAT_STEREO16;
  switch (d_func()->codec()->bitsPerSample())
  {
    case 8:

      m_format = (1 == d_func()->codec()->channels()) ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
      break;

    case 16:

      m_format = (1 == d_func()->codec()->channels()) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
      break;

    default:

      EGE_ASSERT("Invalid bits per sample!");
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundPrivate::~SoundPrivate()
{
  if (isValid())
  {
    alDeleteBuffers(BUFFERS_COUNT, m_buffers);
    if (AL_NO_ERROR != alGetError())
    {
      // error!
      EGE_PRINT("ERROR: Could not delete audio buffers.");
    }

    // reset
    EGE_MEMSET(m_buffers, 0, sizeof (m_buffers));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool SoundPrivate::isValid() const
{
  // check all buffers
  for (s32 i = 0; i < BUFFERS_COUNT; ++i)
  {
    if (0 == m_buffers[i])
    {
      // not valid
      return false;
    }
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates object. */
void SoundPrivate::update(const Time& time)
{
  if (!m_stopped)
  {
    // updates buffers for given channel
    updateBuffers();

    // check if channel stopped
    ALint state;
	  alGetSourcei(m_channel, AL_SOURCE_STATE, &state);
    if (AL_PLAYING != state)
    {
      // check if there are any queued buffers. If so it means audio was starved and needs to be resumed
      ALint value;
		  alGetSourcei(m_channel, AL_BUFFERS_QUEUED, &value);
		  if (value)
		  {
        // resume playback
			  alSourcePlay(m_channel);
		  }
		  else
		  {
			  // finished
        d_func()->notifyFinished();

        // stop
        stop();
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates sound buffers. */
void SoundPrivate::updateBuffers()
{
  AudioCodec* codec = d_func()->codec();

	ALint buffersProcessed;

  // get channel type
  ALint channelType;
  alGetSourcei(m_channel, AL_SOURCE_TYPE, &channelType);
  if (AL_NO_ERROR != alGetError())
  {
    // error!
    EGE_PRINT("ERROR: Could not retrive channel type.");
    return;
  }

  // check if channel is not defined yet (uninitialized)
  if (AL_UNDETERMINED == channelType)
  {
    // make sure all buffers are updates initially
    buffersProcessed = BUFFERS_COUNT;
  }
  else
  {
  	// request the number of OpenAL buffers that have been processed (played and finished) for this channel
	  alGetSourcei(m_channel, AL_BUFFERS_PROCESSED, &buffersProcessed);
    if (AL_NO_ERROR != alGetError())
    {
      // error!
      EGE_PRINT("ERROR: Could not retrive number of processed buffers.");
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
    }

    // upload 250ms audio data to buffer
    DataBuffer data;
    s32 samplesDecoded = 0;
    bool endOfData = codec->decode(data, codec->frequency() >> 2, samplesDecoded);
    if (0 < samplesDecoded)
    {
      // copy data into buffer
      alBufferData(bufferId, m_format, data.data(), static_cast<ALsizei>(data.size()), codec->frequency());
      if (AL_NO_ERROR != alGetError())
      {
        // error!
        EGE_PRINT("ERROR: Could not bind data to buffer.");
      }

			// add buffer to channel
			alSourceQueueBuffers(m_channel, 1, &bufferId);
      if (AL_NO_ERROR != alGetError())
      {
        // error!
        EGE_PRINT("ERROR: Could queue buffer to channel.");
      }
    }
  
    // check if no more data in codec
    if (endOfData)
    {
      // check if any pending repeats
      s32 repeatsLeft = d_func()->repeatsLeft();
      if ((0 > repeatsLeft) || (1 <= repeatsLeft))
      {
        // update repeat counter
        if (0 < repeatsLeft)
        {
          d_func()->setRepeatCount(repeatsLeft -1);
        }

        // reset codec
        codec->reset();
      }
    }

    // proceed to next buffer
		--buffersProcessed;
    ++index;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Starts playback on given channel. */
EGEResult SoundPrivate::play(ALuint channel)
{
  // store channel
  m_channel = channel;

  // initially update all buffers
  updateBuffers();

	// set the pitch of the source
	alSourcef(m_channel, AL_PITCH, d_func()->pitch());
	
	// set the looping value
	//alSourcei(channel, AL_LOOPING, sound->looping() ? AL_TRUE : AL_FALSE);
		
	// play the sound
	alSourcePlay(m_channel);

	// check to see if there were any errors
	ALenum error = alGetError();
	if (AL_NO_ERROR != error)
  {
    // error!
    EGE_PRINT("ERROR: Could not start playback!");
    return EGE_ERROR;
	}

  // connect for sound volume changes and set initial volume
  ege_connect(d_func(), volumeChanged, this, SoundPrivate::onSoundVolumeChanged);
  d_func()->setVolume(d_func()->volume());

  // reset flag
  m_stopped = false;

  //EGE_PRINT("SoundPrivate::play - %s", d_func()->name().toAscii());

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Stops playback. */
EGEResult SoundPrivate::stop()
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
	ALenum error = alGetError();
	if (AL_NO_ERROR != error)
  {
    // error!
    EGE_PRINT("ERROR: %s!", d_func()->name().toAscii());
    return EGE_ERROR;
	}

  EGE_PRINT("ERROR: %s", d_func()->name().toAscii());

  // notify stopped
  d_func()->notifyStopped();

  // set flag to prevent this sound from being updated
  m_stopped = true;

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if sound is being played. */
bool SoundPrivate::isPlaying() const
{
  ALint state;
	alGetSourcei(m_channel, AL_SOURCE_STATE, &state);

  return AL_PLAYING == state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called on sound volume change. */
void SoundPrivate::onSoundVolumeChanged(const Sound* sound, float32 oldVolume)
{
  EGE_ASSERT(sound->p_func() == this);

  //EGE_PRINT("SoundPrivate::onSoundVolumeChanged: %s %.2f -> %.2f", sound->name().toAscii(), oldVolume, sound->volume());

  // set new volume
  alSourcef(m_channel, AL_GAIN, sound->volume());

  // check to see if there were any errors
	ALenum error = alGetError();
	if (AL_NO_ERROR != error)
  {
    // error!
    EGE_PRINT("ERROR: %s could not set volume!", sound->name().toAscii());
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif // EGE_AUDIO_OPENAL
