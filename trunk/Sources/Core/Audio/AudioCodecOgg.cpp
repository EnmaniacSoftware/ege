#include "Core/Audio/AudioCodecOgg.h"
#include "EGEAudio.h"
#include "EGEMath.h"
#include "EGEDebug.h"
#include "EGEFile.h"

#include "Core/Audio/stb_vorbis.c"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(AudioCodecOgg)
EGE_DEFINE_DELETE_OPERATORS(AudioCodecOgg)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function clipping given value to S16 range. Taken from AIRPLAY SDK. */
static s16 ClipToS16(s32 value)
{
  // quick overflow test, the addition moves valid range to 0-allbits
  if ((value - Math::MIN_S16) & ~Math::MAX_U16)
  {
    // we overflowed.
    if (value > Math::MAX_S16)
    {
        value = Math::MAX_S16;
    }
    else if (value < Math::MIN_S16)
    {
      value = Math::MIN_S16;
    }
  }

  return (s16) value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioCodecOgg::AudioCodecOgg(const PDataBuffer& stream) : AudioCodec(EGE_OBJECT_UID_AUDIO_CODEC_OGG, stream), 
                                                          m_codecStream(NULL)
{
  reset();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioCodecOgg::~AudioCodecOgg()
{
  close();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AudioCodecOgg::decode(const PDataBuffer& out, s32 samplesCount, s32& samplesDecoded)
{
  // check if no codec stream
  if (NULL == m_codecStream)
  {
    // do nothing
    return false;
  }

  const s32 sampleSize = (m_channels * (m_bitsPerSample >> 3));

  // preallocate memory in output buffer
  out->setCapacity(samplesCount * sampleSize);

  // reset data
  samplesDecoded = 0;

  PDataBuffer stream;
  if (EGE_OBJECT_UID_DATA_BUFFER == m_stream->uid())
  {
    stream = m_stream;

    // check if there are some overflous samples present from last decompression interation
    if (0 < m_overflousDecodedSamples.size())
    {
      // determine number of overflous samples in buffer
      s32 overflousSamplesCount = static_cast<s32>((m_overflousDecodedSamples.size() - m_overflousDecodedSamples.readOffset())) / sampleSize;

      // read as much as we can from overflous buffer
      samplesDecoded += Math::Min(overflousSamplesCount, samplesCount);
      out->write(m_overflousDecodedSamples, samplesDecoded * sampleSize);

      // check if overflous buffer is completly read
      if (m_overflousDecodedSamples.readOffset() == m_overflousDecodedSamples.size())
      {
        // clear it up so it does not get any bigger for the time being
        m_overflousDecodedSamples.clear();
      }
    }

    // further decompression is needed
    unsigned char* data = reinterpret_cast<unsigned char*>(stream->data(m_streamOffset));
    s32 offset = 0;
    int n;
    float* left;
    float* right;
    float** outputs;
    int num_c;
    int dataLength = Math::Min(128, static_cast<int>(stream->size() - m_streamOffset));
    while (samplesDecoded != samplesCount)
    {
      // try to decode frame
      s32 used = stb_vorbis_decode_frame_pushdata(m_codecStream, data + offset, dataLength, &num_c, &outputs, &n);

      // check if nothing has been decoded
      if (0 == used) 
      {
        // continue with 128-byte blocks
        dataLength = Math::Max(128, dataLength);

        // try to get some additional data length
        int newDataLength = Math::Min(dataLength << 1, static_cast<int>(stream->size() - m_streamOffset));

        // check if no more data available
        if (0 == newDataLength)
        {
          // no more data, we r done
          return true;
        }

        // try again
        dataLength = newDataLength;
        continue;
      }

      // update offset
      offset += used;
      m_streamOffset += used;

      if (0 == n)
      {
        // seek/error recovery
        continue;
      }

      left = outputs[0];
      right = num_c > 1 ? outputs[1] : outputs[0];

      s32 count = Math::Min(static_cast<s32>(n), samplesCount - samplesDecoded);
      if (1 == m_channels)
      {
        uploadMonoChannel(out, count, n, left);
      }
      else
      {
        uploadStereoChannels(out, count, n, left, right);
      }

      // update number of decoded samples which ended up into output buffer
      samplesDecoded += count;
    }
  }
  else
  {
    EGE_ASSERT("Not supported");
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioCodecOgg::uploadMonoChannel(const PDataBuffer& out, s32 outCount, s32 totalCount, float* channel)
{
  // process data which should end up directly in output buffer
  s32 i = 0;
  for (; i < outCount; ++i)
  {
    s16 x = ClipToS16(static_cast<s32>(channel[i] * Math::MAX_S16));
    *out << x;
  }

  // process data which needs to be buffered (overflous samples)
  for (; i < totalCount; ++i)
  {
    s16 x = ClipToS16(static_cast<s32>(channel[i] * Math::MAX_S16));
    m_overflousDecodedSamples << x;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioCodecOgg::uploadStereoChannels(const PDataBuffer& out, s32 outCount, s32 totalCount, float* leftChannel, float* rightChannel)
{
  // process data which should end up directly in output buffer
  s32 i = 0;
  for (; i < outCount; ++i)
  {
    s16 x = ClipToS16(static_cast<s32>(leftChannel[i] * Math::MAX_S16));
    *out << x;
    x = ClipToS16(static_cast<s32>(rightChannel[i] * Math::MAX_S16));
    *out << x;
  }

  // process data which needs to be buffered (overflous samples)
  for (; i < totalCount; ++i)
  {
    s16 x = ClipToS16(static_cast<s32>(leftChannel[i] * Math::MAX_S16));
    m_overflousDecodedSamples << x;
    x = ClipToS16(static_cast<s32>(rightChannel[i] * Math::MAX_S16));
    m_overflousDecodedSamples << x;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AudioCodecOgg::reset()
{
  int error;
  int streamDataUsed;

  // close any opened stream
  // TAGE - perhaps there is a way to reset if it is too expensive to close/reopen
  close();

  PDataBuffer stream;
  if (EGE_OBJECT_UID_DATA_BUFFER == m_stream->uid())
  {
    stream = m_stream;
        
    // initially supply up to 1024 bytes
    s32 dataLength = Math::Min(static_cast<s32>(1024), static_cast<s32>(stream->size()));
    do
    {
      // try to initialize stream with current data length
      m_codecStream = stb_vorbis_open_pushdata(reinterpret_cast<unsigned char*>(stream->data()), dataLength, &streamDataUsed, &error, NULL);

      // check if failed
      if (NULL == m_codecStream) 
      {
        // check if more data needed
        if (VORBIS_need_more_data == error) 
        {
          // try to increase data length by up to next 1024 bytes
          s32 newDataLength = Math::Min(dataLength + 1024, static_cast<s32>(stream->size()));

          // check if no more data is available
          if (newDataLength == dataLength)
          {
            // error! not enough data in a stream
            return false;
          }

          // try again with new data length
          dataLength = newDataLength;
          continue;
        }

        // error!
        return false;
      }
    }
    while (NULL == m_codecStream);

    // set stream read offset to exactly what was used
    m_streamOffset = streamDataUsed;  
  }
  else
  {
      EGE_ASSERT("Not supported");
  }

  // retrive stream data
  stb_vorbis_info info = stb_vorbis_get_info(m_codecStream);
  m_channels  = info.channels;
  m_frequency = info.sample_rate;

  // vorbis internally works on floating point samples but we will convert to 16 bit integers where required
  m_bitsPerSample = 16;

  // preallocate memory for 512 samples
  m_overflousDecodedSamples.setCapacity(512 * (m_channels * (m_bitsPerSample >> 3)));
  m_overflousDecodedSamples.setByteOrdering(ELittleEndian);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioCodecOgg::close()
{
  if (m_codecStream)
  {
    stb_vorbis_close(m_codecStream);
    m_codecStream = NULL;
  }

  m_streamOffset = 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END