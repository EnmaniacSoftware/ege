#ifndef EGE_CORE_AUDIO_CODEC_H
#define EGE_CORE_AUDIO_CODEC_H

/** Class represents audio codec interface.
*/

#include <EGE.h>
#include <EGEDataBuffer.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(Object, PObject)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class AudioCodec : public Object
{
  public:

    AudioCodec(const PDataBuffer& stream) : Object(NULL), m_stream(stream), m_channels(0), m_frequency(0), m_bitsPerSample(0) {}
    virtual ~AudioCodec() {}

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
    inline s32 channels() const { return m_channels; }
    /*! Returns playback frequency (in Hz). */
    inline s32 frequency() const { return m_frequency; }
    /*! Returns number of bits per sample (for single channel). */
    inline s32 bitsPerSample() const { return m_bitsPerSample; }

  protected:

    /*! Data stream. */
    PObject m_stream;
    /*! Number of channels. */
    s32 m_channels;
    /*! Playback frequency (sample rate) (in Hz). */
    s32 m_frequency;
    /*! Number of bits per sample (for single channel). */
    s32 m_bitsPerSample;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIO_CODEC_H