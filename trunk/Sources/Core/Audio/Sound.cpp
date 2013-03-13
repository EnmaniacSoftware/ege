#include "Core/Audio/Sound.h"
#include "Core/Audio/AudioCodecWav.h"
#include "Core/Audio/AudioCodecOgg.h"
#include "Core/Audio/AudioCodecMp3.h"
#include "Core/Audio/AudioUtils.h"
#include <EGEMath.h>
#include <EGEDebug.h>

#ifdef EGE_PLATFORM_WIN32
  #if EGE_AUDIO_OPENAL
    #include "Core/Audio/OpenAL/SoundOpenAL_p.h"
  #elif EGE_AUDIO_NULL
    #include "Core/Audio/Null/SoundNull_p.h"    
  #endif // EGE_AUDIO_OPENAL
#elif EGE_PLATFORM_AIRPLAY
  #if EGE_AUDIO_AIRPLAY
    #include "Airplay/Audio/SoundAirplay_p.h"
    #include "Airplay/Audio/SoundSoftwareAirplay_p.h"
  #elif EGE_AUDIO_NULL
    #include "Core/Audio/Null/SoundNull_p.h"    
  #endif // EGE_AUDIO_AIRPLAY
#elif EGE_PLATFORM_IOS
  #if EGE_AUDIO_OPENAL
    #include "Core/Audio/OpenAL/SoundOpenAL_p.h"
  #elif EGE_AUDIO_NULL
    #include "Core/Audio/Null/SoundNull_p.h"
  #endif // EGE_AUDIO_OPENAL
#endif // EGE_PLATFORM_WIN32

EGE_NAMESPACE_BEGIN

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
                                                            m_data(data)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Sound::~Sound()
{
  EGE_DELETE(m_p);
  EGE_DELETE(m_codec);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult Sound::construct()
{
  // detect stream type
  EGEAudio::StreamType type = AudioUtils::DetectStreamType(m_data);

  switch (type)
  {
    case EGEAudio::ST_WAVE:
        
      m_codec = ege_new AudioCodecWav(m_data);
      break;

    case EGEAudio::ST_OGG:

      m_codec = ege_new AudioCodecOgg(m_data);
      break;
      
    case EGEAudio::ST_MP3:

      m_codec = ege_new AudioCodecMp3(m_data);
      break;

    default:

      EGE_ASSERT("Unsupported audio format!");
      return EGE_ERROR_NOT_SUPPORTED;
  }

  if (NULL == m_codec)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // create and initialize private
  m_p = ege_new SoundPrivate(this);
  if (NULL == m_p)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  return m_p->construct();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::update(const Time& time)
{
  // update effects
  updateSoundEffects(time);

  // update implementation
  p_func()->update(time);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::setPitch(float32 value)
{
  m_pitch = value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::setRepeatCount(s32 count)
{
  m_repeatsLeft = count;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::setVolume(float32 volume)
{
  float32 old = m_volume;

  // clamp to valid range
  m_volume = Math::Bound(volume, 0.0f, 1.0f);

  // notify
  notifyVolumeChanged(old);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Sound::addEffect(PSoundEffect effect)
{
  if (NULL != effect)
  {
    m_effects.push_back(effect);

    // do initial update
    effect->update(0LL, this);
    return true;
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::notifyFinished()
{
  emit finished(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::notifyStopped()
{
  emit stopped(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::notifyVolumeChanged(float32 oldVolume)
{
  emit volumeChanged(this, oldVolume);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundEffectList Sound::effects(u32 uid) const
{
  SoundEffectList list;
  for (SoundEffectList::const_iterator it = m_effects.begin(); it != m_effects.end(); ++it)
  {
    const PSoundEffect& effect = *it;
    if (effect->uid() == uid)
    {
      list << effect;
    }
  }

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::updateSoundEffects(const Time& time)
{
  for (SoundEffectList::iterator it = m_effects.begin(); it != m_effects.end(); )
  {
    PSoundEffect& effect = *it;

    // update
    if (effect->update(time, this))
    {
      // effect is over, remove
      m_effects.erase(it++);
    }
    else
    {
      ++it;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END