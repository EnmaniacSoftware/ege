#ifndef EGE_AIRPLAY_AUDIO_AUDIOMANAGERAIRPLAY_H
#define EGE_AIRPLAY_AUDIO_AUDIOMANAGERAIRPLAY_H

/** This class represents Airplay implementation of audio manager.
 */

#include "EGE.h"
#include "EGETime.h"
#include "EGEAudio.h"
#include "Core/Audio/AudioManagerBase.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Sound, PSound)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AudioManagerAirplay : public Object, public IAudioManagerBase, public IAudioManager
{
  public:
    
    AudioManagerAirplay(Application* app);
   ~AudioManagerAirplay();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! @see IAudioManagerBase::requestPlay. */
    EGEResult requestPlay(PSound sound) override;
    /*! @see IAudioManagerBase::requestStop. */
    void requestStop(PSound sound) override;
    /*! @see IAudioManagerBase::requestPause. */    
    void requestPause(PSound sound) override;

    /*! @see IAudioManager::construct. */
    EGEResult construct() override;
    /*! @see IAudioManager::update. */
    void update(const Time& time) override;
    /*! @see IAudioManager::setEnabled. */
    void setEnable(bool set) override;
    /*! @see IAudioManager::isEnabled. */
    bool isEnabled() const override;
    /*! @see IAudioManager::createSound. */
    PSound createSound(const String& name, PDataBuffer& data) const override;
    /*! @see IAudioManager::state. */
    EState state() const override;

  private:

    /*! @ see IEventListener::onEventRecieved. */
    void onEventRecieved(PEvent event) override;
    /*! Shuts down. */
    void shutDown();

  private:

    /*! Current state. */
    IAudioManager::EState m_state;
    /*! List of sounds being played. */
    SoundList m_sounds;
    /*! Enable flag. */
    bool m_enabled;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_AIRPLAY_AUDIO_AUDIOMANAGERAIRPLAY_H