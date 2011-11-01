#ifndef EGE_CORE_AUDIO_CODEC_OGG_H
#define EGE_CORE_AUDIO_CODEC_OGG_H

/** OGG audio file codec specialization. 
*/

#include <EGE.h>
#include "Core/Audio/AudioCodec.h"

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

  private:

    /*! Stream data size left (in bytes). */
    s32 m_streamSizeLeft;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIO_CODEC_OGG_H