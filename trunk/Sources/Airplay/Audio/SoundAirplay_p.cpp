#include "Core/Application/Application.h"
#include "Core/Audio/AudioUtils.h"
#include "Airplay/Audio/SoundAirplay_p.h"
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(SoundPrivate)
EGE_DEFINE_DELETE_OPERATORS(SoundPrivate)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundPrivate::SoundPrivate(Sound* base, const PDataBuffer& data) : m_d(base),
                                                                   m_data(data), 
                                                                   m_channels(-1), 
                                                                   m_sampleRate(-1), 
                                                                   m_bitsPerSample(-1), 
                                                                   m_soundDataOffset(0),
                                                                   m_soundDataSize(0),
                                                                   m_samplesPlayed(0),
                                                                   m_samplesCount(0)
{
  // check if WAV file
  if (EGE_SUCCESS != AudioUtils::GetWavData(data, m_soundDataOffset, m_soundDataSize, m_channels, m_sampleRate, m_bitsPerSample))
  {
    // check other formats
    // ...
  }

  // calculate number of all samples
  m_samplesCount = m_soundDataSize / (m_channels * (m_bitsPerSample >> 3));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundPrivate::~SoundPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool SoundPrivate::isValid() const
{
  return (NULL != m_data) && (-1 != m_channels) && (-1 != m_sampleRate) && (-1 != m_bitsPerSample) && (0 < m_soundDataSize);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets number of played samples. */
void SoundPrivate::setSamplesPlayed(s32 count)
{
  m_samplesPlayed = count;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
