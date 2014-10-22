#include "Core/Audio/Interface/AudioManagerFactory.h"
#include "Core/Services/Interface/AdNetworkRegistry.h"
#include "iOS/Audio/Interface/OpenAL/AudioManagerOpenALIOS.h"
#include "iOS/Engine/Interface/EngineInstanceIOS.h"
#include "iOS/Services/Interface/SocialServicesIOS.h"
#include "iOS/Services/Interface/AdNetworkChartboost.h"
#include "EGEDictionary.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AudioManagerOpenALCreateFunctor : public Factory<EngineModule<IAudioManager>*>::CreateInstanceFunctor
{
  public:
  
    AudioManagerOpenALCreateFunctor(Engine& engine) : m_engine(engine) {}
  
    EngineModule<IAudioManager>* operator()() const override { return ege_new AudioManagerOpenALIOS(m_engine); }
  
  private:
  
    Engine& m_engine;
};

class AdNetworkChartboostCreateFunctor : public Factory<AdNetwork*>::CreateInstanceFunctor
{
  public:
  
    AdNetworkChartboostCreateFunctor(Engine& engine) : m_engine(engine) {}
  
    AdNetwork* operator()() const override { return ege_new AdNetworkChartboost(m_engine); }
  
  private:
  
    Engine& m_engine;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EngineInstanceIOS::EngineInstanceIOS()
: EngineInstance()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EngineInstanceIOS::~EngineInstanceIOS()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult EngineInstanceIOS::construct(const Dictionary& configParamDictionary)
{
  EGEResult result;

  // make local copy for modifications
  Dictionary specificConfigParamDictionary(configParamDictionary);

  // register audio interfaces
  result = audioManagerFactory()->registerInterface(KOpenALAudioManagerName, ege_new AudioManagerOpenALCreateFunctor(*this));
  if (EGE_SUCCESS != result)
  {
    egeWarning(KEngineDebugName) << "Could not register AudioManagerOpenAL instance!";
  }
  
  if (EGE_SUCCESS == result)
  {
    // setup audio manager type
    if ( ! specificConfigParamDictionary.contains(KConfigParamAudioManagerTypeName) && audioManagerFactory()->isInterfaceRegistered(KOpenALAudioManagerName))
    {
      specificConfigParamDictionary.insert(KConfigParamAudioManagerTypeName, KOpenALAudioManagerName);
    }
  }

  // register AdNetwork interfaces
  result = adNetworkRegistry()->registerInterface(KChartboostAdNetworkName, ege_new AdNetworkChartboostCreateFunctor(*this));
  if (EGE_SUCCESS != result)
  {
    egeWarning(KEngineDebugName) << "Could not register AdNetworkChartboost instance!";
  }
  
  // create social services
  m_socialServices = ege_new SocialServicesIOS(*this);

  // call base class
  return (EGE_SUCCESS == result) ? EngineInstance::construct(specificConfigParamDictionary) : result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END
