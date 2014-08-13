#ifndef EGE_CORE_AUDIO_CODECS_AUDIOCODECWAV_H
#define EGE_CORE_AUDIO_CODECS_AUDIOCODECWAV_H

/** WAVE audio file codec specialization. 
 */

#include "EGE.h"
#include "Core/Audio/Implementation/Codecs/AudioCodec.h"
#include "Core/Audio/Implementation/AudioUtils.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AudioCodecWav : public AudioCodec
{
  public:

    AudioCodecWav(const PDataBuffer& stream);
    virtual ~AudioCodecWav();

  private:

    /*! @see AudioCodec::decode. */
    bool decode(const PDataBuffer& out, s32 samplesCount, s32& samplesDecoded) override;
    /*! @see AudioCodec::reset. */
    bool reset() override;
    /*! Returns number of samples left. */
    u32 remainingSamplesCount() const;

  private:

    /*! Stream data size left (in bytes). */
    s32 m_streamSizeLeft;
    /*! Riff chunk header. */
    AudioWavRiffHeader m_riffHeader;
    /*! Fmt chunk header. */
    AudioWavFormatHeader m_fmtHeader;
    /*! Data chunk header. */
    AudioWavDataHeader m_dataHeader;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIO_CODECS_AUDIOCODECWAV_H