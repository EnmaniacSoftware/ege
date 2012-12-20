#ifndef EGE_CORE_AUDIOMANAGER_NULL_PRIVATE_H
#define EGE_CORE_AUDIOMANAGER_NULL_PRIVATE_H

#ifdef EGE_AUDIO_NULL

#include <EGE.h>
#include <EGETime.h>

EGE_NAMESPACE_BEGIN

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
    /*! Pauses given sound.
     *  @param sound Sound to pause.
     *  @return  Returns EGE_SUCCESS if sound is sucessfully paused or EGE_ERROR if sound could not be paused.
     */
    EGEResult pause(const PSound& sound);
    /*! Returns TRUE if given sound is paused. */
    bool isPaused(const PSound& sound) const;
    /*! Returns TRUE if given sound is stopped. */
    bool isStopped(const PSound& sound) const;

    /*! Shuts manager down. */
    void shutDown();
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_AUDIO_NULL

#endif // EGE_CORE_AUDIOMANAGER_NULL_PRIVATE_H