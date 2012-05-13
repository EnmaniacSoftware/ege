#ifndef EGE_AIRPLAY_AUDIOMANAGER_PRIVATE_H
#define EGE_AIRPLAY_AUDIOMANAGER_PRIVATE_H

/** Audio manager class responsible for non-software sound mixing. 
 *  @note Due to Airplay (or underlying hardware) limitations it is only possible to play one compressed audio at any given time. It is possible, however,
 *        to play multiple uncompressed samples.
 */

#ifndef EGE_AIRPLAY_AUDIO_SOFTWARE

#include <EGE.h>
#include <EGETime.h>
#include <EGEList.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class AudioManager;
class SoundPrivate;

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

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Updates manager. */
    void update(const Time& time);
    /* Plays given sound.
     * @param sound Sound to play.
     * @return  Returns EGE_SUCCESS if sound is sucessfully started or EGE_ERROR if sound could not be started.
     */
    EGEResult play(const PSound& sound);
    /* Returns TRUE if given sound is being played. */
    bool isPlaying(const PSound& sound) const;
    /* Stops playback of the given sound. */
    EGEResult stop(const PSound& sound);
    /* Pauses given sound.
     * @param sound Sound to pause.
     * @return  Returns EGE_SUCCESS if sound is sucessfully paused or EGE_ERROR if sound could not be paused.
     */
    EGEResult pause(const PSound& sound);
    /* Returns TRUE if given sound is paused. */
    bool isPaused(const PSound& sound) const;
    /* Returns TRUE if given sound is stopped. */
    bool isStopped(const PSound& sound) const;

  private:

    typedef List<PSound> SoundsList;

  private:

    /*! Currently played compressed audio. */
//    PSound m_compessedAudio;
    /*! Currently played uncompressed audio samples. */
  //  SoundsList m_uncompressedAudio;
 };

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // !EGE_AIRPLAY_AUDIO_SOFTWARE

#endif // EGE_AIRPLAY_AUDIOMANAGER_PRIVATE_H