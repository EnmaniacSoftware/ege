#include "Core/Audio/AudioUtils.h"
#include <EGEDataBuffer.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define WAVE_RIFF_HEADER_ID 0x52494646
#define WAVE_FMT_HEADER_ID  0x666d7420
#define WAVE_DATA_HEADER_ID 0x64617461
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
struct WaveRiffHeader
{
  u32 id;               /*!< RIFF or 0x52494646 big-endian. */
  s32 size;             /*!< This is size of entire file minus 8 bytes for id and size entires of this struct. */
  u32 format;           /*!< WAVE or 0x57415645 big-endian. */
};

struct WaveFmtHeader
{
  u32 id;                 /*!< fmt or 0x666d7420 big-endian. */
  s32 size;               /*!< Size of the rest of header starting from next entry. For PCM = 16. */
  s16 audioFormat;        /*!< 1 for PCM. Other values for some compressed formats. */
  s16 channels;           /*!< Mono = 1, Stereo = 2 etc. */
  s32 sampleRate;         /*!< 8000, 44100 etc. */
  s32 byteRate;           /*!< SampleRate * NumChannels * BitsPerSample/8. */
  s16 blockAlign;         /*!< NumChannels * BitsPerSample/8. */
  s16 bitsPerSample;      /*!< 8 - for 8 bits, 16 - for 16 bits etc. */
};

struct WaveDataHeader
{
  u32 id;               /*!< data or 0x64617461 big-endian. */
  s32 size;             /*!< This is size of sound data (in bytes). */
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Retrieves data from WAV file.
 * @param data            Data buffer containing wav file data.
 * @param soundDataOffset Offset (in bytes) to begining of sound data.
 * @param soundDataSize   Size (in bytes) of all sound data.
 * @param channels        Number of channels of wav file.
 * @param sampleRate      Sample rate of wav file.
 * @param bitsPerSample   Number of bits per sample of wav file.
 * @return  Returns success if given buffer contains valid wav file data and data has been retrieved successfully.
 */
EGEResult AudioUtils::GetWavData(const PDataBuffer& data, s32& soundDataOffset, s32& soundDataSize, s32& channels, s32& sampleRate, s32& bitsPerSample)
{
  WaveRiffHeader riffHeader;
  WaveFmtHeader fmtHeader;
  WaveDataHeader dataHeader;

  if (data->size() < sizeof (WaveFmtHeader) + sizeof (WaveRiffHeader) + sizeof (WaveDataHeader))
  {
    // insuffcient data size
    return EGE_ERROR;
  }

  // read headers in
  data->setByteOrdering(EGEByteOrder::BIG_ENDIAN);
  *data >> riffHeader.id;
  data->setByteOrdering(EGEByteOrder::LITTLE_ENDIAN);
  *data >> riffHeader.size;
  data->setByteOrdering(EGEByteOrder::BIG_ENDIAN);
  *data >> riffHeader.format;

  data->setByteOrdering(EGEByteOrder::BIG_ENDIAN);
  *data >> fmtHeader.id;
  data->setByteOrdering(EGEByteOrder::LITTLE_ENDIAN);
  *data >> fmtHeader.size;
  *data >> fmtHeader.audioFormat;
  *data >> fmtHeader.channels;
  *data >> fmtHeader.sampleRate;
  *data >> fmtHeader.byteRate;
  *data >> fmtHeader.blockAlign;
  *data >> fmtHeader.bitsPerSample;

  // skip extra data
  if (16 < fmtHeader.size)
  {
    data->setReadOffset(data->readOffset() + fmtHeader.size - 16);
  }

  data->setByteOrdering(EGEByteOrder::BIG_ENDIAN);
  *data >> dataHeader.id;
  data->setByteOrdering(EGEByteOrder::LITTLE_ENDIAN);
  *data >> dataHeader.size;

  // check if wave format
  if ((WAVE_RIFF_HEADER_ID != riffHeader.id) || (WAVE_FMT_HEADER_ID != fmtHeader.id) || (WAVE_DATA_HEADER_ID != dataHeader.id))
  {
    // error!
    return EGE_ERROR;
  }
  
  // store data
  channels        = fmtHeader.channels;
  sampleRate      = fmtHeader.sampleRate;
  bitsPerSample   = fmtHeader.bitsPerSample;
  soundDataSize   = dataHeader.size;
  soundDataOffset = static_cast<s32>(data->readOffset());

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
