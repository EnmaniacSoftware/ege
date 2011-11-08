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
                                                            m_name(name),
                                                            m_pitch(1.0f), 
                                                            m_gain(1.0f), 
                                                            m_codec(NULL),
                                                            m_repeatsLeft(0)
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
  m_p = ege_new SoundPrivate(this);
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
/*! Sets pitch value. */
void Sound::setPitch(float32 value)
{
  m_pitch = value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets gain value. */
void Sound::setGain(float32 value)
{
  m_gain = value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets repeat count. */
void Sound::setRepeatCount(s32 count)
{
  m_repeatsLeft = count;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
