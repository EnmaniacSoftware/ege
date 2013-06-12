#include "Core/Application/Application.h"
#include "Airplay/Audio/AudioManagerAirplay.h"
#include "Airplay/Audio/SoundAirplay.h"
#include "Core/Audio/Sound.h"
#include "EGEEvent.h"
#include "EGEDebug.h"
#include <s3eSound.h>
#include <s3eAudio.h>
#include <s3e.h>
#include <s3eIOSAudioRoute.h>
#include <s3eIOSBackgroundAudio.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static s16 l_emptySoundSampleData[1];
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(AudioManagerAirplay)
EGE_DEFINE_DELETE_OPERATORS(AudioManagerAirplay)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerAirplay::AudioManagerAirplay(Application* app) : Object(app),
                                                             IAudioManagerBase(),
                                                             IAudioManager(),
                                                             m_state(IAudioManager::StateNone),
                                                             m_enabled(true)


{
  // NOTE: we set an empty sound to be played here so Airplay sound system gets initialized
  l_emptySoundSampleData[0] = 0;

  // get free channel
  s32 channel = s3eSoundGetFreeChannel();
  if (-1 != channel)
  {
    s3eSoundChannelSetInt(channel, S3E_CHANNEL_RATE, 22050);
    s3eSoundChannelPlay(channel, l_emptySoundSampleData, 1, 1, 0);
  }

  // check if audio routing is available
  if (s3eIOSAudioRouteAvailable())
  {
    s3eIOSAudioRouteSetCategory(S3E_IOSAUDIOROUTE_CATEGORY_AMBIENT);
    s3eIOSBackgroundAudioSetMix(true);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerAirplay::~AudioManagerAirplay()
{
  app()->eventManager()->removeListener(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult AudioManagerAirplay::construct()
{
  // subscribe for event notifications
  if ( ! app()->eventManager()->addListener(this))
  {
    // error!
    return EGE_ERROR;
  }
  
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerAirplay::update(const Time& time)
{
  // go thru all sounds
  for (SoundList::iterator it = m_sounds.begin(); it != m_sounds.end();)
  {
    SoundAirplay* sound = ege_cast<SoundAirplay*>(*it);

    // update sound
    sound->update(time);

    // check if sound is stopped
    if (sound->isStopped())
    {
      // remove from list
      it = m_sounds.erase(it);
    }
    else
    {
      ++it;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerAirplay::setEnable(bool set)
{
  if (m_enabled != set)
  {
    // check if disabling
    if ( ! set)
    {
      // queue all currently played sounds for stop
      for (SoundList::iterator it = m_sounds.begin(); it != m_sounds.end(); ++it)
      {
        PSound sound = *it;

        // stop
        sound->stop();
      }
      m_sounds.clear();
    }

    // set flag
    m_enabled = set;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AudioManagerAirplay::isEnabled() const
{
  return m_enabled;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PSound AudioManagerAirplay::createSound(const String& name, PDataBuffer& data) const
{
  SoundAirplay* object = ege_new SoundAirplay(const_cast<AudioManagerAirplay*>(this), name, data);
  if ((NULL == object) || (EGE_SUCCESS != object->construct()))
  {
    // error!
    object = NULL;
  }

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IAudioManager::EState AudioManagerAirplay::state() const
{
  return m_state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerAirplay::onEventRecieved(PEvent event)
{
  switch (event->id())
  {
    case EGE_EVENT_ID_CORE_QUIT_REQUEST:

      if ((StateClosing != state()) && (StateClosed != state()))
      {
        // set state
        m_state = IAudioManager::StateClosed;
      }
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult AudioManagerAirplay::requestPlay(PSound sound)
{
  // just add sound into the pool for updating
  // NOTE: This method is only called from Sound itself and all logic is done there
  m_sounds.push_back(sound);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerAirplay::requestStop(PSound sound)
{
  EGE_UNUSED(sound);
  
  // no need to do anything
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerAirplay::requestPause(PSound sound)
{
  EGE_UNUSED(sound);

  // no need to do anything
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
