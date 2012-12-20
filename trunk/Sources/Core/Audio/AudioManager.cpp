#include "Core/Application/Application.h"
#include "Core/Audio/AudioManager.h"
#include "Core/Event/Event.h"
#include "Core/Event/EventIDs.h"
#include "Core/Event/EventManager.h"
#include <EGEResources.h>

#ifdef EGE_PLATFORM_WIN32
  #if EGE_AUDIO_OPENAL
    #include "Core/Audio/OpenAL/AudioManagerOpenAL_p.h"
  #elif EGE_AUDIO_NULL
    #include "Core/Audio/Null/AudioManagerNull_p.h"    
  #endif // EGE_AUDIO_OPENAL
#elif EGE_PLATFORM_AIRPLAY
  #if EGE_AUDIO_AIRPLAY
    #include "Airplay/Audio/AudioManagerAirplay_p.h"
    #include "Airplay/Audio/AudioManagerSoftwareAirplay_p.h"
  #elif EGE_AUDIO_NULL
    #include "Core/Audio/Null/AudioManagerNull_p.h"    
  #endif // EGE_AUDIO_AIRPLAY
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
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult AudioManager::construct()
{
  EGEResult result = EGE_SUCCESS;

  // allocate private
  m_p = ege_new AudioManagerPrivate(this);
  if (NULL == m_p)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // construct private
  if (EGE_SUCCESS != (result = m_p->construct()))
  {
    // error!
    return result;
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
void AudioManager::update(const Time& time)
{
  if (STATE_CLOSED == m_state)
  {
    // do nothing
    return;
  }
  
  // update private implementation
  p_func()->update(time);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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

    result = p_func()->play(sound);
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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

  // propagate to implementation
  return p_func()->isPlaying(soundName);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AudioManager::isPlaying(const PSound& sound) const
{
  if (sound)
  {
    return p_func()->isPlaying(sound);
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
bool AudioManager::isPaused(const PSound& sound) const
{
  return p_func()->isPaused(sound);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
void AudioManager::shutDown()
{
  // change state
  m_state = STATE_CLOSING;

  // request work thread stop
  p_func()->shutDown();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManager::onStopped(PSound sound)
{
  ege_disconnect(sound, stopped, this, AudioManager::onStopped);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END