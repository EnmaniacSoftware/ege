#ifndef EGE_CORE_SOUND_OPENAL_H
#define EGE_CORE_SOUND_OPENAL_H

#ifdef EGE_AUDIO_OPENAL

#include <EGE.h>
#include <EGEDataBuffer.h>
#include <al.h>
#include <alc.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define BUFFERS_COUNT 3
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

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Updates object. */
    void update(const Time& time);
    /* Starts playback on given channel. */
    EGEResult play(ALuint channel);
    /* Stops playback. */
    EGEResult stop();
    /* Returns TRUE if sound is being played. */
    bool isPlaying() const;
    /* Pauses playback. */
    EGEResult pause();
    /* Returns TRUE if sound is paused. */
    bool isPaused() const;

  private:

    /* Updates sound buffers. */
    void updateBuffers();

  private slots:

    /* Slot called on sound volume change. */
    void onSoundVolumeChanged(PSound sound, float32 oldVolume);

  private:

    /*! OpenAL sound buffer objects. */
    ALuint m_buffers[BUFFERS_COUNT];
    /*! OpenAL buffer format. */
    ALenum m_format;
    /*! OpenAL channel id sound is being played on. */
    ALuint m_channel;
    /*! Flag indicating sound has been stopped and should not be updated anymore. */
    bool m_stopped;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_NAMESPACE_END

#endif // EGE_AUDIO_OPENAL

#endif // EGE_CORE_SOUND_OPENAL_H