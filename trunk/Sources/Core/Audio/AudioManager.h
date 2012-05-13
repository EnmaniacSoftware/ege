#ifndef EGE_CORE_AUDIOMANAGER_H
#define EGE_CORE_AUDIOMANAGER_H

/** Class managing audio.
 */

#include <EGE.h>
#include <EGETime.h>
#include <EGEList.h>
#include <EGEThread.h>
#include <EGEMutex.h>
#include "Core/Event/EventListener.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Sound, PSound)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AudioManager : public Object, public IEventListener
{
  /*! For accessing private data. */
  friend class AudioThread;

  public:

    AudioManager(Application* app);
   ~AudioManager();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Available states. */
    enum State
    {
      STATE_NONE = -1,
      STATE_INITIALIZING,
      STATE_READY,
      STATE_CLOSING,
      STATE_CLOSED
    };

  public:

    /* Creates object. */
    EGEResult construct();
    /*! Returns current state. */
    inline State state() const { return m_state; }
    /* Updates manager. */
    void update(const Time& time);
    /* Plays sound with given name. 
     * @param soundName   Name of the sound to play.
     * @param repeatCount Number of times sound should be repeated.
     * @return  Returns EGE_SUCCESS if sound is sucessfully started, EGE_ERROR_NOT_FOUND if sound could not be found or EGE_ERROR if sound could not be started.
     * @note  When repeatCount is set to zero the sound is going to be played exactly once. For negative values sound will be played forever.
     */
    EGEResult play(const String& soundName, s32 repeatCount = 0);
    /* Plays given sound.
     * @param sound Sound to play.
     * @return  Returns EGE_SUCCESS if sound is sucessfully started or EGE_ERROR if sound could not be started.
     */
    EGEResult play(const PSound& sound);
    /* Returns TRUE if sound of a given name is being played. */
    bool isPlaying(const String& soundName) const;
    /* Returns TRUE if given sound is being played. */
    bool isPlaying(const PSound& sound) const;
    /* Stops playback of all sounds with a given name. */
    void stop(const String& soundName);
    /* Returns list of sounds being played with the given name. */
    List<PSound> sounds(const String& soundName) const;
    /* Pauses sound with given name. 
     * @param soundName   Name of the sound to pause.
     * @return  Returns EGE_SUCCESS if sound is sucessfully paused, EGE_ERROR_NOT_FOUND if sound could not be found or EGE_ERROR if sound could not be paused.
     */
    EGEResult pause(const String& soundName);
    /* Pauses given sound.
     * @param sound Sound to pause.
     * @return  Returns EGE_SUCCESS if sound is sucessfully paused or EGE_ERROR if sound could not be paused.
     */
    EGEResult pause(const PSound& sound);
    /* Returns TRUE if sound of a given name is paused. */
    bool isPaused(const String& soundName) const;
    /* Returns TRUE if given sound is paused. */
    bool isPaused(const PSound& sound) const;

    /* Enables/disables manager. 
     * @note Disabling manager stops all currently played sounds and prevents any sound from being played. Enabling manager will allow sound to be played.
     *       However, it will not resume playback of any sounds stopped during last disabling request.
     */
    void setEnable(bool set);
    /*! Returns TRUE if manager is enabled. */
    inline bool isEnabled() const { return m_enabled; }

  public slots:

    /* Stops playback of the given sound. */
    void stop(PSound sound);

  private:

    /* EventListener override. Event reciever. */
    void onEventRecieved(PEvent event) override;
    /* Shuts down. */
    void shutDown();

  private:

    typedef List<PSound> SoundList;

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(AudioManager);

    /*! List of sounds being played. */
    SoundList m_sounds;
    /*! Enable flag. */
    bool m_enabled;
    /*! Audio thread. */
    PThread m_thread;
    /*! Data access mutex. */
    PMutex m_mutex;
    /*! Current state. */
    State m_state;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIOMANAGER_H