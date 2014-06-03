#include "Core/Audio/Implementation/Codecs/AudioCodecWav.h"
#include "Core/Audio/Implementation/AudioUtils.h"
#include "EGEMath.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KAudioCodecWavDebugName = "EGEAudioCodecWav";
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
bool AudioCodecWav::decode(const PDataBuffer& out, s32 samplesCount, s32& samplesDecoded)
{
  const s32 sampleSize = m_channels * (m_bitsPerSample >> 3);

  PDataBuffer stream;
  if (EGE_OBJECT_UID_DATA_BUFFER == m_stream->uid())
  {
    stream = ege_pcast<PDataBuffer>(m_stream);

    // apply current stream offset
    // NOTE: multiple codecs can re-use same stream
    stream->setReadOffset(m_streamOffset);

    // calculate number of samples we can read
    samplesDecoded = Math::Min(samplesCount, m_streamSizeLeft / sampleSize);

    // move required data from stream into output buffer
    // NOTE: this should not require multiple attepts as read is done from memory buffer
    if (out->write(stream, samplesDecoded * sampleSize) != samplesDecoded * sampleSize)
    {
      // error!
      egeCritical(KAudioCodecWavDebugName) << "Error decoding!";
    }

    // update stream offset
    m_streamOffset = stream->readOffset();
  }
  else
  {
    EGE_ASSERT_X(false, "Not supported");
  }

  // update stream size left indicator
  m_streamSizeLeft -= samplesDecoded * sampleSize;

  return (0 == m_streamSizeLeft);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AudioCodecWav::reset()
{
  PDataBuffer stream;
  if (EGE_OBJECT_UID_DATA_BUFFER == m_stream->uid())
  {
    stream = ege_pcast<PDataBuffer>(m_stream);

    // reset stream to begining
    stream->setReadOffset(0);

    AudioUtils::ReadWavHeaders(stream, m_riffHeader, m_fmtHeader, m_dataHeader);
  }
  else
  {
    EGE_ASSERT_X(false, "Not supported");
  }

  // store data
  m_channels        = m_fmtHeader.channels;
  m_frequency       = m_fmtHeader.sampleRate;
  m_bitsPerSample   = m_fmtHeader.bitsPerSample;
  m_streamSizeLeft  = m_dataHeader.size;

  // set read offset at the begining of wave data
  m_streamOffset = m_dataHeader.offset;

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 AudioCodecWav::remainingSamplesCount() const
{
  const s32 sampleSize = m_channels * (m_bitsPerSample >> 3);

  return m_streamSizeLeft / sampleSize;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END