#include "Core/Application/Application.h"
#include "Airplay/Audio/AudioManagerAirplay_p.h"
#include "Airplay/Audio/SoundAirplay_p.h"
#include <s3eSound.h>
#include <s3e.h>
#include <EGEAudio.h>
#include <EGEDebug.h>
#include <EGEMath.h>

#include <EGEOverlay.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(AudioManagerPrivate)
EGE_DEFINE_DELETE_OPERATORS(AudioManagerPrivate)

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
AudioManagerPrivate::AudioManagerPrivate(AudioManager* base) : m_d(base)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerPrivate::~AudioManagerPrivate()
{
  // clean up active channels
  for (ChannelsMap::const_iterator it = m_activeSounds.begin(); it != m_activeSounds.end();)
  {
    s3eSoundChannelStop(it->first);
  }
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
  // remove finished sounds
  for (ChannelsMap::const_iterator it = m_activeSounds.begin(); it != m_activeSounds.end();)
  {
    s32 channel = it->first;
    const PSound& sound = it->second;

    // updates buffers for given channel
      EGE_PRINT("AudioManagerPrivate::update - updating buffers");
    sound->p_func()->updateBuffers();

    // move iterator to next entry before potential removal
    ++it;

    if (sound->p_func()->isDone() && (0 == s3eSoundChannelGetInt(channel, S3E_CHANNEL_STATUS)) && (0 == s3eSoundChannelGetInt(channel, S3E_CHANNEL_PAUSED)))
    {
      EGE_PRINT("AudioManagerPrivate::update - removing channel: %d", channel);
      m_activeSounds.erase(channel);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Plays given sound. */
EGEResult AudioManagerPrivate::play(const PSound& sound)
{
  int channel = s3eSoundGetFreeChannel();
  if (-1 != channel)
  {
    SoundPrivate* soundPrivate = sound->p_func();

    // set channel properties
    //s3eSoundChannelSetInt(channel, S3E_CHANNEL_RATE, sound->codec()->frequency());
    
  //  s3eSoundChannelSetInt(channel, S3E_CHANNEL_PITCH, sound->pitch());

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

    // play sound
    const PDataBuffer& firstBuffer = *soundPrivate->buffers().begin();
    if (S3E_RESULT_SUCCESS != s3eSoundChannelPlay(channel, reinterpret_cast<int16*>(firstBuffer->data()), static_cast<uint32>(firstBuffer->size() >> 1), 
                                                  1, 0))
    {
      // error!
      return EGE_ERROR;
    }

    EGE_PRINT("AudioManagerPrivate::play - channel: %d", channel);

    // add to channels map
    m_activeSounds.insert(channel, sound);

    return EGE_SUCCESS;
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Airplay callback function called once sample has ended. */
int32 AudioManagerPrivate::SampleEndCallback(void* systemData, void* userData)
{ 
  s3eSoundEndSampleInfo* sampleInfo = reinterpret_cast<s3eSoundEndSampleInfo*>(systemData);

  SoundPrivate* sound = reinterpret_cast<SoundPrivate*>(userData);

  //EGE_PRINT("AudioManagerPrivate::SampleEndCallback - channel %d", sampleInfo->m_Channel);

  // unregister all if done to clean up
  if (0 == sampleInfo->m_RepsRemaining)
  {
    //s3eSoundChannelUnRegister(sampleInfo->m_Channel, S3E_CHANNEL_END_SAMPLE);
    //s3eSoundChannelUnRegister(sampleInfo->m_Channel, S3E_CHANNEL_GEN_AUDIO_STEREO);
    //s3eSoundChannelUnRegister(sampleInfo->m_Channel, S3E_CHANNEL_GEN_AUDIO);
  }

  return sampleInfo->m_RepsRemaining;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Airplay callback function called everytime when more audio data is required. */
int32 AudioManagerPrivate::AudioCallback(void* systemData, void* userData)
{
  s3eSoundGenAudioInfo* info = reinterpret_cast<s3eSoundGenAudioInfo*>(systemData);
  // TAGE - garbage in info->m_OrigRepeats

  SoundPrivate* sound = reinterpret_cast<SoundPrivate*>(userData);

  if (sound->areBuffersLocked())
  {
    return 0;
  }

  sound->lockBuffers();

  AudioCodec* codec = sound->d_func()->codec();

  // get list of sound buffers
  const SoundPrivate::BuffersList& buffers = sound->buffers();
  
  // calculate frequency resample factor
  const s32 gcd = Math::GreatestCommonDivisor(codec->frequency(), s3eSoundGetInt(S3E_SOUND_OUTPUT_FREQ));
  const float32 frequencyResampleFactor = (codec->frequency() / gcd) / static_cast<float32>(s3eSoundGetInt(S3E_SOUND_OUTPUT_FREQ) / gcd);

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

    // check how many samples we can get from current buffer
    u32 samplesAvailable = Math::Min(samplesStillNeeded, 
                                     static_cast<uint32>((buffer->size() - buffer->readOffset()) / (sizeof (int16) * codec->channels() * frequencyResampleFactor)));

    // check if any samples still can be played
    if (0 < samplesAvailable)
    {
      // get pointer to begining of sample array 
      int16* data = reinterpret_cast<int16*>(buffer->data(buffer->readOffset()));

      // move read offset as we are going to reference buffer directly without any copying
      buffer->setReadOffset(buffer->readOffset() + static_cast<s64>(samplesAvailable * frequencyResampleFactor) * sizeof (int16) * codec->channels());

      // mix all available samples
      for (uint32 i = 0; i < samplesAvailable; i++)
      {
        // calculate base sample index (with frequency resampling applied)
        // TAGE - this probably fail if factor is not multiplication of 2 :-/
        s32 sampleIndex = static_cast<s32>(frequencyResampleFactor * i) * codec->channels();

        // get previous data
        int16 currentValue = info->m_Mix ? *target : 0;

        //sprintf(szbuffer, "%d %d\n", data[sampleIndex], sampleIndex);
        //s3eDebugOutputString(szbuffer);

        // mix and store to output
        *target++ = ClipToS16(data[sampleIndex] + currentValue);
        if (info->m_Stereo)
        {
          currentValue = info->m_Mix ? *target : 0;

          // mix and store to output
          *target++ = ClipToS16(data[sampleIndex + 1] + currentValue);
        }
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

  // go thru all samples to be generated
  //for (uint32 i = 0; i < info->m_NumSamples; i++)
  //{
  //  // check if end of samples reached
  //  // NOTE: we do check while applying frequency factor. It seems input data is in sample frequency rather than output one ???
  //  if ((currentSamplePosition + i) * frequencyResampleFactor >= info->m_OrigNumSamples)
  //  {
  //    // end of sound reached
  //    info->m_EndSample = S3E_TRUE;

  //    //u64 dur = s3eTimerGetMs() - sound->play;
  //    //sprintf(buffer, "AudioCallback for channel %d done\n", info->m_Channel);
  //    //s3eDebugOutputString(buffer);
  //    break;
  //  }

  //  // calculate base sample index (with frequency resampling applied)
  //  s32 sampleIndex = static_cast<s32>(frequencyResampleFactor * (currentSamplePosition + i));

  //  int16 currentValue = 0;

  //  // check if stereo
  //  if (info->m_Stereo)
  //  {
  //    // NOTE: for stereo each sample constists of two int16's one for each channel
  //    // NOTE: if no mixing is necessary set it to 0. Otherwise, take what is currently in a target
  //    currentValue = info->m_Mix ? *target : 0;
  //    *target++ = ClipToS16(info->m_OrigStart[2 * sampleIndex] + currentValue);

  //    currentValue = info->m_Mix ? *target : 0;
  //    *target++ = ClipToS16(info->m_OrigStart[2 * sampleIndex + 1] + currentValue);
  //  }
  //  else
  //  {
  //    // NOTE: if no mixing is necessary set it to 0. Otherwise, take what is currently in a target
  //    currentValue = info->m_Mix ? *target : 0;
  //    *target++ = ClipToS16(info->m_OrigStart[sampleIndex] + currentValue);
  //  }

  //  samplesPlayed++;
  //}

  char szbuffer[245];
  sprintf(szbuffer, "AudioCallback for channel %d, mixing: %d %d\n", info->m_Channel, info->m_Mix, samplesPlayed);
  s3eDebugOutputString(szbuffer);

  sound->unlockBuffers();

  return samplesPlayed;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
