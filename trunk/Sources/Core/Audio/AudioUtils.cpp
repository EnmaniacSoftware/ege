#include "Core/Audio/AudioUtils.h"
#include "EGEDataBuffer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define WAVE_RIFF_HEADER_ID 0x52494646
#define WAVE_FMT_HEADER_ID  0x666d7420
#define WAVE_DATA_HEADER_ID 0x64617461
#define OGG_PAGE_HEADER_ID  0x4f676753
#define MP3_ID3_HEADER_ID   0x49443300
#define MP3_ENC_HEADER_ID   0xfffb0000
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioUtils::ReadWavHeaders(const PDataBuffer& data, EGEAudio::WaveRiffHeader& riffHeader, EGEAudio::WaveFmtHeader& fmtHeader, 
                                EGEAudio::WaveDataHeader& dataHeader)
{
  EGE_MEMSET(&riffHeader, 0, sizeof (riffHeader));
  EGE_MEMSET(&fmtHeader, 0, sizeof (fmtHeader));
  EGE_MEMSET(&dataHeader, 0, sizeof (dataHeader));

  // read in RIFF header
  data->setByteOrdering(EBigEndian);
  *data >> riffHeader.id;
  data->setByteOrdering(ELittleEndian);
  *data >> riffHeader.size;
  data->setByteOrdering(EBigEndian);
  *data >> riffHeader.format;

  // check if there is a chance this is wav file
  if (WAVE_RIFF_HEADER_ID == riffHeader.id)
  {
    // read all chunks till we have required ones
    while ((data->size() != data->readOffset()) && ((WAVE_FMT_HEADER_ID != fmtHeader.id) || (WAVE_DATA_HEADER_ID != dataHeader.id)))
    {
      // read chunk header
      u32 chunkId;
      u32 chunkSize;
      data->setByteOrdering(EBigEndian);
      *data >> chunkId;

      // read chunk size
      data->setByteOrdering(ELittleEndian);
      *data >> chunkSize;

      // check if FMT chunk
      switch (chunkId)
      {
        case WAVE_FMT_HEADER_ID:

          fmtHeader.id = chunkId;
          fmtHeader.size = chunkSize;

          // read rest of header
          *data >> fmtHeader.audioFormat;
          *data >> fmtHeader.channels;
          *data >> fmtHeader.sampleRate;
          *data >> fmtHeader.byteRate;
          *data >> fmtHeader.blockAlign;
          *data >> fmtHeader.bitsPerSample;

          // skip extra data
          if (16 < fmtHeader.size)
          {
            data->setReadOffset(data->readOffset() + chunkSize - 16);
          }
          break;

        case WAVE_DATA_HEADER_ID:

          dataHeader.id   = chunkId;
          dataHeader.size = chunkSize;
          break;

        default:

          // skip header
          data->setReadOffset(data->readOffset() + chunkSize);
          break;
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
  data->setByteOrdering(EBigEndian);

  u32 headerId;
  *data >> headerId;
 
  if (OGG_PAGE_HEADER_ID == headerId)
  {
    // found
    return EGEAudio::ST_OGG;
  }

  // check if MP3 stream
  // NOTE: due to many different variations of MP3 formats we do the following checks:
  // - for ID3v2 header which can be pre-appended or appended:
  //    - if pre-appended it is at the begining of file
  //    - if appened it is 10 bytes from the end of file (current not done!)
  // - for ENC header first two bytes needs to be 0xFFFB
  data->setReadOffset(0);
  data->setByteOrdering(EBigEndian);
  
  *data >> headerId;
  if ((MP3_ID3_HEADER_ID == (headerId & 0xffffff00)) || (MP3_ENC_HEADER_ID == (headerId & 0xffff0000)))
  {
    // found
    return EGEAudio::ST_MP3;
  }

  return EGEAudio::ST_UNKNOWN;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END