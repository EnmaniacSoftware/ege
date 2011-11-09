#include "Core/Audio/Sound.h"
#include "Core/Audio/AudioCodecWav.h"
#include "Core/Audio/AudioCodecOgg.h"
#include "Core/Audio/AudioCodecMp3.h"
#include "Core/Audio/AudioUtils.h"
#include <EGEDebug.h>

#ifdef EGE_PLATFORM_WIN32
  #ifdef EGE_AUDIO_OPENAL
    #include "Core/Audio/OpenAL/SoundOpenAL_p.h"
  #endif // EGE_AUDIO_OPENAL
#elif EGE_PLATFORM_AIRPLAY
  #include "Airplay/Audio/SoundAirplay_p.h"
  #include "Airplay/Audio/SoundSoftwareAirplay_p.h"
#endif // EGE_PLATFORM_WIN32

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(Sound)
EGE_DEFINE_DELETE_OPERATORS(Sound)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Sound::Sound(const String& name, const PDataBuffer& data) : Object(NULL), 
                                                            m_p(NULL),
                                                            m_name(name),
                                                            m_pitch(1.0f), 
                                                            m_codec(NULL),
                                                            m_repeatsLeft(0),
                                                            m_volume(1.0f),
                                                            m_state(STATE_IDLE)
{
  // detect stream type
  EGEAudio::StreamType type = AudioUtils::DetectStreamType(data);

  switch (type)
  {
    case EGEAudio::ST_WAVE:
        
      m_codec = ege_new AudioCodecWav(data);
      break;

    case EGEAudio::ST_OGG:

      m_codec = ege_new AudioCodecOgg(data);
      break;
      
    case EGEAudio::ST_MP3:

      m_codec = ege_new AudioCodecMp3(data);
      break;

    default:

      EGE_ASSERT("Unsupported audio format!");
      break;
  }

  // create private
  if (m_codec)
  {
    m_p = ege_new SoundPrivate(this);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Sound::~Sound()
{
  EGE_DELETE(m_p);
  EGE_DELETE(m_codec);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool Sound::isValid() const
{
  return (NULL != m_p) && m_p->isValid() && (NULL != m_codec);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates object. */
void Sound::update(const Time& time)
{
  switch (m_state)
  {
    case STATE_FADING_IN:

      // update fade time
      m_fadeTime += time;
      if (m_fadeTime >= m_fadeDuration)
      {
        m_fadeTime = m_fadeDuration;

        // reset state
        m_state = STATE_IDLE;

        // emit
        emit fadeInComplete(this);
      }

      // set volume
      setVolume(Math::Lerp(0.0f, 1.0f, m_fadeTime.seconds() / m_fadeDuration.seconds()));
      break;

    case STATE_FADING_OUT:

      // update fade time
      m_fadeTime += time;
      if (m_fadeTime >= m_fadeDuration)
      {
        m_fadeTime = m_fadeDuration;

        // reset state
        m_state = STATE_IDLE;

        // emit
        emit fadeOutComplete(this);
      }

      // set volume
      setVolume(Math::Lerp(1.0f, 0.0f, m_fadeTime.seconds() / m_fadeDuration.seconds()));
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets pitch value. */
void Sound::setPitch(float32 value)
{
  m_pitch = value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets repeat count. */
void Sound::setRepeatCount(s32 count)
{
  m_repeatsLeft = count;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets volume. */
void Sound::setVolume(float32 volume)
{
  float32 old = m_volume;

  // clamp to valid range
  m_volume = Math::Bound(volume, 0.0f, 1.0f);

  // emit signal
  emit volumeChanged(this, old);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Starts fading out. */
void Sound::startFadeOut(const Time& duration)
{
  m_fadeDuration = duration;
  m_fadeTime = 0.0f;

  // state state
  m_state = STATE_FADING_OUT;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Starts fading in. */
void Sound::startFadeIn(const Time& duration)
{
  m_fadeDuration = duration;
  m_fadeTime = 0.0f;

  // state state
  m_state = STATE_FADING_IN;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
