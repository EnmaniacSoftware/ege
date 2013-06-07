#ifndef EGE_CORE_AUDIO_NULL_AUDIOMANAGERNULL_H
#define EGE_CORE_AUDIO_NULL_AUDIOMANAGERNULL_H

/** This class represents NULL audio manager.
 */

#include "EGE.h"
#include "EGETime.h"
#include "Core/Audio/AudioManagerBase.h"
#include "Core/Audio/AudioManager.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Sound, PSound)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AudioManagerNull : public Object, public IAudioManagerBase, public IAudioManager
{
  public:
    
    AudioManagerNull(Application* app);
   ~AudioManagerNull();

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

  private:

    /*! Current state. */
    IAudioManager::EState m_state;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIO_NULL_AUDIOMANAGERNULL_H