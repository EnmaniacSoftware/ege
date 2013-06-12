#include "Airplay/Audio/SoundAirplay.h"
#include "Airplay/Audio/AudioManagerAirplay.h"
#include "EGEMath.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define NO_CHANNEL_ID         -2
#define COMPRESSED_CHANNEL_ID -1
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(SoundAirplay)
EGE_DEFINE_DELETE_OPERATORS(SoundAirplay)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundAirplay::SoundAirplay(AudioManagerAirplay* manager, const String& name, PDataBuffer& data) : Sound(manager->app(), name, data),
                                                                                                  m_manager(manager),
                                                                                                  m_channel(NO_CHANNEL_ID),
                                                                                                  m_pitch(1.0f),
                                                                                                  m_volume(0.0f)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundAirplay::~SoundAirplay()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SoundAirplay::construct()
{
  // call base class
  EGEResult result = Sound::construct();

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SoundAirplay::update(const Time& time)
{
  // call base class
  Sound::update(time);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SoundAirplay::setPitch(float32 value)
{
  if (value != m_pitch)
  {
    m_pitch = value;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 SoundAirplay::pitch() const
{
  return m_pitch;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SoundAirplay::setVolume(float32 volume)
{
  float32 oldVolume = m_volume;

  // clamp to valid range
  m_volume = Math::Bound(volume, 0.0f, 1.0f);

  // notify
  notifyVolumeChanged(oldVolume);

  // check if compressed sound
  if (COMPRESSED_CHANNEL_ID == m_channel)
  {
    s3eAudioSetInt(S3E_AUDIO_VOLUME, static_cast<int32>(S3E_AUDIO_MAX_VOLUME * volume));
  }
  // check if uncompressed sound
  else if (0 <= m_channel)
  {
    s3eSoundChannelSetInt(m_channel, S3E_CHANNEL_VOLUME, static_cast<int32>(S3E_SOUND_MAX_VOLUME * volume));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 SoundAirplay::volume() const
{
  return m_volume;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SoundAirplay::play(s32 repeatCount)
{
  EGEResult result = EGE_ERROR_ALREADY_EXISTS;

  // check if manager is enabled
  if (m_manager->isEnabled())
  {
    // check if not playing
    if ( ! isPlaying())
    {
      AudioCodec* codec = d_func()->codec();

      const PObject& stream = codec->stream();

      result = EGE_SUCCESS;

      // check if MP3 (or other supported compressed audio)
      if (EGE_OBJECT_UID_AUDIO_CODEC_MP3 == codec->uid())
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
              result = EGE_ERROR;
            }
            break;

          default:

            result = EGE_ERROR_NOT_SUPPORTED;
            break;
        }

        if (EGE_SUCCESS == result)
        {
          // store channel
          m_channel = COMPRESSED_CHANNEL_ID;

          // connect for sound volume changes
          ege_connect(d_func(), volumeChanged, this, SoundPrivate::onSoundVolumeChanged);
          d_func()->setVolume(d_func()->volume());
        }
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
          s3eSoundChannelSetInt(m_channel, S3E_CHANNEL_PITCH, static_cast<int32>(s3eSoundChannelGetInt(m_channel, S3E_CHANNEL_PITCH) * pitch()));

          // play sound
          if (S3E_RESULT_ERROR == s3eSoundChannelPlay(m_channel, reinterpret_cast<int16*>(dataBuffer->data(dataBuffer->readOffset())),
                                                      reinterpret_cast<AudioCodecWav*>(codec)->remainingSamplesCount(), repeatCount + 1, 0))
          {
            // error!
            result = EGE_ERROR;
          }

          // connect for sound volume changes
          ege_connect(d_func(), volumeChanged, this, SoundPrivate::onSoundVolumeChanged);
          d_func()->setVolume(d_func()->volume());
        }
        else
        {
          // error!
          result = EGE_ERROR;
        }
      }

      // check if success
      if (EGE_SUCCESS == result)
      {
        // set state
        setState(StatePlaying);
      }
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
bool SoundAirplay::isPlaying() const
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
bool SoundAirplay::isPaused() const
{
  // check if compressed sound
  if (COMPRESSED_CHANNEL_ID == m_channel)
  {
    return (S3E_AUDIO_PAUSED == s3eAudioGetInt(S3E_AUDIO_STATUS));
  }
  // check if uncompressed sound
  else if (0 <= m_channel)
  {
    return (1 == s3eSoundChannelGetInt(m_channel, S3E_CHANNEL_PAUSED));
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SoundAirplay::stop()
{
  // check if manager is enabled
  if (m_manager->isEnabled())
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
      s3eSoundChannelStop(m_channel);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SoundAirplay::pause()
{
  // check if manager is enabled
  if (m_manager->isEnabled())
  {
    // check if compressed sound
    if (COMPRESSED_CHANNEL_ID == m_channel)
    {
      s3eAudioPause();
    }
    // check if uncompressed sound
    else if (0 <= m_channel)
    {
      s3eSoundChannelPause(m_channel);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SoundAirplay::isStopped() const
{
  // check if compressed sound
  if (COMPRESSED_CHANNEL_ID == m_channel)
  {
    return (S3E_AUDIO_STOPPED == s3eAudioGetInt(S3E_AUDIO_STATUS));
  }
  // check if uncompressed sound
  else if (0 <= m_channel)
  {
    return (0 == s3eSoundChannelGetInt(m_channel, S3E_CHANNEL_STATUS));
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
