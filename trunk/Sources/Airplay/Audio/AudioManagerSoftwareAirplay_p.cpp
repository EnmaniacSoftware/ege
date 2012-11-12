#if EGE_AUDIO_AIRPLAY && EGE_AIRPLAY_AUDIO_SOFTWARE

#include "Core/Application/Application.h"
#include "Airplay/Audio/AudioManagerSoftwareAirplay_p.h"
#include "Airplay/Audio/SoundSoftwareAirplay_p.h"
#include <s3eSound.h>
#include <s3e.h>
#include <EGEAudio.h>
#include <EGEDebug.h>
#include <EGEMath.h>

#include <EGEOverlay.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(AudioManagerPrivate)
EGE_DEFINE_DELETE_OPERATORS(AudioManagerPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define ENDED_SOUND_KEEP_ALIVE_DURATION 1.0f
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function clipping given value to S16 range. Taken from AIRPLAY SDK. */
static s16 ClipToS16(s32 value)
{
  enum
  {
    minval =  INT16_MIN,
    maxval =  INT16_MAX,
    allbits = UINT16_MAX
  };

  // quick overflow test, the addition moves valid range to 0-allbits
  if ((value - minval) & ~allbits)
  {
    // we overflowed.
    if (value > maxval)
    {
        value = maxval;
    }
    else if (value < minval)
    {
      value = minval;
    }
  }

  return (s16) value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerPrivate::AudioManagerPrivate(AudioManager* base) : m_d(base),
                                                               m_resampleFactor(0.0f),
                                                               m_mixing(false),
                                                               m_sound(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerPrivate::~AudioManagerPrivate()
{
  // unregister what is to be unregistered
  for (SoundsList::iterator it = m_sounds.begin(); it != m_sounds.end(); ++it)
  {
    SoundPrivate* sound = (*it)->p_func();

    // check if unregistering is needed
    if (0 == sound->keepAliveTimer().microseconds())
    {
      // set time to some positive value so all callback which can be called before unregistering takes place will exit immediately
      sound->keepAliveTimer() = ENDED_SOUND_KEEP_ALIVE_DURATION;

      // request unregistering and stop channel
      s3eSoundChannelStop(sound->channel());
      s3eSoundChannelUnRegister(sound->channel(), S3E_CHANNEL_END_SAMPLE);
      s3eSoundChannelUnRegister(sound->channel(), S3E_CHANNEL_GEN_AUDIO_STEREO);
      s3eSoundChannelUnRegister(sound->channel(), S3E_CHANNEL_GEN_AUDIO);
    }
  }

  // give some time to sound thread
  s3eDeviceYield(100);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool AudioManagerPrivate::isValid() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates manager. */
void AudioManagerPrivate::update(const Time& time)
{
  // update sounds
  for (SoundsList::iterator it = m_sounds.begin(); it != m_sounds.end();)
  {
    SoundPrivate* sound = (*it)->p_func();

    // check if sound is still being played
    if (0 == sound->keepAliveTimer().microseconds())
    {
      // updates buffers for given channel
      sound->updateBuffers();
    }
    else
    {
      // check if sound was marked as stopped
      if (ENDED_SOUND_KEEP_ALIVE_DURATION == sound->keepAliveTimer().seconds())
      {
        // clear callbacks
        // NOTE: Due to nature of callbacks setup (async) we request unregistration here and will keep sound alive for a while to make sure that
        //       if in the case any of the callbacks is called before it is unregistered we still have a valid sound object
        EGE_PRINT("AudioManagerPrivate::update - unregistering: %d %p", sound->channel(), sound);

        s3eSoundChannelStop(sound->channel());
        s3eSoundChannelUnRegister(sound->channel(), S3E_CHANNEL_END_SAMPLE);
        s3eSoundChannelUnRegister(sound->channel(), S3E_CHANNEL_GEN_AUDIO_STEREO);
        s3eSoundChannelUnRegister(sound->channel(), S3E_CHANNEL_GEN_AUDIO);
      }

      // update keep alive timer
      sound->keepAliveTimer() -= time;
      if (0 >= sound->keepAliveTimer().microseconds())
      {
        EGE_PRINT("AudioManagerPrivate::update - removing: %d %p", sound->channel(), sound);

        // now we can safely remove sound
        m_sounds.erase(it++);
        continue;
      }
    }

    ++it;

    //if (sound->p_func()->isDone() && (0 == s3eSoundChannelGetInt(channel, S3E_CHANNEL_STATUS)) && (0 == s3eSoundChannelGetInt(channel, S3E_CHANNEL_PAUSED)))
    //{

    //  EGE_PRINT("AudioManagerPrivate::update - removing channel: %d %p", channel, sound->p_func());
    //  m_activeSounds.erase(channel);
    //}
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
  int channel = s3eSoundGetFreeChannel();
  if (-1 != channel)
  {
    SoundPrivate* soundPrivate = sound->p_func();

    soundPrivate->keepAliveTimer() = 0.0f;

    // set channel properties
    //s3eSoundChannelSetInt(channel, S3E_CHANNEL_RATE, sound->codec()->frequency());
    //s3eSoundChannelSetInt(channel, S3E_CHANNEL_PITCH, sound->pitch());

    // register callbacks
    if (S3E_RESULT_SUCCESS != s3eSoundChannelRegister(channel, S3E_CHANNEL_END_SAMPLE, AudioManagerPrivate::SampleEndCallback, soundPrivate) ||
        S3E_RESULT_SUCCESS != s3eSoundChannelRegister(channel, S3E_CHANNEL_GEN_AUDIO, AudioManagerPrivate::AudioCallback, soundPrivate))
    {
      // error!
      return EGE_ERROR;
    }

    // check if stereo sound
    if (2 == sound->codec()->channels())
    {
      if (S3E_RESULT_SUCCESS != s3eSoundChannelRegister(channel, S3E_CHANNEL_GEN_AUDIO_STEREO, AudioManagerPrivate::AudioCallback, soundPrivate))
      {
        // error!
        return EGE_ERROR;
      }
    }

    // initially update all buffers
    soundPrivate->updateBuffers();

    // initialize filters
    soundPrivate->initializeFilter();

    // setup data for later use
    soundPrivate->setAudioManagerPrivate(this);
    soundPrivate->setChannel(channel);

    // play sound
    const PDataBuffer& firstBuffer = *soundPrivate->buffers().begin();
    if (S3E_RESULT_SUCCESS != s3eSoundChannelPlay(channel, reinterpret_cast<int16*>(firstBuffer->data()), static_cast<uint32>(firstBuffer->size() >> 1), 
                                                  1, 0))
    {
      // error!
      return EGE_ERROR;
    }

    EGE_PRINT("AudioManagerPrivate::play - channel: %d %p", channel, soundPrivate);

    // add list
    m_sounds.push_back(sound);
    return EGE_SUCCESS;
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Airplay callback function called once sample has ended. */
int32 AudioManagerPrivate::SampleEndCallback(void* systemData, void* userData)
{ 
  s3eSoundEndSampleInfo* info = reinterpret_cast<s3eSoundEndSampleInfo*>(systemData);

  SoundPrivate* sound = reinterpret_cast<SoundPrivate*>(userData);

  sound->keepAliveTimer().fromSeconds(ENDED_SOUND_KEEP_ALIVE_DURATION);

  return info->m_RepsRemaining;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Airplay callback function called everytime when more audio data is required. */
int32 AudioManagerPrivate::AudioCallback(void* systemData, void* userData)
{
  s3eSoundGenAudioInfo* info = reinterpret_cast<s3eSoundGenAudioInfo*>(systemData);
  // TAGE - garbage in info->m_OrigRepeats

  SoundPrivate* sound = reinterpret_cast<SoundPrivate*>(userData);

  AudioManagerPrivate* me = sound->audioManager();

  if (sound->areBuffersLocked() || (0 < sound->keepAliveTimer().microseconds()))
  {
    return 0;
  }

  me->m_mixing = (0 != info->m_Mix);
  me->m_sound = sound;

  sound->lockBuffers();

  AudioCodec* codec = sound->d_func()->codec();

  const s32 sampleSize = codec->channels() * sizeof (int16);

  // get list of sound buffers
  const SoundPrivate::BuffersList& buffers = sound->buffers();
  
  // calculate frequency resample factor
  const s32 gcd = Math::GreatestCommonDivisor(codec->frequency(), s3eSoundGetInt(S3E_SOUND_OUTPUT_FREQ));
  me->m_resampleFactor = (codec->frequency() / gcd) / static_cast<float32>(s3eSoundGetInt(S3E_SOUND_OUTPUT_FREQ) / gcd);

  // set flag indicating sound hasnt been completly played yet
  info->m_EndSample = 0;

  // reset count indicating how many samples have been generated this time
  s32 samplesPlayed = 0;

  // get pointer to first sample to generate
  int16* target = (int16*) info->m_Target;

  u32 samplesStillNeeded = info->m_NumSamples;

  // go thru all buffers (or till all requested samples hasnt been played yet)
  for (SoundPrivate::BuffersList::const_iterator it = buffers.begin(); it != buffers.end() && (0 < samplesStillNeeded); ++it)
  {
    const PDataBuffer& buffer = *it;

    // number of addressable samples in buffer
    u32 addressableSamplesInBuffer = static_cast<uint32>((buffer->size() - buffer->readOffset())) / sampleSize;

    // number of samples which can be processed to be in addressable range
    u32 availableSamplesInBuffer = static_cast<u32>(Math::Floor(addressableSamplesInBuffer / me->m_resampleFactor));

    // required number of addressable samples for currently needed samples
    u32 addressableSamplesStillNeeded = static_cast<u32>(Math::Ceil(samplesStillNeeded * me->m_resampleFactor));

    // determine number of 'base' samples according to what is addressable
    u32 samplesAvailable = (addressableSamplesStillNeeded < addressableSamplesInBuffer) ? samplesStillNeeded : availableSamplesInBuffer;

    // check if any samples still can be played
    if (0 < samplesAvailable)
    {
      // get pointer to begining of sample array 
      int16* data = reinterpret_cast<int16*>(buffer->data(buffer->readOffset()));

      // move read offset as we are going to reference buffer directly without any copying
      buffer->setReadOffset(buffer->readOffset() +  Math::Min(addressableSamplesStillNeeded, addressableSamplesInBuffer) * sampleSize);

      // upload depending on number of channels
      switch (codec->channels())
      {
        case 1:

          me->uploadMonoChannel(target, data, samplesAvailable);
          break;

        case 2:

          me->uploadStereoChannels(target, data, samplesAvailable);
          break;

        default:

          EGE_ASSERT(false && "Not supported yet!");
          break;
      }
    }

    // check if in the buffer still some data is present but not enough for even one refrequenced sample
    if ((samplesStillNeeded != samplesAvailable) && (buffer->size() != buffer->readOffset()))
    {
      // move read pointer to the end of buffer so it gets refilled
      buffer->setReadOffset(buffer->size());

      // skip sample on the edge
      ++samplesPlayed;
    }

    // update samples played counter
    samplesPlayed += samplesAvailable;

    // update requested number of samples counter
    samplesStillNeeded -= samplesAvailable;
  }

  // check if we played less samples than requested
  if (0 < samplesStillNeeded)
  {
    // check if no more data in codec
    if (sound->isDone())
    {
      // done
      info->m_EndSample = S3E_TRUE;
    }
    else
    {
      // audio is starved
      s3eDeviceYield(1);
    }
  }

  sound->unlockBuffers();

  return samplesPlayed;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Uploads mono channel into output buffer. 
 * @param out         Output buffer data in being uploaded into.
 * @param data        Input data buffer from which samples are to be mixed from.
 * @param count       Total number of samples to be uploaded.
 */
void AudioManagerPrivate::uploadMonoChannel(int16* out, int16* data, u32 count) const
{
  // go thru all samples
  for (register u32 i = 0; i < count; i++)
  {
    // calculate base sample index (with frequency resampling applied)
    u32 sampleIndex = static_cast<s32>(m_resampleFactor * i);

    // TAGE - debug
    //if (sampleIndex >= Math::Min(addressableSamplesStillNeeded, addressableSamplesInBuffer))
    //{
    //  int a = 1;
    //}

    // get input sample data
    int16 y = data[sampleIndex];

    if (1)
    {
        // apply FIR filter to output to remove artifacts     
        static int buffer_pos = -1;
        buffer_pos = (buffer_pos + 1) % 129;

        // use circular buffer to store previous inputs
        m_sound->m_filterBuffer[buffer_pos] = y;

        if (0)
        {
          y = 0;
          for (int i=0; i< 129; i++)
          {
              y += (int16)(m_sound->m_filterCoefficients[i] * m_sound->m_filterBuffer[(buffer_pos + i) % 129]);
          }
        }
    }

    // mix and store to output
    *out++ = ClipToS16(y + (m_mixing ? *out : 0));
  }  
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Uploads stereo channel into output buffer. 
 * @param out         Output buffer data in being uploaded into.
 * @param data        Input data buffer from which samples are to be mixed from.
 * @param count       Total number of samples to be uploaded.
 */
void AudioManagerPrivate::uploadStereoChannels(int16* out, int16* data, u32 count) const
{
  // go thru all samples
  for (register u32 i = 0; i < count; i++)
  {
    // calculate base sample index (with frequency resampling applied)
    u32 sampleIndex = static_cast<s32>(m_resampleFactor * (i << 1));

    // get input sample data
    int16 l = data[sampleIndex];
    int16 r = data[sampleIndex + 1];

    // mix and store to output
    *out++ = ClipToS16(l + (m_mixing ? *out : 0));
    *out++ = ClipToS16(r + (m_mixing ? *out : 0));
  }  
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Stops playback of the sound with a given name. */
void AudioManagerPrivate::stop(const String& soundName)
{
  // go thru all sounds
  for (SoundsList::iterator it = m_sounds.begin(); it != m_sounds.end(); ++it)
  {
    PSound& sound = *it;
    if (sound->name() == soundName)
    {
      // mark it to stop
      sound->p_func()->keepAliveTimer() = ENDED_SOUND_KEEP_ALIVE_DURATION;
      return;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if sound of a given name is being played. */
bool AudioManagerPrivate::isPlaying(const String& soundName) const
{
  // look for the sound in the active pool
  for (SoundsList::const_iterator it = m_sounds.begin(); it != m_sounds.end(); ++it)
  {
    const PSound& sound = *it;
    if (sound->name() == soundName)
    {
      // found
      return true;
    }
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_AUDIO_AIRPLAY && EGE_AIRPLAY_AUDIO_SOFTWARE