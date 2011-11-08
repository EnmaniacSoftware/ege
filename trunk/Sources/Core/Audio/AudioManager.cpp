#include "Core/Application/Application.h"
#include "Core/Audio/AudioManager.h"
#include <EGEResources.h>

#ifdef EGE_PLATFORM_WIN32
  #ifdef EGE_AUDIO_OPENAL
    #include "Core/Audio/OpenAL/AudioManagerOpenAL_p.h"
  #endif // EGE_AUDIO_OPENAL
#elif EGE_PLATFORM_AIRPLAY
  #include "Airplay/Audio/AudioManagerAirplay_p.h"
  #include "Airplay/Audio/AudioManagerSoftwareAirplay_p.h"
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
/*! Plays sound with given name. 
 * @param soundName   Name of the sound to play.
 * @param repeatCount Number of times sound should be repeated.
 * @note  When repeatCount is set to zero the sound is going to be played exactly once. For negative values sound will be played forever.
 * @return  Returns EGE_SUCCESS if sound is sucessfully started, EGE_ERROR_NOT_FOUND if sound could not be found or EGE_ERROR if sound could not be started.
 */
EGEResult AudioManager::play(const String& soundName, s32 repeatCount)
{
  if (isValid())
  {
    PResourceSound soundResource = app()->resourceManager()->soundResource(soundName);
    if (soundResource)
    {
      PSound sound = soundResource->createInstance();
      return play(sound, repeatCount);
    }

    return EGE_ERROR_NOT_FOUND;
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Plays given sound.
 * @param sound       Sound to play.
 * @param repeatCount Number of times sound should be repeated.
 * @return  Returns EGE_SUCCESS if sound is sucessfully started or EGE_ERROR if sound could not be started.
 * @note  When repeatCount is set to zero the sound is going to be played exactly once. For negative values sound will be played forever.
 */
EGEResult AudioManager::play(const PSound& sound, s32 repeatCount)
{
  if (isValid() && sound && sound->isValid())
  {
    // set number of repeats
    sound->setRepeatCount(repeatCount);

    // play
    return p_func()->play(sound, repeatCount);
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if sound of a given name is being played. */
bool AudioManager::isPlaying(const String& soundName) const
{
  if (isValid())
  {
    return p_func()->isPlaying(soundName);
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Stops playback of the sound with a given name. */
void AudioManager::stop(const String& soundName)
{
  if (isValid())
  {
    p_func()->stop(soundName);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
