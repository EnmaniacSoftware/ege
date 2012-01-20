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
AudioManager::AudioManager(Application* app) : Object(app),
                                               m_enabled(true)
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
    // update private implementation
    p_func()->update(time);

    // go thru all sounds
    // NOTE: no const iterator due to GCC compiler complaints
    for (SoundList::iterator it = m_sounds.begin(); it != m_sounds.end();)
    {
      const PSound& sound = *it;

      // update sound
      sound->update(time);

      // check if sound is not being played anymore
      if (!p_func()->isPlaying(sound))
      {
        EGE_PRINT("%s removed.", sound->name().toAscii());

        // remove it
        m_sounds.erase(it++);
      }
      else
      {
        ++it;
      }
    }
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
    // check if disabled
    if (!isEnabled())
    {
      // done
      return EGE_SUCCESS;
    }

    // find sound resource
    PResourceSound soundResource = app()->resourceManager()->soundResource(soundName);
    if (soundResource)
    {
      // create sound instance
      PSound sound = soundResource->createInstance();

      // set repeat count
      sound->setRepeatCount(repeatCount);

      // play
      return play(sound);
    }

    EGE_LOG("AudioManager::play - %s could not find!", soundName.toAscii());
    return EGE_ERROR_NOT_FOUND;
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Plays given sound.
 * @param sound Sound to play.
 * @return  Returns EGE_SUCCESS if sound is sucessfully started or EGE_ERROR if sound could not be started.
 */
EGEResult AudioManager::play(const PSound& sound)
{
  EGEResult result = EGE_ERROR;

  if (isValid() && sound && sound->isValid())
  {
    // check if disabled
    if (!isEnabled())
    {
      // done
      return EGE_SUCCESS;
    }

    EGE_PRINT("%s", sound->name().toAscii());

    // play
    if (EGE_SUCCESS == (result = p_func()->play(sound)))
    {
      // add to pool
      m_sounds.push_back(sound);
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if sound of a given name is being played. */
bool AudioManager::isPlaying(const String& soundName) const
{
  if (isValid())
  {
    // look for the sound in the active pool
    for (SoundList::const_iterator it = m_sounds.begin(); it != m_sounds.end(); ++it)
    {
      const PSound& sound = *it;
      if (sound->name() == soundName)
      {
        return isPlaying(sound);
      }
    }
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if given sound is being played. */
bool AudioManager::isPlaying(const PSound& sound) const
{
  if (isValid())
  {
    return p_func()->isPlaying(sound);
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Stops playback of all sounds with a given name. */
EGEResult AudioManager::stop(const String& soundName)
{
  if (isValid())
  {
    // look for the sound in the active pool
    for (SoundList::const_iterator it = m_sounds.begin(); it != m_sounds.end(); ++it)
    {
      const PSound& sound = *it;
      if (sound->name() == soundName)
      {
        // stop
        if (EGE_SUCCESS != stop(sound))
        {
          // error!
          return EGE_ERROR;
        }
      }
    }

    return EGE_SUCCESS;
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Stops playback of the given sound. */
EGEResult AudioManager::stop(const PSound& sound)
{
  if (isValid())
  {
    return p_func()->stop(sound);
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns list of sounds being played with the given name. */
List<PSound> AudioManager::sounds(const String& soundName) const
{
  List<PSound> list;

  // go thru all active sounds
  for (SoundList::const_iterator it = m_sounds.begin(); it != m_sounds.end(); ++it)
  {
    const PSound& sound = *it;
    if (sound->name() == soundName)
    {
      // append
      list << sound;
    }
  }

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Enables/disables manager. 
 * @note Disabling manager stops all currently played sounds and prevents any sound from being played. Enabling manager will allow sound to be played.
 *       However, it will not resume playback of any sounds stopped during last disabling request.
 */
void AudioManager::setEnable(bool set)
{
  if (set != m_enabled)
  {
    // check if disabling
    if (!set)
    {
      // stop all active sounds
      // NOTE: make a copy of all sounds first as there is no guarantee on when sounds will be removed from pool
      SoundList soundsToStop(m_sounds);
      for (SoundList::iterator it = soundsToStop.begin(); it != soundsToStop.end(); ++it)
      {
        // stop
        stop(*it);
      }
    }

    // store flag
    m_enabled = set;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
