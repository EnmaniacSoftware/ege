#include "Core/Application/Application.h"
#include "Core/Audio/AudioManager.h"
#include <EGEResources.h>

#ifdef EGE_PLATFORM_WIN32
  #ifdef EGE_AUDIO_OPENAL
    #include "Core/Audio/OpenAL/AudioManagerOpenAL_p.h"
  #endif // EGE_AUDIO_OPENAL
#elif EGE_PLATFORM_AIRPLAY
  #include "Airplay/Audio/AudioManagerAirplay_p.h"
#endif // EGE_PLATFORM_WIN32

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(AudioManager)
EGE_DEFINE_DELETE_OPERATORS(AudioManager)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManager::AudioManager(Application* app) : Object(app)
{
  m_p = ege_new AudioManagerPrivate(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManager::~AudioManager()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool AudioManager::isValid() const
{
  return (NULL != m_p) && m_p->isValid();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates manager. */
void AudioManager::update(const Time& time)
{
  if (isValid())
  {
    p_func()->update(time);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Plays sound with given name. */
EGEResult AudioManager::play(const String& soundName)
{
  if (isValid())
  {
    PResourceSound soundResource = app()->resourceManager()->soundResource(soundName);
    if (soundResource)
    {
      PSound sound = soundResource->createInstance();
      if (sound)
      {
        return p_func()->play(sound);
      }
    }

    return EGE_ERROR_NOT_FOUND;
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Plays given sound. */
EGEResult AudioManager::play(const PSound& sound)
{
  if (isValid())
  {
    return p_func()->play(sound);
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
