#ifdef EGE_AUDIO_OPENAL

#include "Core/Application/Application.h"
#include "Core/Audio/OpenAL/AudioManagerOpenAL_p.h"
#include "Core/Audio/OpenAL/SoundOpenAL_p.h"
#include "Core/Audio/OpenAL/AudioThreadOpenAL.h"
#include "EGEAudio.h"
#include "EGEDebug.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static ALenum l_result;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define IS_AL_ERROR() (AL_NO_ERROR != (l_result = alGetError()))
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(AudioManagerPrivate)
EGE_DEFINE_DELETE_OPERATORS(AudioManagerPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerPrivate::AudioManagerPrivate(AudioManager* base) : m_d(base),
                                                               m_device(NULL),
                                                               m_context(NULL)
{
  EGE_MEMSET(m_channels, 0, sizeof (m_channels));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerPrivate::~AudioManagerPrivate()
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
EGEResult AudioManagerPrivate::construct()
{
  // create thread
  m_thread = ege_new AudioThreadOpenAL(d_func()->app(), this);
  if (NULL == m_thread)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  ege_connect(m_thread, finished, this, AudioManagerPrivate::onThreadFinished);

  // create access mutex
  m_mutex = ege_new Mutex(d_func()->app(), EGEMutex::Recursive);
  if (NULL == m_mutex)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }
  
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
  if (IS_AL_ERROR())
  {
    egeCritical() << l_result << "Could not generate sources.";
    EGE_MEMSET(m_channels, 0, sizeof (m_channels));

    return EGE_ERROR;
  }

  // start thread
  if ( ! m_thread->start())
  {
    // error!
    return EGE_ERROR;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerPrivate::update(const Time& time)
{
  // do nothing, updated from another thread
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerPrivate::threadUpdate(const Time& time)
{
  if (AudioManager::STATE_CLOSING == d_func()->state())
  {
    if (m_thread->isFinished() || !m_thread->isRunning())
    {
      // set state
      d_func()->m_state = AudioManager::STATE_CLOSED;
      return;
    }
  }

  MutexLocker locker(m_mutex);

  // go thru all sounds
  // NOTE: no const iterator due to GCC compiler complaints
  for (AudioManager::SoundList::iterator it = d_func()->m_sounds.begin(); it != d_func()->m_sounds.end();)
  {
    const PSound& sound = *it;

    //egeDebug()  << "Updating sound" << sound->name();

    // check if sound is stopped
    if (isStopped(sound))
    {
      d_func()->m_sounds.erase(it++);
      continue;
    }

    // update sound
    sound->update(time);
    ++it;
  }

  // start pending playbacks
  for (AudioManager::SoundList::iterator it = m_soundsToPlay.begin(); it != m_soundsToPlay.end(); ++it)
  {
    const PSound& sound = *it;

    // play
    if (EGE_SUCCESS == doPlay(sound))
    {
      // connect
      ege_connect(sound, stopped, d_func(), AudioManager::onStopped);

      // add to pool
      d_func()->m_sounds.push_back(sound);
    }
  }
  m_soundsToPlay.clear();

  // stop playbacks
  for (AudioManager::SoundList::iterator it = m_soundsToStop.begin(); it != m_soundsToStop.end(); ++it)
  {
    const PSound& sound = *it;

    // play
    doStop(sound);
  }
  m_soundsToStop.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult AudioManagerPrivate::play(const PSound& sound)
{
  // add to pool for later playback
  MutexLocker locker(m_mutex);
  m_soundsToPlay.push_back(sound);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AudioManagerPrivate::isPlaying(const String& soundName) const
{
  MutexLocker locker(m_mutex);

  // look for the sound in the active pool
  for (AudioManager::SoundList::const_iterator it = d_func()->m_sounds.begin(); it != d_func()->m_sounds.end(); ++it)
  {
    const PSound& sound = *it;
    if (sound->name() == soundName)
    {
      return true;
    }
  }

  // look for the sounds-to-be-played pool
  for (AudioManager::SoundList::const_iterator it = m_soundsToPlay.begin(); it != m_soundsToPlay.end(); ++it)
  {
    const PSound& sound = *it;
    if (sound->name() == soundName)
    {
      return true;
    }
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ALuint AudioManagerPrivate::availableChannel() const
{
  // go thru all channels
  for (s32 i = 0; i < CHANNELS_COUNT; ++i)
  {
    ALint state;
		alGetSourcei(m_channels[i], AL_SOURCE_STATE, &state);
    if (IS_AL_ERROR())
    {
      // error!
      egeCritical() << l_result << "Could not retrieve channel state:" << m_channels[i] << "at index" << i;
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
EGEResult AudioManagerPrivate::stop(const PSound& sound)
{
  // add to pool for later stop
  MutexLocker locker(m_mutex);
  m_soundsToStop.push_back(sound);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerPrivate::stop(const String& soundName)
{
  MutexLocker locker(m_mutex);

  // look for the sound in the active pool
  for (AudioManager::SoundList::const_iterator it = d_func()->m_sounds.begin(); it != d_func()->m_sounds.end(); ++it)
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
List<PSound> AudioManagerPrivate::sounds(const String& soundName) const
{
  List<PSound> list;

  // go thru all active sounds
  for (AudioManager::SoundList::const_iterator it = d_func()->m_sounds.begin(); it != d_func()->m_sounds.end(); ++it)
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
bool AudioManagerPrivate::isPlaying(const PSound& sound) const
{
  return m_soundsToPlay.contains(sound) || sound->p_func()->isPlaying();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult AudioManagerPrivate::pause(const String& soundName)
{
  MutexLocker locker(m_mutex);

  // look for the sound in the active pool
  for (AudioManager::SoundList::const_iterator it = d_func()->m_sounds.begin(); it != d_func()->m_sounds.end(); ++it)
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
EGEResult AudioManagerPrivate::pause(const PSound& sound)
{
  return sound->p_func()->pause();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AudioManagerPrivate::isPaused(const String& soundName) const
{
  MutexLocker locker(m_mutex);

  // look for the sound in the active pool
  for (AudioManager::SoundList::const_iterator it = d_func()->m_sounds.begin(); it != d_func()->m_sounds.end(); ++it)
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
bool AudioManagerPrivate::isPaused(const PSound& sound) const
{
  return sound->p_func()->isPaused();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AudioManagerPrivate::isStopped(const PSound& sound) const
{
  return sound->p_func()->isStopped();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult AudioManagerPrivate::doPlay(const PSound& sound)
{
  // check if paused
  if (isPaused(sound))
  {
    // resume playback
    return sound->p_func()->resume();
  }

  // try to find available channel
  ALuint channel = availableChannel();
  if (0 < channel)
  {
    SoundPrivate* soundPrivate = sound->p_func();

    // start playback
    return soundPrivate->play(channel);
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerPrivate::doStop(const PSound& sound)
{
  sound->p_func()->stop();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerPrivate::shutDown() 
{ 
  if (m_thread != NULL)
  {
    m_thread->stop(0); 
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerPrivate::onThreadFinished(const PThread& thread)
{
  EGE_UNUSED(thread);

  d_func()->m_state = AudioManager::STATE_CLOSED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerPrivate::setEnable(bool set)
{
  // check if disabling
  if ( ! set)
  {
    // stop all active sounds
    // NOTE: make a copy of all sounds first as there is no guarantee on when sounds will be removed from pool
    m_mutex->lock();
    AudioManager::SoundList soundsToStop(d_func()->m_sounds);
    m_mutex->unlock();
    for (AudioManager::SoundList::iterator it = soundsToStop.begin(); it != soundsToStop.end(); ++it)
    {
      // stop
      stop(*it);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_AUDIO_OPENAL