#include "Core/Application/Application.h"
#include "Airplay/Audio/AudioManagerAirplay_p.h"
#include "Airplay/Audio/SoundAirplay_p.h"
#include <s3eSound.h>
#include <s3e.h>
#include <EGEAudio.h>
#include <EGEDebug.h>
#include <EGEMath.h>

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
AudioManagerPrivate::AudioManagerPrivate(AudioManager* base)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerPrivate::~AudioManagerPrivate()
{
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

    ++it;

    if (sound->p_func()->samplesCount() == sound->p_func()->samplesPlayed())
    {
      EGE_PRINT("ERASING channel: %d", channel);
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
    s3eSoundChannelSetInt(channel, S3E_CHANNEL_RATE, soundPrivate->sampleRate());
  //  s3eSoundChannelSetInt(channel, S3E_CHANNEL_PITCH, sound->pitch());

    // register callbacks
    if (S3E_RESULT_SUCCESS != s3eSoundChannelRegister(channel, S3E_CHANNEL_END_SAMPLE, AudioManagerPrivate::SampleEndCallback, soundPrivate) ||
        S3E_RESULT_SUCCESS != s3eSoundChannelRegister(channel, S3E_CHANNEL_GEN_AUDIO, AudioManagerPrivate::AudioCallback, soundPrivate))
    {
      // error!
      return EGE_ERROR;
    }

    // check if stereo sound
    if (2 == soundPrivate->channels())
    {
      if (S3E_RESULT_SUCCESS != s3eSoundChannelRegister(channel, S3E_CHANNEL_GEN_AUDIO_STEREO, AudioManagerPrivate::AudioCallback, soundPrivate))
      {
        // error!
        return EGE_ERROR;
      }
    }

    // play sound
    if (S3E_RESULT_SUCCESS != s3eSoundChannelPlay(channel, reinterpret_cast<int16*>(soundPrivate->data()->data(soundPrivate->soundDataOffset())), 
                                                  soundPrivate->samplesCount(), 0, 0))
    {
      // error!
      return EGE_ERROR;
    }

    EGE_PRINT("Playing at channel: %d", channel);

    // reset sound data
    soundPrivate->setSamplesPlayed(0);

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

  sound->setSamplesPlayed(sound->samplesCount());

  //char buffer[64];
  //sprintf(buffer, "FINALIZE: channel %d", sampleInfo->m_Channel);
  //s3eDebugOutputString(buffer);

  // unregister ??
  // TAGE this should probably be done only once we r really done (after all repets etc)
  s3eSoundChannelUnRegister(sampleInfo->m_Channel, S3E_CHANNEL_END_SAMPLE);
  s3eSoundChannelUnRegister(sampleInfo->m_Channel, S3E_CHANNEL_GEN_AUDIO_STEREO);
  s3eSoundChannelUnRegister(sampleInfo->m_Channel, S3E_CHANNEL_GEN_AUDIO);

  return sampleInfo->m_RepsRemaining;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Airplay callback function called everytime when more audio data is required. */
int32 AudioManagerPrivate::AudioCallback(void* systemData, void* userData)
{
  s3eSoundGenAudioInfo* info = reinterpret_cast<s3eSoundGenAudioInfo*>(systemData);

  SoundPrivate* sound = reinterpret_cast<SoundPrivate*>(userData);

  // calculate frequency resample factor
  const s32 gcd = Math::GreatestCommonDivisor(sound->sampleRate(), s3eSoundGetInt(S3E_SOUND_OUTPUT_FREQ));
  const float32 frequencyResampleFactor = (sound->sampleRate() / gcd) / static_cast<float32>(s3eSoundGetInt(S3E_SOUND_OUTPUT_FREQ) / gcd);

  // get number of played samples so far
  s32 currentSamplePosition = sound->samplesPlayed();

  // set flag indicating sound hasnt been completly played yet
  info->m_EndSample = 0;

  // reset count indicating how many samples have been generated this time
  s32 samplesPlayed = 0;

  // get pointer to first sample to generate
  int16* target = (int16*) info->m_Target;

  char buffer[245];
  //sprintf(buffer, "AudioCallback for channel %d, mixing: %d STM: %d AS: %d (%d)\n", info->m_Channel, info->m_Mix, info->m_NumSamples, currentSamplePosition, info->m_OrigNumSamples);
  //s3eDebugOutputString(buffer);

  // go thru all samples to be generated
  for (uint32 i = 0; i < info->m_NumSamples; i++)
  {
    // check if end of samples reached
    // NOTE: we do check while applying frequency factor. It seems input data is in sample frequency rather than output one ???
    if ((currentSamplePosition + i) * frequencyResampleFactor >= info->m_OrigNumSamples)
    {
      // end of sound reached
      info->m_EndSample = S3E_TRUE;

      //u64 dur = s3eTimerGetMs() - sound->play;
      //sprintf(buffer, "AudioCallback for channel %d done\n", info->m_Channel);
      //s3eDebugOutputString(buffer);
      break;
    }

    // calculate base sample index (with frequency resampling applied)
    s32 sampleIndex = static_cast<s32>(frequencyResampleFactor * (currentSamplePosition + i));

    int16 currentValue = 0;

    // check if stereo
    if (info->m_Stereo)
    {
      // NOTE: for stereo each sample constists of two int16's one for each channel
      // NOTE: if no mixing is necessary set it to 0. Otherwise, take what is currently in a target
      currentValue = info->m_Mix ? *target : 0;
      *target++ = ClipToS16(info->m_OrigStart[2 * sampleIndex] + currentValue);

      currentValue = info->m_Mix ? *target : 0;
      *target++ = ClipToS16(info->m_OrigStart[2 * sampleIndex + 1] + currentValue);
    }
    else
    {
      // NOTE: if no mixing is necessary set it to 0. Otherwise, take what is currently in a target
      currentValue = info->m_Mix ? *target : 0;
      *target++ = ClipToS16(info->m_OrigStart[sampleIndex] + currentValue);
    }

    samplesPlayed++;
  }

  // update number of samples played
  // NOTE: all generated samples here will be played immediatly after this function exits
  sound->setSamplesPlayed(currentSamplePosition + samplesPlayed);

  return samplesPlayed;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
