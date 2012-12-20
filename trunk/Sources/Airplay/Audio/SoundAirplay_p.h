#ifndef EGE_AIRPLAY_SOUND_PRIVATE_H
#define EGE_AIRPLAY_SOUND_PRIVATE_H

/** Sound class used with non-software mixing. 
 */

#if EGE_AUDIO_AIRPLAY && !EGE_AIRPLAY_AUDIO_SOFTWARE

#include <EGE.h>
#include <EGEDataBuffer.h>
#include <EGETime.h>
#include <EGEList.h>
#include <EGEDynamicArray.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Sound, PSound)
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

    /*! Constructs object. */
    EGEResult construct();
    /*! Updates object. */
    void update(const Time& time);
    /*! Starts playback. */
    EGEResult play();
    /*! Stops playback. */
    EGEResult stop();
    /*! Returns TRUE if sound is being played. */
    bool isPlaying() const;
    /*! Pauses playback. */
    EGEResult pause();
    /*! Returns TRUE if sound is paused. */
    bool isPaused() const;
    /*! Returns TRUE if sound is stopped. */
    bool isStopped() const;

  private slots:

    /*! Slot called on sound volume change. */
    void onSoundVolumeChanged(PSound sound, float32 oldVolume);

  private:

    /*! Channel sound is being played on. */
    s32 m_channel;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_AUDIO_AIRPLAY && !EGE_AIRPLAY_AUDIO_SOFTWARE

#endif // EGE_AIRPLAY_SOUND_PRIVATE_H
