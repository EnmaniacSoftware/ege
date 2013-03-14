#ifndef EGE_CORE_AUDIO_CODEC_OGG_H
#define EGE_CORE_AUDIO_CODEC_OGG_H

/** OGG audio file codec specialization. 
*/

#include "EGE.h"
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

    /*! @see AudioCodec::decode. */
    bool decode(const PDataBuffer& out, s32 samplesCount, s32& samplesDecoded) override;
    /*! @see AudioCodec::reset. */
    bool reset() override;

  private:

    /*! Uploads mono channel into data buffers. 
     *  @param out         Primary output buffer data in being uploaded into.
     *  @param outCount    Number of samples to be uploaded to primary buffer.
     *  @param totalCount  Total number of samples to be uploaded.
     *  @param channel     Samples data to be uploaded into the buffer.
     *  @note  Only outCount number of samples will be uploaded into primary buffer. Any exceeding samples will be appended into overflous buffer. Sample data
     *         is mapped into 16-bit integer samples before uploading.
     */
    void uploadMonoChannel(const PDataBuffer& out, s32 outCount, s32 totalCount, float* channel);
    /*! Uploads stereo channels into data buffers.
     *  @param out             Primary output buffer data in being uploaded into.
     *  @param outCount        Number of samples to be uploaded to primary buffer.
     *  @param totalCount      Total number of samples to be uploaded.
     *  @param leftChannel     Samples data of left channel to be uploaded into the buffer.
     *  @param rightChannel    Samples data of right channel to be uploaded into the buffer.
     *  @note  Only outCount number of samples will be uploaded into primary buffer. Any exceeding samples will be appended into overflous buffer. Sample data
     *         is mapped into 16-bit integer samples before uploading.
     */
    void uploadStereoChannels(const PDataBuffer& out, s32 outCount, s32 totalCount, float* leftChannel, float* rightChannel);

  private:

    /*! Closes stream. */
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