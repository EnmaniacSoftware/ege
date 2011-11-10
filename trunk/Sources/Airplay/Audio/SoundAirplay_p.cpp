#ifndef EGE_AIRPLAY_AUDIO_SOFTWARE
#include "Core/Application/Application.h"
#include "Airplay/Audio/SoundAirplay_p.h"
#include "Core/Audio/AudioCodecWav.h"
#include <s3eSound.h>
#include <s3e.h>
#include <EGEDebug.h>
#include <EGEAudio.h>
#include <EGEDevice.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(SoundPrivate)
EGE_DEFINE_DELETE_OPERATORS(SoundPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define NO_CHANNEL_ID         -2
#define COMPRESSED_CHANNEL_ID -1
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundPrivate::SoundPrivate(Sound* base) : m_d(base),
                                          m_channel(NO_CHANNEL_ID)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundPrivate::~SoundPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool SoundPrivate::isValid() const
{
  return true;;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates object. */
void SoundPrivate::update(const Time& time)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Starts playback. */
EGEResult SoundPrivate::play()
{
  AudioCodec* codec = d_func()->codec();

  const PObject& stream = codec->stream();

  // check if MP3 (or other supported compressed audio)
  if (EGE_OBJECT_UID_AUDIO_CODEC_MP3 == codec->uid())
  {
    // process according to stream type
    switch (stream->uid())
    {
      case EGE_OBJECT_UID_DATA_BUFFER:

        // play sound
        if (S3E_RESULT_ERROR == s3eAudioPlayFromBuffer(ege_cast<DataBuffer*>(stream)->data(), static_cast<uint32>(ege_cast<DataBuffer*>(stream)->size()), 
                                                       d_func()->repeatsLeft() + 1))
        {
          // error!
          return EGE_ERROR;
        }
        break;

      default:

        return EGE_ERROR_NOT_SUPPORTED;
    }

    // connect for sound volume changes
    ege_connect(d_func(), volumeChanged, this, SoundPrivate::onSoundVolumeChanged);
    d_func()->setVolume(d_func()->volume());

    // store channel
    m_channel = COMPRESSED_CHANNEL_ID;
    return EGE_SUCCESS;
  }
  // check if WAV file
  else if (EGE_OBJECT_UID_AUDIO_CODEC_WAV == codec->uid())
  {
    // get free channel
    m_channel = s3eSoundGetFreeChannel();
    if (-1 != m_channel)
    {
      DataBuffer* dataBuffer = ege_cast<DataBuffer*>(stream);

      // setup playback data
      // NOTE: first set channel frequency and then pitch as setting channel frequency modifies pitch
      s3eSoundChannelSetInt(m_channel, S3E_CHANNEL_RATE, codec->frequency());
      s3eSoundChannelSetInt(m_channel, S3E_CHANNEL_PITCH, static_cast<int32>(s3eSoundChannelGetInt(m_channel, S3E_CHANNEL_PITCH) * d_func()->pitch()));

      // play sound
      if (S3E_RESULT_ERROR == s3eSoundChannelPlay(m_channel, reinterpret_cast<int16*>(dataBuffer->data(dataBuffer->readOffset())),
                                                  reinterpret_cast<AudioCodecWav*>(codec)->remainingSamplesCount(), d_func()->repeatsLeft() + 1, 0))
      {
        // error!
        return EGE_ERROR;
      }

      // connect for sound volume changes
      ege_connect(d_func(), volumeChanged, this, SoundPrivate::onSoundVolumeChanged);
      d_func()->setVolume(d_func()->volume());
      return EGE_SUCCESS;
    }

    return EGE_ERROR;
  }

  EGE_PRINT("SoundPrivate::play - %s", d_func()->name().toAscii());

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Stops playback. */
EGEResult SoundPrivate::stop()
{
  // check if compressed sound
  if (COMPRESSED_CHANNEL_ID == m_channel)
  {
    // request stop
    // NOTE: due to nature of Airplay these are asynchronous requests rather than immediate state changes
    s3eAudioStop();
  }
  // check if uncompressed sound
  else if (0 <= m_channel)
  {
    // request stop
    // NOTE: due to nature of Airplay these are asynchronous requests rather than immediate state changes
    if (S3E_RESULT_SUCCESS != s3eSoundChannelStop(m_channel))
    {
      // error!
      return EGE_ERROR;
    }
  }

  EGE_PRINT("SoundPrivate::stop - %s", d_func()->name().toAscii());

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if sound is being played. */
bool SoundPrivate::isPlaying() const
{
  // check if compressed sound
  if (COMPRESSED_CHANNEL_ID == m_channel)
  {
    return (S3E_TRUE == s3eAudioIsPlaying());
  }
  // check if uncompressed sound
  else if (0 <= m_channel)
  {
    return (1 == s3eSoundChannelGetInt(m_channel, S3E_CHANNEL_STATUS));
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called on sound volume change. */
void SoundPrivate::onSoundVolumeChanged(const Sound* sound, float32 oldVolume)
{
  EGE_ASSERT(sound->p_func() == this);

  EGE_PRINT("SoundPrivate::onSoundVolumeChanged: %s %.2f -> %.2f", sound->name().toAscii(), oldVolume, sound->volume());

  // check if compressed sound
  if (COMPRESSED_CHANNEL_ID == m_channel)
  {
    s3eAudioSetInt(S3E_AUDIO_VOLUME, static_cast<int32>(S3E_AUDIO_MAX_VOLUME * sound->volume()));
  }
  // check if uncompressed sound
  else if (0 <= m_channel)
  {
    s3eSoundChannelSetInt(m_channel, S3E_CHANNEL_VOLUME, static_cast<int32>(S3E_SOUND_MAX_VOLUME * sound->volume()));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif // !EGE_AIRPLAY_AUDIO_SOFTWARE