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
SoundPrivate::SoundPrivate(Sound* base) : m_d(base), m_format(0)
{
  EGE_MEMSET(m_buffers, 0, sizeof (m_buffers));

  // generate buffers
  alGenBuffers(BUFFERS_COUNT, m_buffers);
  if (AL_NO_ERROR != alGetError())
  {
    // error!
    EGE_PRINT("SoundPrivate::SoundPrivate - could not generate audio buffers.");
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
      EGE_PRINT("SoundPrivate::~SoundPrivate - could not delete audio buffers.");
    }
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
/*! Updates buffers. This is called by AudioManagerPrivate. 
 *  @param channel OpenAL channel (source) id for which buffers should be updated.
 */
void SoundPrivate::updateBuffers(ALuint channel)
{
  AudioCodec* codec = d_func()->codec();

	ALint buffersProcessed;

  // get channel type
  ALint channelType;
  alGetSourcei(channel, AL_SOURCE_TYPE, &channelType);
  if (AL_NO_ERROR != alGetError())
  {
    // error!
    EGE_PRINT("SoundPrivate::updateBuffers - could not retrive channel type.");
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
	  alGetSourcei(channel, AL_BUFFERS_PROCESSED, &buffersProcessed);
    if (AL_NO_ERROR != alGetError())
    {
      // error!
      EGE_PRINT("SoundPrivate::updateBuffers - could not retrive number of processed buffers.");
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
		  alSourceUnqueueBuffers(channel, 1, &bufferId);
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
        EGE_PRINT("SoundPrivate::updateBuffer - could not bind data to buffer.");
      }

			// add buffer to channel
			alSourceQueueBuffers(channel, 1, &bufferId);
      if (AL_NO_ERROR != alGetError())
      {
        // error!
        EGE_PRINT("SoundPrivate::updateBuffer - could queue buffer to channel.");
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
#endif // EGE_AUDIO_OPENAL