#include "Core/Audio/Implementation/AudioUtils.h"
#include "EGEDataBuffer.h"
#include "EGEDataStream.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define WAVE_RIFF_HEADER_ID 0x52494646
#define WAVE_FMT_HEADER_ID  0x666d7420
#define WAVE_DATA_HEADER_ID 0x64617461
#define OGG_PAGE_HEADER_ID  0x4f676753
#define MP3_ID3_HEADER_ID   0x49443300
#define MP3_ENC_HEADER_ID   0xfffb0000
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioUtils::ReadWavHeaders(const PDataBuffer& data, AudioWavRiffHeader& riffHeader, AudioWavFormatHeader& fmtHeader, AudioWavDataHeader& dataHeader)
{
  EGE_MEMSET(&riffHeader, 0, sizeof (riffHeader));
  EGE_MEMSET(&fmtHeader, 0, sizeof (fmtHeader));
  EGE_MEMSET(&dataHeader, 0, sizeof (dataHeader));

  DataStream stream(data);

  // read in RIFF header
  stream.setByteOrdering(EBigEndian);
  stream >> riffHeader.id;
  stream.setByteOrdering(ELittleEndian);
  stream >> riffHeader.size;
  stream.setByteOrdering(EBigEndian);
  stream >> riffHeader.format;

  // check if there is a chance this is wav file
  if (WAVE_RIFF_HEADER_ID == riffHeader.id)
  {
    // read all chunks till we have required ones
    while ((data->size() != data->readOffset()) && ((WAVE_FMT_HEADER_ID != fmtHeader.id) || (WAVE_DATA_HEADER_ID != dataHeader.id)))
    {
      // read chunk header
      u32 chunkId;
      u32 chunkSize;
      stream.setByteOrdering(EBigEndian);
      stream >> chunkId;

      // read chunk size
      stream.setByteOrdering(ELittleEndian);
      stream>> chunkSize;

      // check if FMT chunk
      switch (chunkId)
      {
        case WAVE_FMT_HEADER_ID:

          fmtHeader.id = chunkId;
          fmtHeader.size = chunkSize;

          // read rest of header
          stream >> fmtHeader.audioFormat;
          stream >> fmtHeader.channels;
          stream >> fmtHeader.sampleRate;
          stream >> fmtHeader.byteRate;
          stream >> fmtHeader.blockAlign;
          stream >> fmtHeader.bitsPerSample;

          // skip extra data
          if (16 < fmtHeader.size)
          {
            data->setReadOffset(data->readOffset() + chunkSize - 16);
          }
          break;

        case WAVE_DATA_HEADER_ID:

          dataHeader.id     = chunkId;
          dataHeader.size   = chunkSize;
          dataHeader.offset = static_cast<u32>(data->readOffset());
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
AudioStreamType AudioUtils::DetectStreamType(const PDataBuffer& data)
{
  DataStream stream(data);

  // check if WAVE stream
  data->setReadOffset(0);

  // check if amount of data at least for the headers
  if (data->size() > sizeof (AudioWavFormatHeader) + sizeof (AudioWavRiffHeader) + sizeof (AudioWavDataHeader))
  {
    AudioWavRiffHeader riffHeader;
    AudioWavFormatHeader fmtHeader;
    AudioWavDataHeader dataHeader;

    // read headers in
    AudioUtils::ReadWavHeaders(data, riffHeader, fmtHeader, dataHeader);

    // check if wave format
    if ((WAVE_RIFF_HEADER_ID == riffHeader.id) && (WAVE_FMT_HEADER_ID == fmtHeader.id) && (WAVE_DATA_HEADER_ID == dataHeader.id))
    {
      // found
      return AST_WAV;
    }
  }

  // check if OGG stream
  data->setReadOffset(0);
  stream.setByteOrdering(EBigEndian);

  u32 headerId;
  stream >> headerId;
 
  if (OGG_PAGE_HEADER_ID == headerId)
  {
    // found
    return AST_OGG;
  }

  // check if MP3 stream
  // NOTE: due to many different variations of MP3 formats we do the following checks:
  // - for ID3v2 header which can be pre-appended or appended:
  //    - if pre-appended it is at the begining of file
  //    - if appened it is 10 bytes from the end of file (current not done!)
  // - for ENC header first two bytes needs to be 0xFFFB
  data->setReadOffset(0);
  stream.setByteOrdering(EBigEndian);
  
  stream >> headerId;
  if ((MP3_ID3_HEADER_ID == (headerId & 0xffffff00)) || (MP3_ENC_HEADER_ID == (headerId & 0xffff0000)))
  {
    // found
    return AST_MP3;
  }

  return AST_UNKNOWN;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END