#include "Core/Audio/AudioCodecWav.h"
#include <EGEMath.h>
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(AudioCodecWav)
EGE_DEFINE_DELETE_OPERATORS(AudioCodecWav)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioCodecWav::AudioCodecWav(const PDataBuffer& stream) : AudioCodec(EGE_OBJECT_UID_AUDIO_CODEC_WAV, stream)
{
  reset();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioCodecWav::~AudioCodecWav()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* AudioCodec override. Decodes up to given number of samples. 
 * @param  out             Data buffer containing decoded samples.
 * @param  samplesCount    Number of samples to be decoded.
 * @param  samplesDecoded  Number of actual samples decoded. May be different than requested.
 * @return Returns TRUE if end of data has been reached. Otherwise, false.
 * @note Number of actually decoded samples may be less than requested if end of data is reached. Otherwise it should match the requested value.
 */
bool AudioCodecWav::decode(const PDataBuffer& out, s32 samplesCount, s32& samplesDecoded)
{
  const s32 sampleSize = m_channels * (m_bitsPerSample >> 3);

  PDataBuffer stream;
  if (EGE_OBJECT_UID_DATA_BUFFER == m_stream->uid())
  {
    stream = m_stream;

    // calculate number of samples we can read
    samplesDecoded = Math::Min(samplesCount, m_streamSizeLeft / sampleSize);

    // move required data from stream into output buffer
    // NOTE: this should not require multiple attepts as read is done from memory buffer
    if (out->write(stream, samplesDecoded * sampleSize) != samplesDecoded * sampleSize)
    {
      // error!
      egeCritical() << "Error decoding!";
    }
  }
  else
  {
    EGE_ASSERT("Not supported");
  }

  // update stream size left indicator
  m_streamSizeLeft -= samplesDecoded * sampleSize;

  return (0 == m_streamSizeLeft);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Resets codec. */
bool AudioCodecWav::reset()
{
  PDataBuffer stream;
  if (EGE_OBJECT_UID_DATA_BUFFER == m_stream->uid())
  {
    stream = m_stream;

    stream->setReadOffset(0);
    AudioUtils::ReadWavHeaders(stream, m_riffHeader, m_fmtHeader, m_dataHeader);
  }
  else
  {
    EGE_ASSERT("Not supported");
  }

  // store data
  m_channels        = m_fmtHeader.channels;
  m_frequency       = m_fmtHeader.sampleRate;
  m_bitsPerSample   = m_fmtHeader.bitsPerSample;
  m_streamSizeLeft  = m_dataHeader.size;

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns number of samples left. */
u32 AudioCodecWav::remainingSamplesCount() const
{
  const s32 sampleSize = m_channels * (m_bitsPerSample >> 3);

  return m_streamSizeLeft / sampleSize;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END