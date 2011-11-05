#ifndef EGE_AIRPLAY_AUDIOMANAGER_PRIVATE_H
#define EGE_AIRPLAY_AUDIOMANAGER_PRIVATE_H

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
    /* Plays given sound. */
    EGEResult play(const PSound& sound);

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

    /*! Data structure for active sound. */
    struct SoundData
    {
      s32 channel;                                    /*!< Channel sound was original played on.*/
      PSound sound;                                   /*!< Sound played. */
    };

    typedef List<SoundData> SoundsDataList;

  private:

    /*! Set of all active sounds. */
    SoundsDataList m_sounds;
    /*! Resample factor for sound being currently processed. */
    float32 m_resampleFactor;
    /*! Mixing flag for sound being currently processed. */
    bool m_mixing;

    SoundPrivate* m_sound;
 };

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_AIRPLAY_AUDIOMANAGER_PRIVATE_H