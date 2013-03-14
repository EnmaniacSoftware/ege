#ifndef EGE_CORE_AUDIO_CODEC_MP3_H
#define EGE_CORE_AUDIO_CODEC_MP3_H

/** MP3 audio file codec specialization. 
*/

#include "EGE.h"
#include "Core/Audio/AudioCodec.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AudioCodecMp3 : public AudioCodec
{
  public:

    AudioCodecMp3(const PDataBuffer& stream);
    virtual ~AudioCodecMp3();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! @see AudioCodec::decode. */
    bool decode(const PDataBuffer& out, s32 samplesCount, s32& samplesDecoded) override;
    /*! @see AudioCodec::reset. */
    bool reset() override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIO_CODEC_MP3_H