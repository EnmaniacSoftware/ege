#ifndef EGE_AIRPLAY_AUDIO_SOFTWARE
#include "Core/Application/Application.h"
#include "Airplay/Audio/AudioManagerAirplay_p.h"
#include "Airplay/Audio/SoundAirplay_p.h"
#include "Core/Audio/AudioCodecWav.h"
#include <s3eSound.h>
#include <s3eAudio.h>
#include <s3e.h>
#include <EGEAudio.h>
#include <EGEDebug.h>
#include <EGEMath.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(AudioManagerPrivate)
EGE_DEFINE_DELETE_OPERATORS(AudioManagerPrivate)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerPrivate::AudioManagerPrivate(AudioManager* base) : m_d(base)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerPrivate::~AudioManagerPrivate()
{
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
  // check if compressed audio was being played
  if (m_compessedAudio && m_compessedAudio->isValid())
  {
    // check if finished playing
    if (S3E_FALSE == s3eAudioIsPlaying())
    {
      // ok, clean up
      m_compessedAudio = NULL;
    }
  }

  // go thru all uncompressed sounds
  for (SoundsList::iterator it = m_uncompressedAudio.begin(); it != m_uncompressedAudio.end();)
  {
    PSound& sound = *it;

    // check if finished playing
    if ((0 == s3eSoundChannelGetInt(sound->p_func()->channel(), S3E_CHANNEL_STATUS)) && 
        (0 == s3eSoundChannelGetInt(sound->p_func()->channel(), S3E_CHANNEL_PAUSED)))
    {
      // ok, clean up
      m_uncompressedAudio.erase(it++);
      continue;
    }

    ++it;
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
  const PObject& stream = sound->codec()->stream();

  // check if MP3 (or other supported compressed audio)
  if (EGE_OBJECT_UID_AUDIO_CODEC_MP3 == sound->codec()->uid())
  {
    // process according to stream type
    switch (stream->uid())
    {
      case EGE_OBJECT_UID_DATA_BUFFER:

        // play sound
        if (S3E_RESULT_ERROR == s3eAudioPlayFromBuffer(ege_cast<DataBuffer*>(stream)->data(), static_cast<uint32>(ege_cast<DataBuffer*>(stream)->size()), 
                                                       repeatCount + 1))
        {
          // error!
          return EGE_ERROR;
        }

        // store it
        m_compessedAudio = sound;
        break;

      default:

        EGE_ASSERT("Not supported!");
        return EGE_ERROR_NOT_SUPPORTED;
    }

    return EGE_SUCCESS;
  }
  // check if WAV file
  else if (EGE_OBJECT_UID_AUDIO_CODEC_WAV == sound->codec()->uid())
  {
    // get free channel
    int channel = s3eSoundGetFreeChannel();
    if (-1 != channel)
    {
      DataBuffer* dataBuffer = ege_cast<DataBuffer*>(stream);

      // setup playback data
      // NOTE: first set channel frequency and then pitch as setting channel frequency modifies pitch
      s3eSoundChannelSetInt(channel, S3E_CHANNEL_RATE, sound->codec()->frequency());
      s3eSoundChannelSetInt(channel, S3E_CHANNEL_PITCH, static_cast<int32>(s3eSoundChannelGetInt(channel, S3E_CHANNEL_PITCH) * sound->pitch()));

      // play sound
      if (S3E_RESULT_ERROR == s3eSoundChannelPlay(channel, reinterpret_cast<int16*>(dataBuffer->data(dataBuffer->readOffset())),
                                                  reinterpret_cast<AudioCodecWav*>(sound->codec())->remainingSamplesCount(), repeatCount + 1, 0))
      {
        // error!
        return EGE_ERROR;
      }

      // setup data for later use
      sound->p_func()->setChannel(channel);

      // store it
      m_uncompressedAudio.push_back(sound);
      return EGE_SUCCESS;
    }

    return EGE_ERROR;
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Stops playback of the sound with a given name. */
void AudioManagerPrivate::stop(const String& soundName)
{
  // check if compressed audio
  if (m_compessedAudio && m_compessedAudio->isValid() && m_compessedAudio->name() == soundName)
  {
    // stop it
    s3eAudioStop();
  }
  else
  {
    // go thru all uncompressed sounds
    for (SoundsList::iterator it = m_uncompressedAudio.begin(); it != m_uncompressedAudio.end(); ++it)
    {
      PSound& sound = *it;
      if (sound->name() == soundName)
      {
        // mark it to stop
        s3eSoundChannelStop(sound->p_func()->channel());
        return;
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if sound of a given name is being played. */
bool AudioManagerPrivate::isPlaying(const String& soundName) const
{
  // check if compressed audio
  if (m_compessedAudio && m_compessedAudio->isValid() && m_compessedAudio->name() == soundName)
  {
    // found
    return true;
  }
  else
  {
    // go thru all uncompressed sounds
    for (SoundsList::const_iterator it = m_uncompressedAudio.begin(); it != m_uncompressedAudio.end(); ++it)
    {
      const PSound& sound = *it;
      if (sound->name() == soundName)
      {
        // found
        return true;
      }
    }
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // !EGE_AIRPLAY_AUDIO_SOFTWARE