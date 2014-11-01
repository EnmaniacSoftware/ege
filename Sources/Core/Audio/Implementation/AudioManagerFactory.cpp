#include "Core/Audio/Interface/AudioManagerFactory.h"
#include "Core/Audio/Interface/Null/AudioManagerNull.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* KAudioManagerFactoryDebugName = "EGEAudioManagerFactory";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static EngineModule<IAudioManager>* CreateAudioManagerNull(Engine& engine)
{
  return ege_new AudioManagerNull(engine);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(AudioManagerFactory)
EGE_DEFINE_DELETE_OPERATORS(AudioManagerFactory)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerFactory::AudioManagerFactory(Engine& engine) 
: Factory<EngineModule<IAudioManager>*>(engine)
{
  // register default interfaces
  if (EGE_SUCCESS != registerInterface(KDefaultAudioManagerName, CreateAudioManagerNull))
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

