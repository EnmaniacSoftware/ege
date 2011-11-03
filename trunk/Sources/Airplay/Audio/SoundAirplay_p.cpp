#include "Core/Application/Application.h"
#include "Airplay/Audio/SoundAirplay_p.h"
#include <s3eSound.h>
#include <s3e.h>
#include <EGEDebug.h>
#include <EGEAudio.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(SoundPrivate)
EGE_DEFINE_DELETE_OPERATORS(SoundPrivate)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define BUFFERS_COUNT 3
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundPrivate::SoundPrivate(Sound* base) : m_d(base),
                                          m_done(false)
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
  if (m_done)
  {
    // nothing to update as there is no more data
    return;
  }

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

    // upload 250ms audio data to buffer
    EGE_PRINT("Uploading sound buffer");
    buffer->clear();
    m_done = codec->decode(buffer, codec->frequency() >> 2, samplesDecoded);

    // add to the end of the buffer list
    m_buffers.push_back(buffer);
  }

  // clean up
  emptyBuffers.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
