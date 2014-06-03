#ifndef EGE_IOS_AUDIO_OPENAL_SOUNDOPENALIOS_H
#define EGE_IOS_AUDIO_OPENAL_SOUNDOPENALIOS_H

#include "EGE.h"
#include "Core/Audio/Implementation/OpenAL/SoundOpenAL.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
@class SoundDelegate;
@class AVAudioPlayer;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

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

    /*! @see SoundOpenAL::construct. */
    EGEResult construct() override;
    /*! @see SoundOpenAL::setState. */
    void setState(State state) override;

  private:

    /*! @see SoundOpenAL::update. */
    void update(const Time& time) override;
    /*! @see SoundOpenAL::doPlay. */
    bool doPlay(ALuint channel) override;
    /*! @see SoundOpenAL::doStop. */
    bool doStop() override;
    /*! @see SoundOpenAL::doPause. */
    bool doPause() override;
    /*! @see SoundOpenAL::doResume. */
    bool doResume() override;
    /*! @see SoundOpenAL::setVolume. */
    void setVolume(float32 volume) override;
  
  private:
  
    /*! Audio player. */
    AVAudioPlayer* m_audioPlayer;
    /*! Delegate. */
    SoundDelegate* m_delegate;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_IOS_AUDIO_OPENAL_SOUNDOPENALIOS_H