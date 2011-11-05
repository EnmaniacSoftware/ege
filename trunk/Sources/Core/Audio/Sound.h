#ifndef EGE_CORE_SOUND_H
#define EGE_CORE_SOUND_H

/** Objects of this class represents discreet sound effect.
*/

#include <EGE.h>
#include <EGEDataBuffer.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class AudioCodec;

EGE_DECLARE_SMART_CLASS(Sound, PSound)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Sound : public Object
{
  friend class AudioManagerPrivate;

  public:

    Sound(const PDataBuffer& data);
    virtual ~Sound();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Sets pitch value. */
    void setPitch(float32 value);
    /*! Returns pitch value. */
    inline float32 pitch() const { return m_pitch; }
    /* Sets gain value. */
    void setGain(float32 value);
    /*! Returns gain value. */
    inline float32 gain() const { return m_gain; }
    /* Sets looping flag. */
    void setLooping(bool set);
    /*! Returns looping flag. */
    inline bool looping() const { return m_looping; }
    /*! Returns codec. */
    inline AudioCodec* codec() const { return m_codec; }

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(Sound);

    /*! Pitch value. */
    float32 m_pitch;
    /*! Gain value. */
    float32 m_gain;
    /*! Looping flag. */
    bool m_looping;
    /*! Audio codec. */
    AudioCodec* m_codec;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SOUND_H