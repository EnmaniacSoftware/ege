#include "Core/Audio/Interface/AudioManagerFactory.h"
#include "Core/Audio/Interface/Null/AudioManagerNull.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* KAudioManagerFactoryDebugName = "EGEAudioManagerFactory";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AudioManagerNullCreateFunctor : public Factory<EngineModule<IAudioManager>*>::CreateInstanceFunctor
{
  public:

    AudioManagerNullCreateFunctor(Engine& engine) : m_engine(engine) {}

    EngineModule<IAudioManager>* operator()() const override { return ege_new AudioManagerNull(m_engine); }

  private:

    Engine& m_engine;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(AudioManagerFactory)
EGE_DEFINE_DELETE_OPERATORS(AudioManagerFactory)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerFactory::AudioManagerFactory(Engine& engine) 
: Factory<EngineModule<IAudioManager>*>(engine)
{
  // register default interfaces
  if (EGE_SUCCESS != registerInterface(KDefaultAudioManagerName, ege_new AudioManagerNullCreateFunctor(engine)))
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

