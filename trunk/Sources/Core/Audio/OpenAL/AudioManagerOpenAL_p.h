#ifndef EGE_CORE_AUDIOMANAGER_OPENAL_H
#define EGE_CORE_AUDIOMANAGER_OPENAL_H

#ifdef EGE_AUDIO_OPENAL

#include "EGE.h"
#include "EGETime.h"
#include "EGEThread.h"
#include "EGEMutex.h"
#include "Core/Audio/AudioManager.h"
#include <al.h>
#include <alc.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define CHANNELS_COUNT 24
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AudioManager;

EGE_DECLARE_SMART_CLASS(Sound, PSound)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AudioManagerPrivate
{
  public:

    AudioManagerPrivate(AudioManager* base);
   ~AudioManagerPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(AudioManager)

    /*! Constructs object. */
    EGEResult construct();
    /*! Updates manager. */
    void update(const Time& time);
    /*! Updates manager. Meant to be used from another thread. */
    void threadUpdate(const Time& time);

    /*! Plays given sound.
     *  @param sound Sound to play.
     *  @return  Returns EGE_SUCCESS if sound is sucessfully started or EGE_ERROR if sound could not be started.
     */
    EGEResult play(const PSound& sound);
    /*! Returns TRUE if given sound is being played. */
    bool isPlaying(const PSound& sound) const;
    /*! Returns TRUE if sound of a given name is being played. */
    bool isPlaying(const String& soundName) const;
    /*! Stops playback of the given sound. */
    EGEResult stop(const PSound& sound);
    /*! Stops playback of all sounds with a given name. */
    void stop(const String& soundName);
    /*! Pauses sound with given name. 
     *  @param soundName   Name of the sound to pause.
     *  @return  Returns EGE_SUCCESS if sound is sucessfully paused, EGE_ERROR_NOT_FOUND if sound could not be found or EGE_ERROR if sound could not be paused.
     */
    EGEResult pause(const String& soundName);
    /*! Pauses given sound.
     *  @param sound Sound to pause.
     *  @return  Returns EGE_SUCCESS if sound is sucessfully paused or EGE_ERROR if sound could not be paused.
     */
    EGEResult pause(const PSound& sound);
    /*! Returns TRUE if sound of a given name is paused. */
    bool isPaused(const String& soundName) const;
    /*! Returns TRUE if given sound is paused. */
    bool isPaused(const PSound& sound) const;
    /*! Returns TRUE if given sound is stopped. */
    bool isStopped(const PSound& sound) const;

    /*! Returns list of sounds being played with the given name. */
    List<PSound> sounds(const String& soundName) const;

    /*! Enables/disables manager. 
     *  @note Disabling manager stops all currently played sounds and prevents any sound from being played. Enabling manager will allow sound to be played.
     *        However, it will not resume playback of any sounds stopped during last disabling request.
     */
    void setEnable(bool set);

    /*! Shuts manager down. */
    void shutDown();

  private slots:

    /*! Slot called when audio thread is finished. */
    void onThreadFinished(const PThread& thread);

  private:

    /*! Returns first available channel. */
    ALuint availableChannel() const;
    /*! Starts playback of a given sound. 
     *  @param  sound Sound to start.
     *  @note This method is called from audio manager's thread.
     *  @return EGE_SUCCESS if playback has been started successfully.
     */
    EGEResult doPlay(const PSound& sound);
    /*! Stops playback of a given sound. 
     *  @param  sound Sound to stop.
     *  @note This method is called from audio manager's thread.
     */
    void doStop(const PSound& sound);

  private:

    /*! Audio device. */
    ALCdevice* m_device;
    /*! Audio context. */
    ALCcontext* m_context;
    /*! Available channels. */
    ALuint m_channels[CHANNELS_COUNT];
    /*! List of sounds to start playing. */
    AudioManager::SoundList m_soundsToPlay;
    /*! List of sounds to stop playing. */
    AudioManager::SoundList m_soundsToStop;
    /*! Audio thread. */
    PThread m_thread;
    /*! Data access mutex. */
    PMutex m_mutex;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_AUDIO_OPENAL

#endif // EGE_CORE_AUDIOMANAGER_OPENAL_H