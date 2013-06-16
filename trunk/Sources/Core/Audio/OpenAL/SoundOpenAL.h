#ifndef EGE_CORE_AUDIO_OPENAL_SOUNDOPENAL_H
#define EGE_CORE_AUDIO_OPENAL_SOUNDOPENAL_H

#include "EGE.h"
#include "Core/Audio/Sound.h"

#ifdef EGE_PLATFORM_WIN32
  #include <al.h>
  #include <alc.h>
#elif EGE_PLATFORM_IOS
  #import <OpenAL/al.h>
  #import <OpenAL/alc.h>
#endif // EGE_PLATFORM_WIN32

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define BUFFERS_COUNT 2
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AudioManagerOpenAL;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SoundOpenAL : public Sound
{
  public:

    SoundOpenAL(AudioManagerOpenAL* manager, const String& name, PDataBuffer& data);
   ~SoundOpenAL();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Available states. */
    enum State
    {
      StateNone = 0,        /*!< Invalid state. */
      StateAboutToStop,     /*!< Scheduled for stopping. */
      StateStopped,         /*!< Stopped. */
      StateAboutToPlay,     /*!< Scheduled for playing. */
      StatePlaying,         /*!< Playing. */
      StateAboutToPause,    /*!< Schedules for pause. */
      StatePaused           /*!< Paused. */
    };

  public:

    /*! @see Sound::construct. */
    virtual EGEResult construct() override;
    /*! @see Sound::update. */
    virtual void update(const Time& time) override;
    /*! @see Sound::setPitch. */
    void setPitch(float32 value) override;
    /*! @see Sound::pitch. */
    float32 pitch() const override;
    /*! @see Sound::setVolume. */
    virtual void setVolume(float32 volume) override;
    /*! @see Sound::volume. */
    float32 volume() const override;
    /*! @see Sound::play. */
    EGEResult play(s32 repeatCount) override;
    /*! @see Sound::isPlaying. */
    bool isPlaying() const override;
    /*! @see Sound::isPaused. */
    bool isPaused() const override;
    /*! @see Sound::stop. */
    void stop() override;
    /*! @see Sound::pause. */
    void pause() override;

    /*! Returns current state. */
    State state() const;

    /*! Starts playback.
     *  @param  channel OpenAL channel ID at which playback should be started.
     *  @return TRUE if successfully started.
     *  @note This is for AudioManager use.
     */
    virtual bool doPlay(ALuint channel);
    /*! Stop playback.
     *  @return TRUE if successfully stopped.
     *  @note This is for AudioManager use.
     */
    virtual bool doStop();
    /*! Pauses playback. 
     *  @return TRUE if successfully paused.
     *  @note This is for AudioManager use.
     */
    virtual bool doPause();
    /*! Resumes playback. 
     *  @return TRUE if successfully resumed.
     *  @note This is for AudioManager use.
     */
    virtual bool doResume();

  protected:

    /*! Sets state. 
     *  @param  state New state to set.
     */
    void setState(State state);
    /*! Returns pointer to manager. */
    AudioManagerOpenAL* manager() const;
    /*! Returns repeat count. */
    s32 repeatCount() const;
  
  private:

    /*! Updates sound buffers. */
    void updateSoundBuffers();

  private:

    /*! Pointer to manager. */
    AudioManagerOpenAL* m_manager;
    /*! Playback repeat counter. */
    s32 m_repeatsLeft;
    /*! Current state. */
    State m_state;
    /*! OpenAL sound buffer objects. */
    ALuint m_buffers[BUFFERS_COUNT];
    /*! OpenAL buffer format. */
    ALenum m_format;
    /*! OpenAL channel id sound is being played on. */
    ALuint m_channel;
    /*! Pitch value. Each reduction by 50% repesents 1 octave reduction. Each doubling represents 1 octave increase. */
    float32 m_pitch;
    /*! Volume in [0-1] range. */
    float32 m_volume;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIO_OPENAL_SOUNDOPENAL_H