#include "Core/Audio/AudioUtils.h"
#include <EGEDataBuffer.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define WAVE_RIFF_HEADER_ID 0x52494646
#define WAVE_FMT_HEADER_ID  0x666d7420
#define WAVE_DATA_HEADER_ID 0x64617461
#define OGG_PAGE_HEADER_ID  0x4f676753
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Retrieves WAV stream headers. */
void AudioUtils::ReadWavHeaders(const PDataBuffer& data, EGEAudio::WaveRiffHeader& riffHeader, EGEAudio::WaveFmtHeader& fmtHeader, 
                                EGEAudio::WaveDataHeader& dataHeader)
{
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
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Detects stream type of given data. */
EGEAudio::StreamType AudioUtils::DetectStreamType(const PDataBuffer& data)
{
  // check if WAVE stream
  data->setReadOffset(0);

  // check if amount of data at least for the headers
  if (data->size() > sizeof (EGEAudio::WaveFmtHeader) + sizeof (EGEAudio::WaveRiffHeader) + sizeof (EGEAudio::WaveDataHeader))
  {
    EGEAudio::WaveRiffHeader riffHeader;
    EGEAudio::WaveFmtHeader fmtHeader;
    EGEAudio::WaveDataHeader dataHeader;

    // read headers in
    AudioUtils::ReadWavHeaders(data, riffHeader, fmtHeader, dataHeader);

    // check if wave format
    if ((WAVE_RIFF_HEADER_ID == riffHeader.id) && (WAVE_FMT_HEADER_ID == fmtHeader.id) && (WAVE_DATA_HEADER_ID == dataHeader.id))
    {
      // found
      return EGEAudio::ST_WAVE;
    }
  }

  // check if OGG stream
  data->setReadOffset(0);
  data->setByteOrdering(EGEByteOrder::BIG_ENDIAN);

  u32 headerId;
  *data >> headerId;
 
  if (OGG_PAGE_HEADER_ID == headerId)
  {
    // found
    return EGEAudio::ST_OGG;
  }

  return EGEAudio::ST_UNKNOWN;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------