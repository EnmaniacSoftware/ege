#ifndef EGE_CORE_AUDIOUTILS_H
#define EGE_CORE_AUDIOUTILS_H

#include <EGE.h>
#include <EGEDataBuffer.h>
#include <EGEAudio.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AudioUtils
{
  public:

    /*! Detects stream type of given data. */
    static EGEAudio::StreamType DetectStreamType(const PDataBuffer& data);
    /*! Retrieves WAV stream headers. */
    static void ReadWavHeaders(const PDataBuffer& data, EGEAudio::WaveRiffHeader& riffHeader, EGEAudio::WaveFmtHeader& fmtHeader, 
                               EGEAudio::WaveDataHeader& dataHeader);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIOUTILS_H