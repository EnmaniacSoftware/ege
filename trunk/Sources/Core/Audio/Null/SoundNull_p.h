#ifndef EGE_CORE_SOUND_NULL_PRIVATE_H
#define EGE_CORE_SOUND_NULL_PRIVATE_H

#ifdef EGE_AUDIO_NULL

#include <EGE.h>
#include <EGETime.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Sound, PSound)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SoundPrivate
{
  public:

    SoundPrivate(Sound* base);
   ~SoundPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Constructs object. */
    EGEResult construct();
    /* Updates object. */
    void update(const Time& time);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_AUDIO_NULL

#endif // EGE_CORE_SOUND_NULL_PRIVATE_H