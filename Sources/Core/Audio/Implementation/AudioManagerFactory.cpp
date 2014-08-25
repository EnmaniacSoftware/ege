#include "Core/Audio/Interface/AudioManagerFactory.h"
#include "Core/Audio/Interface/Null/AudioManagerNull.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* KAudioManagerFactoryDebugName = "EGEAudioManagerFactory";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(AudioManagerFactory)
EGE_DEFINE_DELETE_OPERATORS(AudioManagerFactory)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerFactory::AudioManagerFactory(Engine& engine) 
: Factory<IAudioManager*, AudioManagerCreateFunc>(engine)
{
  // register default interfaces
  if (EGE_SUCCESS != registerInterface(KDefaultAudioManagerName, AudioManagerNull::Create))
  {
    egeWarning(KAudioManagerFactoryDebugName) << "Could not register default AudioManager interfaces!";
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerFactory::~AudioManagerFactory()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

