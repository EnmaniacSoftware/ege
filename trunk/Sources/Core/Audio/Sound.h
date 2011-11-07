#ifndef EGE_CORE_SOUND_H
#define EGE_CORE_SOUND_H

/** Objects of this class represents discreet sound effect.
*/

#include <EGE.h>
#include <EGEDataBuffer.h>
#include <EGEString.h>

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

  public:

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /*! Returns name. */
    const String& name() const { return m_name; }
    /* Sets pitch value. */
    void setPitch(float32 value);
    /*! Returns pitch value. */
    inline float32 pitch() const { return m_pitch; }
    /* Sets gain value. */
    void setGain(float32 value);
    /*! Returns gain value. */
    inline float32 gain() const { return m_gain; }
    /* Sets repeat count. */
    void setRepeatCount(s32 count);
    /*! Returns number of repeats left. */
    s32 repeatsLeft() const { return m_repeatsLeft; }
    /*! Returns codec. */
    inline AudioCodec* codec() const { return m_codec; }

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(Sound);

    /*! Name. */
    String m_name;
    /*! Pitch value. */
    float32 m_pitch;
    /*! Gain value. */
    float32 m_gain;
    /*! Number of repeats left. */
    s32 m_repeatsLeft;
    /*! Audio codec. */
    AudioCodec* m_codec;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SOUND_H