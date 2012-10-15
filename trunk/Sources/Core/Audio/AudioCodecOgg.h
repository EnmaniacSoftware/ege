#ifndef EGE_CORE_AUDIO_CODEC_OGG_H
#define EGE_CORE_AUDIO_CODEC_OGG_H

/** OGG audio file codec specialization. 
*/

#include <EGE.h>
#include "Core/Audio/AudioCodec.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
struct stb_vorbis;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AudioCodecOgg : public AudioCodec
{
  public:

    AudioCodecOgg(const PDataBuffer& stream);
    virtual ~AudioCodecOgg();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* AudioCodec override. Decodes up to given number of samples. 
     *  @param  out             Data buffer containing decoded samples.
     *  @param  samplesCount    Number of samples to be decoded.
     *  @param  samplesDecoded  Number of actual samples decoded. May be different than requested.
     *  @return Returns TRUE if end of data has been reached. Otherwise, false.
     *  @note Number of actually decoded samples may be less than requested if end of data is reached. Otherwise it should match the requested value.
     */
    bool decode(const PDataBuffer& out, s32 samplesCount, s32& samplesDecoded) override;
    /* Resets codec. */
    bool reset() override;

  private:

    /* Uploads mono channel into data buffers. 
     * @param out         Primary output buffer data in being uploaded into.
     * @param outCount    Number of samples to be uploaded to primary buffer.
     * @param totalCount  Total number of samples to be uploaded.
     * @param channel     Samples data to be uploaded into the buffer.
     * @note  Only outCount number of samples will be uploaded into primary buffer. Any exceeding samples will be appended into overflous buffer. Sample data
     *        is mapped into 16-bit integer samples before uploading.
     */
    void uploadMonoChannel(const PDataBuffer& out, s32 outCount, s32 totalCount, float* channel);
    /* Uploads stereo channels into data buffers.
     * @param out             Primary output buffer data in being uploaded into.
     * @param outCount        Number of samples to be uploaded to primary buffer.
     * @param totalCount      Total number of samples to be uploaded.
     * @param leftChannel     Samples data of left channel to be uploaded into the buffer.
     * @param rightChannel    Samples data of right channel to be uploaded into the buffer.
     * @note  Only outCount number of samples will be uploaded into primary buffer. Any exceeding samples will be appended into overflous buffer. Sample data
     *        is mapped into 16-bit integer samples before uploading.
     */
    void uploadStereoChannels(const PDataBuffer& out, s32 outCount, s32 totalCount, float* leftChannel, float* rightChannel);

  private:

    /* Closes stream. */
    void close();

  private:

    /*! STB vorbis stream object. */
    stb_vorbis* m_codecStream;
    /*! Buffer containing all samples which were decoded but were over the samples count requested. These will be used next time samples are requested before
        any decompression takes place.
     */
    DataBuffer m_overflousDecodedSamples;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIO_CODEC_OGG_H