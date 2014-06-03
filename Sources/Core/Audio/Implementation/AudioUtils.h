#ifndef EGE_CORE_AUDIO_AUDIOUTILS_H
#define EGE_CORE_AUDIO_AUDIOUTILS_H

#include "EGE.h"
#include "EGEDataBuffer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Available audio stream types. */
enum AudioStreamType
{
  AST_UNKNOWN = -1,
  AST_WAV,
  AST_OGG,
  AST_MP3
};

/*! Wave format RIFF header. */
struct AudioWavRiffHeader
{
  u32 id;                 /*!< RIFF or 0x52494646 big-endian. */
  s32 size;               /*!< This is size of entire file minus 8 bytes for id and size entires of this struct. */
  u32 format;             /*!< WAVE or 0x57415645 big-endian. */
};

/*! Wave format FMT header. */
struct AudioWavFormatHeader
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

/*! Wave format DATA header. */
struct AudioWavDataHeader
{
  u32 id;                 /*!< data or 0x64617461 big-endian. */
  s32 size;               /*!< This is size of sound data (in bytes). */
  u32 offset;             /*!< Offset to data (in bytes). */
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AudioUtils
{
  public:

    /*! Detects stream type of given data. */
    static AudioStreamType DetectStreamType(const PDataBuffer& data);
    /*! Retrieves WAV stream headers. */
    static void ReadWavHeaders(const PDataBuffer& data, AudioWavRiffHeader& riffHeader, AudioWavFormatHeader& fmtHeader, AudioWavDataHeader& dataHeader);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIO_AUDIOUTILS_H