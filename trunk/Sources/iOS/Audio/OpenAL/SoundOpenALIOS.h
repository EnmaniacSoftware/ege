#ifndef EGE_IOS_AUDIO_OPENAL_SOUNDOPENALIOS_H
#define EGE_IOS_AUDIO_OPENAL_SOUNDOPENALIOS_H

#include "EGE.h"
#include "Core/Audio/OpenAL/SoundOpenAL.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AudioManagerOpenALIOS;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SoundOpenALIOS : public SoundOpenAL
{
  public:

    SoundOpenALIOS(AudioManagerOpenALIOS* manager, const String& name, PDataBuffer& data);
   ~SoundOpenALIOS();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! @see SoundOpenAL::doPlay. */
    bool doPlay(ALuint channel) override;
    /*! @see SoundOpenAL::doStop. */
    bool doStop() override;
    /*! @see SoundOpenAL::doPause. */
    bool doPause() override;
    /*! @see SoundOpenAL::doResume. */
    bool doResume() override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_IOS_AUDIO_OPENAL_SOUNDOPENALIOS_H