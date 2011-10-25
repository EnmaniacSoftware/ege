#ifndef EGE_CORE_SOUND_PRIVATE_H
#define EGE_CORE_SOUND_PRIVATE_H

#include <EGE.h>
#include <EGEDataBuffer.h>
#include <al/al.h>
#include <al/alc.h>

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

  private:

    /*! OpenAL sound buffer obejct. */
    ALuint m_buffer;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SOUND_PRIVATE_H