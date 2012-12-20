#ifndef EGE_CORE_AUDIO_CODEC_WAV_H
#define EGE_CORE_AUDIO_CODEC_WAV_H

/** WAVE audio file codec specialization. 
*/

#include <EGE.h>
#include "Core/Audio/AudioCodec.h"
#include <EGEAudio.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AudioCodecWav : public AudioCodec
{
  public:

    AudioCodecWav(const PDataBuffer& stream);
    virtual ~AudioCodecWav();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

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
    EGEAudio::WaveRiffHeader m_riffHeader;
    /*! Fmt chunk header. */
    EGEAudio::WaveFmtHeader m_fmtHeader;
    /*! Data chunk header. */
    EGEAudio::WaveDataHeader m_dataHeader;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIO_CODEC_WAV_H