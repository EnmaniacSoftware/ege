#include "Core/Application/Application.h"
#include "Core/Audio/AudioManager.h"
#include "Core/Audio/AudioThread.h"
#include "Core/Event/Event.h"
#include "Core/Event/EventIDs.h"
#include "Core/Event/EventManager.h"
#include <EGEResources.h>

#ifdef EGE_PLATFORM_WIN32
  #ifdef EGE_AUDIO_OPENAL
    #include "Core/Audio/OpenAL/AudioManagerOpenAL_p.h"
  #endif // EGE_AUDIO_OPENAL
#elif EGE_PLATFORM_AIRPLAY
  #include "Airplay/Audio/AudioManagerAirplay_p.h"
  #include "Airplay/Audio/AudioManagerSoftwareAirplay_p.h"
#endif // EGE_PLATFORM_WIN32

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(AudioManager)
EGE_DEFINE_DELETE_OPERATORS(AudioManager)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManager::AudioManager(Application* app) : Object(app),
                                               m_p(NULL),
                                               m_enabled(true),
                                               m_state(STATE_INITIALIZING)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManager::~AudioManager()
{
  m_thread->stop();

  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates object. */
EGEResult AudioManager::construct()
{
  m_p = ege_new AudioManagerPrivate(this);
  if (NULL == m_p)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // create thread
  m_thread = ege_new AudioThread(app());
  if (NULL == m_thread)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // create access mutex
  m_mutex = ege_new Mutex(app());
  if (NULL == m_mutex)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // start thread
  if (!m_thread->start())
  {
    // error!
    return EGE_ERROR;
  }

  // subscribe for event notifications
  if (!app()->eventManager()->addListener(this))
  {
    // error!
    return EGE_ERROR;
  }

  // set state
  m_state = STATE_READY;
 
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates manager. The audio thread guards the entrance. */
void AudioManager::update(const Time& time)
{
  if (STATE_CLOSING == m_state)
  {
    if (m_thread->isFinished() || !m_thread->isRunning())
    {
      // set state
      m_state = STATE_CLOSED;
      return;
    }
  }

  // update private implementation
  p_func()->update(time);

  // go thru all sounds
  // NOTE: no const iterator due to GCC compiler complaints
  for (SoundList::iterator it = m_sounds.begin(); it != m_sounds.end();)
  {
    const PSound& sound = *it;

    //egeDebug()  << "Updating sound" << sound->name();

    // check if sound is stopped
    if (p_func()->isStopped(sound))
    {
      m_sounds.erase(it++);
      continue;
    }

    // update sound
    sound->update(time);
    ++it;
  }

  // start pending playbacks
  for (SoundList::iterator it = m_soundsToPlay.begin(); it != m_soundsToPlay.end(); ++it)
  {
    const PSound& sound = *it;

    // play
    if (EGE_SUCCESS == p_func()->play(sound))
    {
      // connect
      ege_connect(sound, stopped, this, AudioManager::onStopped);

      // add to pool
      m_sounds.push_back(sound);
    }
  }

  m_soundsToPlay.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Plays sound with given name. 
 * @param soundName   Name of the sound to play.
 * @param repeatCount Number of times sound should be repeated.
 * @param resourceGroup Resource group containing the sound.
 * @note  When repeatCount is set to zero the sound is going to be played exactly once. For negative values sound will be played forever.
 * @return  Returns EGE_SUCCESS if sound is sucessfully started, EGE_ERROR_NOT_FOUND if sound could not be found or EGE_ERROR if sound could not be started.
 */
EGEResult AudioManager::play(const String& soundName, s32 repeatCount, const String& resourceGroup)
{
  // check if disabled
  if (!isEnabled())
  {
    // done
    return EGE_SUCCESS;
  }

  // find sound resource
  PResourceSound soundResource = app()->resourceManager()->soundResource(soundName, resourceGroup);
  if (NULL != soundResource)
  {
    // create sound instance
    PSound sound = soundResource->createInstance();
    if (NULL == sound)
    {
      // error!
      return EGE_ERROR;
    }

    // set repeat count
    sound->setRepeatCount(repeatCount);

    // play
    return play(sound);
  }

  egeWarning() << "Could not find sound:" << soundName;
  return EGE_ERROR_NOT_FOUND;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Plays given sound.
 * @param sound Sound to play.
 * @return  Returns EGE_SUCCESS if sound is sucessfully started or EGE_ERROR if sound could not be started.
 */
EGEResult AudioManager::play(const PSound& sound)
{
  EGEResult result = EGE_ERROR;
  
  if (NULL != sound)
  {
    // check if disabled
    if (!isEnabled())
    {
      // done
      return EGE_SUCCESS;
    }

    // add to pool for later playback
    MutexLocker locker(m_mutex);
    m_soundsToPlay.push_back(sound);
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if sound of a given name is being played. */
bool AudioManager::isPlaying(const String& soundName) const
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

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if given sound is being played. */
bool AudioManager::isPlaying(const PSound& sound) const
{
  if (sound)
  {
    return p_func()->isPlaying(sound);
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Stops playback of all sounds with a given name. */
void AudioManager::stop(const String& soundName)
{
  // look for the sound in the active pool
  for (SoundList::const_iterator it = m_sounds.begin(); it != m_sounds.end(); ++it)
  {
    const PSound& sound = *it;
    if (sound->name() == soundName)
    {
      // stop
      stop(sound);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Stops playback of the given sound. */
void AudioManager::stop(PSound sound)
{
  if (NULL != sound)
  {
    // check if disabled
    if (!isEnabled())
    {
      // done
      return;
    }

    // add to pool
//    MutexLocker locker(m_mutex);
    p_func()->stop(sound);
  }
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
/*! Pauses sound with given name. 
 *  @param soundName   Name of the sound to pause.
 *  @return  Returns EGE_SUCCESS if sound is sucessfully paused, EGE_ERROR_NOT_FOUND if sound could not be found or EGE_ERROR if sound could not be paused.
 */
EGEResult AudioManager::pause(const String& soundName)
{
  // look for the sound in the active pool
  for (SoundList::const_iterator it = m_sounds.begin(); it != m_sounds.end(); ++it)
  {
    const PSound& sound = *it;
    if (sound->name() == soundName)
    {
      // pause
      if (EGE_SUCCESS != pause(sound))
      {
        // error!
        return EGE_ERROR;
      }
    }
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Pauses given sound.
 *  @param sound Sound to pause.
 *  @return  Returns EGE_SUCCESS if sound is sucessfully paused or EGE_ERROR if sound could not be paused.
 */
EGEResult AudioManager::pause(const PSound& sound)
{
  EGEResult result = EGE_ERROR;

  if (sound)
  {
    // check if disabled
    if (!isEnabled())
    {
      // done
      return EGE_SUCCESS;
    }

    // pause
    return p_func()->pause(sound);
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if sound of a given name is paused. */
bool AudioManager::isPaused(const String& soundName) const
{
  // look for the sound in the active pool
  for (SoundList::const_iterator it = m_sounds.begin(); it != m_sounds.end(); ++it)
  {
    const PSound& sound = *it;
    if (sound->name() == soundName)
    {
      return isPaused(sound);
    }
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if given sound is paused. */
bool AudioManager::isPaused(const PSound& sound) const
{
  return p_func()->isPaused(sound);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! EventListener override. Event reciever. */
void AudioManager::onEventRecieved(PEvent event)
{
  switch (event->id())
  {
    case EGE_EVENT_ID_CORE_QUIT_REQUEST:

      if ((STATE_CLOSING != m_state) && (STATE_CLOSED != m_state))
      {
        // do shouting down
        shutDown();
      }
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Shuts down. */
void AudioManager::shutDown()
{
  // change state
  m_state = STATE_CLOSING;

  // request work thread stop
  m_thread->stop(0);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when given sound stopped playback (due to finish or otherwise). */
void AudioManager::onStopped(PSound sound)
{
  ege_disconnect(sound, stopped, this, AudioManager::onStopped);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END