#include "Core/Audio/AudioUtils.h"
#include <EGEDataBuffer.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
struct WaveRiffHeader
{
  u8 id[4];               /*!< RIFF or 0x52494646 big-endian. */
  s32 size;               /*!< This is size of entire file minus 8 bytes for id and size entires of this struct. */
  u8 format[4];           /*!< WAVE or 0x57415645 big-endian. */
};

struct WaveFmtHeader
{
  u8 id[4];               /*!< fmt or 0x666d7420 big-endian. */
  s32 size;               /*!< Size of the rest of header starting from next entry. For PCM = 16. */
  s16 audioFormat;        /*!< 1 for PCM. Other values for some compressed formats. */
  s16 channels;           /*!< Mono = 1, Stereo = 2 etc. */
  s32 sampleRate;         /*!< 8000, 44100 etc. */
  s32 byteRate;           /*!< SampleRate * NumChannels * BitsPerSample/8. */
  s16 blockAlign;         /*!< NumChannels * BitsPerSample/8. */
  s16 bitsPerSample;      /*!< 8 - for 8 bits, 16 - for 16 bits etc. */
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Retrieves data from WAV file.
 * @param data            Data buffer containing wav file data.
 * @param soundDataOffset Offset (in bytes) to begining of sound data.
 * @param channels        Number of channels of wav file.
 * @param sampleRate      Sample rate of wav file.
 * @param bitsPerSample   Number of bits per sample of wav file.
 * @return  Returns success if given buffer contains valid wav file data and data has been retrieved successfully.
 */
EGEResult AudioUtils::GetWavData(const PDataBuffer& data, s32& soundDataOffset, s32& channels, s32& sampleRate, s32& bitsPerSample)
{
  WaveRiffHeader riffHeader;
  WaveFmtHeader fmtHeader;

  if (data->size() < sizeof (WaveFmtHeader) + sizeof (WaveRiffHeader))
  {
    // insuffcient data size
    return EGE_ERROR;
  }

  // read headers in
  *data >> riffHeader.id[0];
  *data >> riffHeader.id[1];
  *data >> riffHeader.id[2];
  *data >> riffHeader.id[3];
  *data >> riffHeader.size;
  *data >> riffHeader.format[0];
  *data >> riffHeader.format[1];
  *data >> riffHeader.format[2];
  *data >> riffHeader.format[3];

  *data >> fmtHeader.id[0];
  *data >> fmtHeader.id[1];
  *data >> fmtHeader.id[2];
  *data >> fmtHeader.id[3];
  *data >> fmtHeader.size;
  *data >> fmtHeader.audioFormat;
  *data >> fmtHeader.channels;
  *data >> fmtHeader.sampleRate;
  *data >> fmtHeader.byteRate;
  *data >> fmtHeader.blockAlign;
  *data >> fmtHeader.bitsPerSample;

  // check if wave format
  if (('R' != riffHeader.id[0]) || ('I' != riffHeader.id[1]) || ('F' != riffHeader.id[2]) || ('F' != riffHeader.id[3]) ||
      ('f' != fmtHeader.id[0]) || ('m' != fmtHeader.id[1]) || ('t' != fmtHeader.id[2]) || (' ' != fmtHeader.id[3]))
  {
    // error!
    return EGE_ERROR;
  }
  
  // store data
  channels        = fmtHeader.channels;
  sampleRate      = fmtHeader.sampleRate;
  bitsPerSample   = fmtHeader.bitsPerSample;
  soundDataOffset = sizeof (WaveRiffHeader) + 8 + fmtHeader.size + 8;     // + 8 first 2 entires of WaveFmtHeader, + 8 first 2 entries of DATA header

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
