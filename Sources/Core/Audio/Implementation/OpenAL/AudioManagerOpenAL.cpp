#include "Core/Audio/Interface/OpenAL/AudioManagerOpenAL.h"
#include "Core/Audio/Implementation/OpenAL/SoundOpenAL.h"
#include "Core/Audio/Implementation/OpenAL/AudioThreadOpenAL.h"
#include "EGEEngine.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KAudioManagerOpenALDebugName = "EGEAudioManagerOpenAL";

const char* KOpenALAudioManagerName = "openal";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(AudioManagerOpenAL)
EGE_DEFINE_DELETE_OPERATORS(AudioManagerOpenAL)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerOpenAL::AudioManagerOpenAL(Engine& engine) 
: m_engine(engine)
, m_device(NULL)
, m_context(NULL)
, m_enabled(true)
{
  m_thread = ege_new AudioThreadOpenAL(this);
  m_mutex  = ege_new Mutex(EGEMutex::Recursive);

  // connect
  ege_connect(m_thread, finished, this, AudioManagerOpenAL::onThreadFinished);

  EGE_MEMSET(m_channels, 0, sizeof (m_channels));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerOpenAL::~AudioManagerOpenAL()
{
  m_thread->stop();

  alcMakeContextCurrent(NULL);
  
  alDeleteSources(CHANNELS_COUNT, m_channels);
  EGE_MEMSET(m_channels, 0, sizeof (m_channels));

  // destroy context
  if (m_context)
  {
    alcDestroyContext(m_context);
    m_context = NULL;
  }

  // destroy audio device
  if (m_device)
  {
    alcCloseDevice(m_device);
    m_device = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EngineModule<IAudioManager>* AudioManagerOpenAL::Create(Engine& engine)
{
  return ege_new AudioManagerOpenAL(engine);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult AudioManagerOpenAL::construct()
{
  // create audio device
  m_device = alcOpenDevice(NULL);
  if (NULL == m_device)
  {
    // error!
    return EGE_ERROR;
  }

  // create context
  m_context = alcCreateContext(m_device, NULL);
  alcMakeContextCurrent(m_context);

  // preallocate channels
  alGenSources(CHANNELS_COUNT, m_channels);
  if (IS_OAL_ERROR())
  {
    egeCritical(KAudioManagerOpenALDebugName) << "[OAL] Could not generate sources.";
    EGE_MEMSET(m_channels, 0, sizeof (m_channels));

    return EGE_ERROR;
  }

  // start thread
  if ( ! m_thread->start())
  {
    // error!
    return EGE_ERROR;
  }

  return EngineModule::construct();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerOpenAL::update(const Time& time)
{
  EGE_UNUSED(time)

  // do nothing, updated from another thread
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult AudioManagerOpenAL::requestPlay(PSound sound)
{
  // check if enabled
  if (m_enabled)
  {
    MutexLocker locker(m_mutex);

    // NOTE: this should only happen when sound is not playing or is paused
    EGE_ASSERT( ! m_sounds.contains(sound) || (m_sounds.contains(sound) && sound->isPaused()));

    // add to pool for later playback
    queueForPlay(sound);
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerOpenAL::requestPause(PSound sound)
{
  if (m_enabled)
  {
    MutexLocker locker(m_mutex);

    EGE_ASSERT(m_sounds.contains(sound));

    // add for pausing
    queueForPause(sound);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerOpenAL::requestStop(PSound sound)
{
  // check if enabled
  if (m_enabled)
  {
    MutexLocker locker(m_mutex);

    EGE_ASSERT(m_sounds.contains(sound) || m_soundsToPause.contains(sound));

    // add to pool for later playback
    queueForStop(sound);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerOpenAL::setEnable(bool set)
{
  if (m_enabled != set)
  {
    // check if disabling
    if ( ! set)
    {
      MutexLocker locker(m_mutex);

      // queue all currently played sounds for stop
      for (SoundList::iterator it = m_sounds.begin(); it != m_sounds.end(); ++it)
      {
        // stop
        queueForStop(*it);
      }
      m_sounds.clear();

      // stop all sounds scheduled for playback
      for (SoundList::iterator it = m_soundsToPlay.begin(); it != m_soundsToPlay.end(); ++it)
      {
        // stop
        queueForStop(*it);
      }
      m_soundsToPlay.clear();
    
      // clear pause list
      // NOTE: it can be cleared as the sounds should co-exists in m_sounds list
      m_soundsToPause.clear();
    }

    // set flag
    m_enabled = set;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AudioManagerOpenAL::isEnabled() const
{
  return m_enabled;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PSound AudioManagerOpenAL::createSound(const String& name, PDataBuffer& data) const
{
  SoundOpenAL* object = ege_new SoundOpenAL(const_cast<AudioManagerOpenAL*>(this), name, data);
  if ((NULL == object) || (EGE_SUCCESS != object->construct()))
  {
    // error!
    object = NULL;
  }

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerOpenAL::onShutdown()
{
  if ((EModuleStateShuttingDown != state()) && (EModuleStateClosed != state()))
  {
    // set state
    setState(EModuleStateShuttingDown);

    if (NULL != m_thread)
    {
      m_thread->stop(0); 
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerOpenAL::threadUpdate(const Time& time)
{
  if (EModuleStateShuttingDown == state())
  {
    if (m_thread->isFinished() || ! m_thread->isRunning())
    {
      // set state
      setState(EModuleStateClosed);
      return;
    }
  }

  MutexLocker locker(m_mutex);

  // process sounds to be paused
  for (SoundList::iterator it = m_soundsToPause.begin(); it != m_soundsToPause.end(); ++it)
  {
    SoundOpenAL* sound = ege_cast<SoundOpenAL*>(*it);

    // pause sound
    sound->doPause();
  }
  m_soundsToPause.clear();

  // go thru all sounds
  for (SoundList::iterator it = m_sounds.begin(); it != m_sounds.end();)
  {
    SoundOpenAL* sound = ege_cast<SoundOpenAL*>(*it);

    // update sound
    sound->update(time);

    // check if sound is stopped
    if (SoundOpenAL::StateStopped == sound->state())
    {
      // remove from list
      it = m_sounds.erase(it);
    }
    else
    {
      ++it;
    }
  }

  // start pending playbacks
  for (SoundList::iterator it = m_soundsToPlay.begin(); it != m_soundsToPlay.end(); ++it)
  {
    SoundOpenAL* sound = ege_cast<SoundOpenAL*>(*it);

    // check if sound is paused
    if (SoundOpenAL::StatePaused == sound->state())
    {
      // resume
      sound->doResume();
    }
    else
    {
      // start playback
      if (sound->doPlay(findAvailableChannel()))
      {
        // add to pool
        m_sounds.push_back(*it);
      }
    }
  }
  m_soundsToPlay.clear();

  // stop playbacks
  for (SoundList::iterator it = m_soundsToStop.begin(); it != m_soundsToStop.end(); ++it)
  {
    SoundOpenAL* sound = ege_cast<SoundOpenAL*>(*it);

    // check if can be stopped
    if (SoundOpenAL::StateStopped != sound->state())
    {
      // NOTE: sounds will be removed next step their internal state is set to StateStopped
      sound->doStop();
    }
  }
  m_soundsToStop.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerOpenAL::onThreadFinished(const PThread& thread)
{
  EGE_UNUSED(thread);

  // set state
  setState(EModuleStateClosed);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerOpenAL::queueForStop(PSound& sound)
{
  // add to list
  m_soundsToStop.push_back(sound);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerOpenAL::queueForPlay(PSound& sound)
{
  // add to list
  m_soundsToPlay.push_back(sound);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerOpenAL::queueForPause(PSound& sound)
{
  // add to list
  m_soundsToPause.push_back(sound);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ALuint AudioManagerOpenAL::findAvailableChannel() const
{
  // go thru all channels
  for (s32 i = 0; i < CHANNELS_COUNT; ++i)
  {
    ALint state;
		alGetSourcei(m_channels[i], AL_SOURCE_STATE, &state);
    if (IS_OAL_ERROR())
    {
      // error!
      egeCritical(KAudioManagerOpenALDebugName) << "[OAL] Could not retrieve channel state:" << m_channels[i] << "at index" << i;
      continue;
    }

		// if this source is not playing/paused then return it
		if ((AL_PLAYING != state) && (AL_PAUSED != state))
    {
      return m_channels[i];
    }
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Engine& AudioManagerOpenAL::engine() const
{
  return m_engine;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 AudioManagerOpenAL::uid() const
{
  return EGE_OBJECT_UID_AUDIO_MANAGER_MODULE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END