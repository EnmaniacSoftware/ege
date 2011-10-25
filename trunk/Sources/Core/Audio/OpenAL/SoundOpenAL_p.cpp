#include "Core/Audio/OpenAL/SoundOpenAL_p.h"
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(SoundPrivate)
EGE_DEFINE_DELETE_OPERATORS(SoundPrivate)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundPrivate::SoundPrivate(Sound* base, const PDataBuffer& data) : m_buffer(0)
{
  alGenBuffers(1, &m_buffer);
  if (AL_NO_ERROR != alGetError())
  {
    // error!
    EGE_PRINT("SoundPrivate::SoundPrivate - could not generate audio buffer.");
    m_buffer = 0;
  }
  else
  {
	  // Place the audio data into the new buffer
	  alBufferData(m_buffer, AL_FORMAT_STEREO16, data->data(), static_cast<ALsizei>(data->size()), 22000);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundPrivate::~SoundPrivate()
{
  if (0 != m_buffer)
  {
    alDeleteBuffers(1, &m_buffer);
    m_buffer = 0;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool SoundPrivate::isValid() const
{
  return 0 != m_buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
