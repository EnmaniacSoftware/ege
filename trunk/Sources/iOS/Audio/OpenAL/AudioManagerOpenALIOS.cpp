#include "Core/Application/Application.h"
#include "iOS/Audio/OpenAL/AudioManagerOpenALIOS.h"
#include "iOS/Audio/OpenAL/SoundOpenALIOS.h"
#include "EGEDebug.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(AudioManagerOpenALIOS)
EGE_DEFINE_DELETE_OPERATORS(AudioManagerOpenALIOS)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerOpenALIOS::AudioManagerOpenALIOS(Application* app) : AudioManagerOpenAL(app)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerOpenALIOS::~AudioManagerOpenALIOS()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PSound AudioManagerOpenALIOS::createSound(const String& name, PDataBuffer& data) const
{
  SoundOpenALIOS* object = ege_new SoundOpenALIOS(const_cast<AudioManagerOpenALIOS*>(this), name, data);
  if ((NULL == object) || (EGE_SUCCESS != object->construct()))
  {
    // error!
    object = NULL;
  }

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------