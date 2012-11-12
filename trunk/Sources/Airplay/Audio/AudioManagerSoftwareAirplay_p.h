#ifndef EGE_AIRPLAY_AUDIOMANAGERSOFTWARE_PRIVATE_H
#define EGE_AIRPLAY_AUDIOMANAGERSOFTWARE_PRIVATE_H

/** Audio manager class responsible for software sound mixing. 
 */

#if EGE_AUDIO_AIRPLAY && EGE_AIRPLAY_AUDIO_SOFTWARE

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
    /* Stops playback of the sound with a given name. */
    void stop(const String& soundName);

  private:

    /* Airplay callback function called once sample has ended. */
    static int32 SampleEndCallback(void* systemData, void* userData);
    /* Airplay callback function called everytime when more audio data is required. */
    static int32 AudioCallback(void* systemData, void* userData);
    /* Uploads mono channel into output buffer. 
     * @param out         Output buffer data in being uploaded into.
     * @param data        Input data buffer from which samples are to be mixed from.
     * @param count       Total number of samples to be uploaded.
     */
    void uploadMonoChannel(int16* out, int16* data, u32 count) const;
    /* Uploads stereo channel into output buffer. 
     * @param out         Output buffer data in being uploaded into.
     * @param data        Input data buffer from which samples are to be mixed from.
     * @param count       Total number of samples to be uploaded.
     */
    void uploadStereoChannels(int16* out, int16* data, u32 count) const;    

  private:

    typedef List<PSound> SoundsList;

  private:

    /*! Set of all active sounds. */
    SoundsList m_sounds;
    /*! Resample factor for sound being currently processed. */
    float32 m_resampleFactor;
    /*! Mixing flag for sound being currently processed. */
    bool m_mixing;
    /*! Pointer to sound being currently processed. */
    SoundPrivate* m_sound;
 };

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_AUDIO_AIRPLAY && EGE_AIRPLAY_AUDIO_SOFTWARE

#endif // EGE_AIRPLAY_AUDIOMANAGER_PRIVATE_H