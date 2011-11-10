#ifndef EGE_AIRPLAY_SOUND_PRIVATE_H
#define EGE_AIRPLAY_SOUND_PRIVATE_H

/** Sound class used with non-software mixing. 
 */

#ifndef EGE_AIRPLAY_AUDIO_SOFTWARE

#include <EGE.h>
#include <EGEDataBuffer.h>
#include <EGETime.h>
#include <EGEList.h>
#include <EGEDynamicArray.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Sound;
class AudioManagerPrivate;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class SoundPrivate
{
  public:

    SoundPrivate(Sound* base);
   ~SoundPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(Sound)

  public:

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Updates object. */
    void update(const Time& time);
    /* Starts playback. */
    EGEResult play();
    /* Stops playback. */
    EGEResult stop();
    /* Returns TRUE if sound is being played. */
    bool isPlaying() const;

  private slots:

    /* Slot called on sound volume change. */
    void onSoundVolumeChanged(const Sound* sound, float32 oldVolume);

  private:

    /*! Channel sound is being played on. */
    s32 m_channel;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // !EGE_AIRPLAY_AUDIO_SOFTWARE

#endif // EGE_AIRPLAY_SOUND_PRIVATE_H
