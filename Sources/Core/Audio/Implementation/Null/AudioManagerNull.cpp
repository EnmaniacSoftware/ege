#include "Core/Audio/Interface/Null/AudioManagerNull.h"
#include "Core/Audio/Implementation/Null/SoundNull.h"
#include "Core/Audio/Interface/Sound.h"
#include "EGEEvent.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(AudioManagerNull)
EGE_DEFINE_DELETE_OPERATORS(AudioManagerNull)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerNull::AudioManagerNull(Engine& engine) : Object()
                                                   , IAudioManagerBase()
                                                   , IAudioManager()
                                                   , m_state(IAudioManager::StateNone)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerNull::~AudioManagerNull()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult AudioManagerNull::construct()
{
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerNull::update(const Time& time)
{
  EGE_UNUSED(time)
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult AudioManagerNull::requestPlay(PSound sound)
{
  EGE_UNUSED(sound);
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerNull::requestPause(PSound sound)
{
  EGE_UNUSED(sound);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerNull::requestStop(PSound sound)
{
  EGE_UNUSED(sound)
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AudioManagerNull::setEnable(bool set)
{
  EGE_UNUSED(set)
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AudioManagerNull::isEnabled() const
{
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PSound AudioManagerNull::createSound(const String& name, PDataBuffer& data) const
{
  Sound* object = ege_new SoundNull(name, data);
  if ((NULL == object) || (EGE_SUCCESS != object->construct()))
  {
    // error!
    object = NULL;
  }

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IAudioManager::EState AudioManagerNull::state() const
{
  return m_state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END