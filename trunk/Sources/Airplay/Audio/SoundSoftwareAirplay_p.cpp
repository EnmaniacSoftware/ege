#if EGE_AUDIO_AIRPLAY && EGE_AIRPLAY_AUDIO_SOFTWARE

#include "Core/Application/Application.h"
#include "Core/Audio/wsfir.h"
#include "Airplay/Audio/SoundSoftwareAirplay_p.h"
#include <s3eSound.h>
#include <s3e.h>
#include <EGEDebug.h>
#include <EGEAudio.h>
#include <EGEDevice.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(SoundPrivate)
EGE_DEFINE_DELETE_OPERATORS(SoundPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define BUFFERS_COUNT               3
#define FILTER_COEFFICIENTS_COUNT 129
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundPrivate::SoundPrivate(Sound* base) : m_d(base),
                                          m_done(false),
                                          m_buffersLocked(false),
                                          m_audioManager(NULL),
                                          m_channelId(-1)
{
  // allocate buffers
  for (s32 i = 0; i < BUFFERS_COUNT; ++i)
  {
    PDataBuffer buffer = ege_new DataBuffer();
    if (buffer)
    {
      m_buffers.push_back(buffer);
    }
  }

  m_filterBuffer.reserve(FILTER_COEFFICIENTS_COUNT);
  m_filterCoefficients.reserve(FILTER_COEFFICIENTS_COUNT);

  for (s32 i = 0; i < FILTER_COEFFICIENTS_COUNT; ++i)
  {
    m_filterBuffer[i] = 0;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundPrivate::~SoundPrivate()
{
  m_buffers.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool SoundPrivate::isValid() const
{
  return BUFFERS_COUNT == m_buffers.size();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates buffers. This is called by AudioManagerPrivate. */
void SoundPrivate::updateBuffers()
{
  if (m_done || m_buffersLocked)
  {
    // nothing to update as there is no more data
    return;
  }

  // lock
  lockBuffers();

  AudioCodec* codec = d_func()->codec();

  s32 samplesDecoded = 0;

  // find empty buffers, remove them from main list and add into empty one for refilling
  BuffersList emptyBuffers;
  for (BuffersList::iterator it = m_buffers.begin(); it != m_buffers.end();)
  {
    const PDataBuffer& buffer = *it;

    // check if list is empty
    if (buffer->readOffset() == buffer->size())
    {
      // add it into empty buffers list first
      emptyBuffers.push_back(buffer);

      // remove from main list
      it = m_buffers.erase(it);
    }
    else
    {
      // go to next
      ++it;
    }
  }

  // go thru all empty buffers and refill them
  for (BuffersList::iterator it = emptyBuffers.begin(); it != emptyBuffers.end(); ++it)
  {
    PDataBuffer& buffer = *it;

    // upload 125ms audio data to buffer
    EGE_PRINT("Uploading sound buffer");
    buffer->clear();
    m_done = codec->decode(buffer, codec->frequency() >> 2, samplesDecoded);

    // add to the end of the buffer list
    m_buffers.push_back(buffer);
  }

  // clean up
  emptyBuffers.clear();

  // unlock
  unlockBuffers();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Locks buffers. */
void SoundPrivate::lockBuffers()
{
  m_buffersLocked = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Unlocks buffers. */
void SoundPrivate::unlockBuffers()
{
  m_buffersLocked = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Initializes audio filter. */
void SoundPrivate::initializeFilter()
{
  const s32 fs = Math::Min(d_func()->codec()->frequency(), Device::AudioOutputFrequency());
  const float32 fc = (fs / 2) / static_cast<float32>(Device::AudioOutputFrequency()); // half the input sample rate (eg nyquist limit of input)

  // generate filter coefficients
  wsfirLP(m_filterCoefficients, FILTER_COEFFICIENTS_COUNT, W_BLACKMAN, fc);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets pointer to private implementation of audio manager through which playback is being done. */
void SoundPrivate::setAudioManagerPrivate(AudioManagerPrivate* manager)
{
  m_audioManager = manager;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets channel sound is being played on. */
void SoundPrivate::setChannel(s32 channelId)
{
  m_channelId = channelId;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_AUDIO_AIRPLAY && EGE_AIRPLAY_AUDIO_SOFTWARE