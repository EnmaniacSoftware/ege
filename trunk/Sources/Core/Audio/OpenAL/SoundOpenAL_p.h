#ifndef EGE_CORE_SOUND_OPENAL_H
#define EGE_CORE_SOUND_OPENAL_H

#ifdef EGE_AUDIO_OPENAL

#include <EGE.h>
#include <EGEDataBuffer.h>
#include <al.h>
#include <alc.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define BUFFERS_COUNT 3
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Sound;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class SoundPrivate
{
  public:

    SoundPrivate(Sound* base);
   ~SoundPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(Sound)

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Updates buffers. This is called by AudioManagerPrivate. 
     * @param channel OpenAL channel (source) id for which buffers should be updated.
     */
    void updateBuffers(ALuint channel);

  private:

    /*! OpenAL sound buffer objects. */
    ALuint m_buffers[BUFFERS_COUNT];
    /*! OpenAL buffer format. */
    ALenum m_format;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_AUDIO_OPENAL

#endif // EGE_CORE_SOUND_OPENAL_H