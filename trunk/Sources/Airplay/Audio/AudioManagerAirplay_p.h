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
     * @param sound       Sound to play.
     * @param repeatCount Number of times sound should be repeated.
     * @return  Returns EGE_SUCCESS if sound is sucessfully started or EGE_ERROR if sound could not be started.
     * @note  When repeatCount is set to zero the sound is going to be played exactly once. For negative values sound will be played forever.
     */
    EGEResult play(const PSound& sound, s32 repeatCount);
    /* Returns TRUE if sound of a given name is being played. */
    bool isPlaying(const String& soundName) const;
    /* Stops playback of the sound with a given name. 
     * @param soundName     Name of the sound to stop playback.
     * @param fadeDuration  Time interval during which sound should be faded out.
     */
    void stop(const String& soundName, const Time& fadeDuration);

  private slots:

    /* Slot called on sound volume change. */
    void soundVolumeChanged(const PSound& sound, float32 oldVolume);
    /* Slot called on sound fade out completion. */
    void soundFadeOutComplete(const PSound& sound);

  private:

    typedef List<PSound> SoundsList;

  private:

    /*! Currently played compressed audio. */
    PSound m_compessedAudio;
    /*! Currently played uncompressed audio samples. */
    SoundsList m_uncompressedAudio;
 };

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // !EGE_AIRPLAY_AUDIO_SOFTWARE

#endif // EGE_AIRPLAY_AUDIOMANAGER_PRIVATE_H