#ifndef EGE_CORE_SOUND_OPENAL_H
#define EGE_CORE_SOUND_OPENAL_H

#ifdef EGE_AUDIO_OPENAL

#include <EGE.h>
#include <EGEDataBuffer.h>
#include <al.h>
#include <alc.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Sound;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class SoundPrivate
{
  public:

    SoundPrivate(Sound* base, const PDataBuffer& data);
   ~SoundPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /*! Returns OpenAL sound buffer object. */
    inline ALuint bufferId() const { return m_buffer; }

  private:

    /*! OpenAL sound buffer obejct. */
    ALuint m_buffer;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_AUDIO_OPENAL

#endif // EGE_CORE_SOUND_OPENAL_H