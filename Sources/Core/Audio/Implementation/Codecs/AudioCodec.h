#ifndef EGE_CORE_AUDIO_CODECS_AUDIOCODEC_H
#define EGE_CORE_AUDIO_CODECS_AUDIOCODEC_H

/** Class represents audio codec interface.
 */

#include "EGE.h"
#include "EGEDataBuffer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Engine;
EGE_PREDECLARE_SMART_CLASS(Object, PObject)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AudioCodec : public Object
{
  public:

    AudioCodec(u32 uid, const PDataBuffer& stream);
    virtual ~AudioCodec();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Decodes up to given number of samples. 
     *  @param  out             Data buffer containing decoded samples.
     *  @param  samplesCount    Number of samples to be decoded.
     *  @param  samplesDecoded  Number of actual samples decoded. May be different than requested.
     *  @return Returns TRUE if end of data has been reached. Otherwise, false.
     *  @note Number of actually decoded samples may be less than requested if end of data is reached. Otherwise it should match the requested value.
     */
    virtual bool decode(const PDataBuffer& out, s32 samplesCount, s32& samplesDecoded) = 0;
    /*! Resets codec. */
    virtual bool reset() = 0;

    /*! Returns number of channels. */
    s32 channels() const;
    /*! Returns playback frequency (in Hz). */
    s32 frequency() const;
    /*! Returns number of bits per sample (for single channel). */
    s32 bitsPerSample() const;
    /*! Returns data stream. */
    const PObject& stream() const;

  protected:

    /*! Data stream. 
     *  @note This is potentially shared resource ie. from sound resource, likely accessed from multiple threads. Internal offsets should nto be modified 
     *        directly. Use m_streamOffset to offest manually.
     */
    PObject m_stream;
    /*! Position within stream. */
    s64 m_streamOffset;
    /*! Number of channels. */
    s32 m_channels;
    /*! Playback frequency (sample rate) (in Hz). */
    s32 m_frequency;
    /*! Number of bits per sample (for single channel). */
    s32 m_bitsPerSample;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIO_CODECS_AUDIOCODEC_H