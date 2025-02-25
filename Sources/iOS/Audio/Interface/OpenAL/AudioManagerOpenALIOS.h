#ifndef EGE_IOS_AUDIO_OPENAL_AUDIOMANAGEROPENALIOS_H
#define EGE_IOS_AUDIO_OPENAL_AUDIOMANAGEROPENALIOS_H

/** This class represents OpenAL audio manager for iOS.
 */

#include "EGE.h"
#include "EGEString.h"
#include "EGEDataBuffer.h"
#include "Core/Audio/Interface/OpenAL/AudioManagerOpenAL.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AudioManagerOpenALIOS : public AudioManagerOpenAL
{
  public:
    
    AudioManagerOpenALIOS(Application* app);
   ~AudioManagerOpenALIOS();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! @see AudioManagerOpenAL::createSound. */
    PSound createSound(const String& name, PDataBuffer& data) const override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_IOS_AUDIO_OPENAL_AUDIOMANAGEROPENALIOS_H