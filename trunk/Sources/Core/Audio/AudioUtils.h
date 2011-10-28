#ifndef EGE_CORE_AUDIOUTILS_H
#define EGE_CORE_AUDIOUTILS_H

#include <EGE.h>
#include <EGEDataBuffer.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class AudioUtils
{
  public:

    /* Retrieves data from WAV file.
     * @param data            Data buffer containing wav file data.
     * @param soundDataOffset Offset (in bytes) to begining of sound data.
     * @param channels        Number of channels of wav file.
     * @param sampleRate      Sample rate of wav file.
     * @param bitsPerSample   Number of bits per sample of wav file.
     * @return  Returns success if given buffer contains valid wav file data and data has been retrieved successfully.
     */
    static EGEResult GetWavData(const PDataBuffer& data, s32& soundDataOffset, s32& channels, s32& sampleRate, s32& bitsPerSample);
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIOUTILS_H