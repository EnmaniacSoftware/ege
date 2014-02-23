#ifndef EGE_CORE_AUDIOUTILS_H
#define EGE_CORE_AUDIOUTILS_H

#include "EGE.h"
#include "EGEDataBuffer.h"
#include "EGEAudio.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AudioUtils
{
  public:

    /*! Detects stream type of given data. */
    static Audio::StreamType DetectStreamType(const PDataBuffer& data);
    /*! Retrieves WAV stream headers. */
    static void ReadWavHeaders(const PDataBuffer& data, Audio::WaveRiffHeader& riffHeader, Audio::WaveFmtHeader& fmtHeader, Audio::WaveDataHeader& dataHeader);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIOUTILS_H