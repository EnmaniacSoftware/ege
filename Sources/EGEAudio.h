#ifndef EGE_AUDIO_H
#define EGE_AUDIO_H

#include "Core/Platform.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace Audio
{
  /*! Available stream types. */
  enum StreamType
  {
    ST_UNKNOWN = -1,
    ST_WAVE,
    ST_OGG,
    ST_MP3
  };

  /*! Wave format RIFF header. */
  struct WaveRiffHeader
  {
    u32 id;                 /*!< RIFF or 0x52494646 big-endian. */
    s32 size;               /*!< This is size of entire file minus 8 bytes for id and size entires of this struct. */
    u32 format;             /*!< WAVE or 0x57415645 big-endian. */
  };

  /*! Wave format FMT header. */
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

  /*! Wave format DATA header. */
  struct WaveDataHeader
  {
    u32 id;                 /*!< data or 0x64617461 big-endian. */
    s32 size;               /*!< This is size of sound data (in bytes). */
    u32 offset;             /*!< Offset to data (in bytes). */
  };
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Core/Audio/AudioUtils.h"
#include "Core/Audio/SoundEffect.h"
#include "Core/Audio/SoundEffectFadeOut.h"
#include "Core/Audio/SoundEffectFadeIn.h"
#include "Core/Audio/Sound.h"
#include "Core/Audio/AudioManager.h"
#include "Core/Audio/AudioCodec.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_AUDIO_H