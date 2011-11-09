#ifndef EGE_CORE_SOUND_H
#define EGE_CORE_SOUND_H

/** Objects of this class represents discreet sound effect.
*/

#include <EGE.h>
#include <EGEDataBuffer.h>
#include <EGEString.h>
#include <EGESignal.h>
#include <EGETime.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class AudioCodec;

EGE_DECLARE_SMART_CLASS(Sound, PSound)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Sound : public Object
{
  friend class AudioManagerPrivate;

  public:

    Sound(const String& name, const PDataBuffer& data);
    virtual ~Sound();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  signals:

    /*! Signal emitted when volume changes. 
     *  @param  Sound for which volume has changed.
     *  @param  oldVolume Old volume level.
     */
    Signal2<const PSound&, float32> volumeChanged;
    /*! Signal emitted when fade in state is over. 
     *  @param  Sound for which fade in has been completed.
     */
    Signal1<const PSound&> fadeInComplete;
    /*! Signal emitted when fade in state is over. 
     *  @param  Sound for which fade out has been completed.
     */
    Signal1<const PSound&> fadeOutComplete;

  public:

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Updates object. */
    void update(const Time& time);
    /*! Returns name. */
    const String& name() const { return m_name; }
    /* Sets pitch value. */
    void setPitch(float32 value);
    /*! Returns pitch value. */
    inline float32 pitch() const { return m_pitch; }
    /* Sets repeat count. */
    void setRepeatCount(s32 count);
    /*! Returns number of repeats left. */
    s32 repeatsLeft() const { return m_repeatsLeft; }
    /*! Returns codec. */
    inline AudioCodec* codec() const { return m_codec; }
    /* Sets volume. */
    void setVolume(float32 volume);
    /*! Returns current volume. */
    inline float32 volume() const { return m_volume; }
    /* Starts fading out. */
    void startFadeOut(const Time& duration);
    /* Starts fading in. */
    void startFadeIn(const Time& duration);

  private:

    /*! Available states. */
    enum State
    {
      STATE_IDLE,
      STATE_FADING_IN,
      STATE_FADING_OUT
    };

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(Sound);

    /*! Name. */
    String m_name;
    /*! Pitch value. Each reduction by 50% repesents 1 octave reduction. Each doubling represents 1 octave increase. */
    float32 m_pitch;
    /*! Number of repeats left. */
    s32 m_repeatsLeft;
    /*! Audio codec. */
    AudioCodec* m_codec;
    /*! Volume in [0-1] range. */
    float32 m_volume;
    /*! Current state. */
    State m_state;
    /*! Fade duration. */
    Time m_fadeDuration;
    /*! Current fade time. */
    Time m_fadeTime;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SOUND_H