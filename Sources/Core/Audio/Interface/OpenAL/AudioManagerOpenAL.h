#ifndef EGE_CORE_AUDIO_OPENAL_AUDIOMANAGEROPENAL_H
#define EGE_CORE_AUDIO_OPENAL_AUDIOMANAGEROPENAL_H

/** This class represents NULL audio manager.
 */

#include "EGE.h"
#include "EGETime.h"
#include "EGEMutex.h"
#include "EGEThread.h"
#include "EGEAudio.h"
#include "Core/Audio/Implementation/AudioManagerBase.h"

#ifdef EGE_PLATFORM_WIN32
  #include <al.h>
  #include <alc.h>
#elif EGE_PLATFORM_IOS
  #import <OpenAL/al.h>
  #import <OpenAL/alc.h>
#endif // EGE_PLATFORM_WIN32

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef EGE_FEATURE_DEBUG
  #define OAL_CHECK() if (AL_NO_ERROR != alGetError()) { egeCritical("EGEOpenAL") << "[OAL] Error" << __FILE__ << __LINE__; }
  #define IS_OAL_ERROR() (AL_NO_ERROR != alGetError())
#else
  #define OAL_CHECK() ege_noop();
  #define IS_OAL_ERROR() (false)
#endif // EGE_FEATURE_DEBUG
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define CHANNELS_COUNT 24
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Sound, PSound)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AudioManagerOpenAL : public Object, public IAudioManagerBase, public IAudioManager
{
  public:
    
    AudioManagerOpenAL(Application* app);
   ~AudioManagerOpenAL();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! @see IAudioManagerBase::requestPlay. */
    EGEResult requestPlay(PSound sound) override;
    /*! @see IAudioManagerBase::requestStop. */
    void requestStop(PSound sound) override;
    /*! @see IAudioManagerBase::requestPause. */    
    void requestPause(PSound sound) override;

    /*! @see IAudioManager::construct. */
    EGEResult construct() override;
    /*! @see IAudioManager::update. */
    void update(const Time& time) override;
    /*! @see IAudioManager::setEnabled. */
    void setEnable(bool set) override;
    /*! @see IAudioManager::isEnabled. */
    bool isEnabled() const override;
    /*! @see IAudioManager::createSound. */
    PSound createSound(const String& name, PDataBuffer& data) const override;
    /*! @see IAudioManager::state. */
    EState state() const override;

    /*! Updates manager. Meant to be used from another thread. */
    void threadUpdate(const Time& time);

  private slots:

    /*! Slot called when audio thread is finished. */
    void onThreadFinished(const PThread& thread);
    /*! Slot called when given sound stopped playback. */
    //void onStopped(PSound sound);

  private:

    /*! Queues given sound for stop. 
     *  @param  sound Sound to be stopped.
     */
    void queueForStop(PSound& sound);
    /*! Queues given sound for playback. 
     *  @param  sound Sound to be started.
     */
    void queueForPlay(PSound& sound);
    /*! Queues given sound for playback pause. 
     *  @param  sound Sound to be paused.
     */
    void queueForPause(PSound& sound);
    /*! Returns first available channel.
     *  @return Available channel ID. Zero is returns if no valid channel has been found.
     */
    ALuint findAvailableChannel() const;

    /*! @ see IEventListener::onEventRecieved. */
    void onEventRecieved(PEvent event) override;
    /*! Shuts down. */
    void shutDown();

  private:

    /*! Current state. */
    IAudioManager::EState m_state;
    /*! Audio device. */
    ALCdevice* m_device;
    /*! Audio context. */
    ALCcontext* m_context;
    /*! Available channels. */
    ALuint m_channels[CHANNELS_COUNT];
    /*! List of sounds being played. */
    SoundList m_sounds;
    /*! List of sounds to start playing. */
    SoundList m_soundsToPlay;
    /*! List of sounds to stop playing. */
    SoundList m_soundsToStop;
    /*! List of sounds to pause. 
     *  @note Sounds scheduled for pausing also exists in @ref m_sounds list.
     */
    SoundList m_soundsToPause;
    /*! Audio thread. */
    PThread m_thread;
    /*! Data access mutex. */
    PMutex m_mutex;
    /*! Enable flag. */
    bool m_enabled;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIO_OPENAL_AUDIOMANAGEROPENAL_H