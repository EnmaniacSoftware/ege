#ifdef EGE_AUDIO_OPENAL
#include "Core/Application/Application.h"
#include "Core/Audio/OpenAL/SoundOpenAL_p.h"
#include "Core/Audio/AudioUtils.h"
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(SoundPrivate)
EGE_DEFINE_DELETE_OPERATORS(SoundPrivate)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundPrivate::SoundPrivate(Sound* base, const PDataBuffer& data) : m_buffer(0)
{
  s32 channels        = -1;
  s32 sampleRate      = -1;
  s32 bitsPerSample   = -1;
  s32 soundDataOffset = 0;
  s32 soundDataSize   = 0;

  // check if WAV file
  if (EGE_SUCCESS != AudioUtils::GetWavData(data, soundDataOffset, soundDataSize, channels, sampleRate, bitsPerSample))
  {
    // check other formats
    // ...

  }

  // check if we detected supported format
  if ((-1 != channels) && (-1 != sampleRate) && (-1 != bitsPerSample) && (0 < soundDataSize))
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
      ALenum format = AL_FORMAT_STEREO16;
      if (8 == bitsPerSample)
      {
        format = (1 == channels) ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
      }
      else if (16 == bitsPerSample)
      {
        format = (1 == channels) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;        
      }
      else
      {
        EGE_ASSERT("Invalid bits per sample!");
      }

	    alBufferData(m_buffer, format, data->data(soundDataOffset), soundDataSize, sampleRate);   
      if (AL_NO_ERROR != alGetError())
      {
        // error!
        EGE_PRINT("SoundPrivate::SoundPrivate - could not define data buffer.");
        m_buffer = 0;
      }
    }
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
#endif // EGE_AUDIO_OPENAL