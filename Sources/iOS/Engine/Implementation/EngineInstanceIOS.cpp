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
  result = audioManagerFactory()->registerInterface(KOpenALAudioManagerName, AudioManagerOpenALIOS::Create);
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
  result = adNetworkRegistry()->registerInterface(KChartboostAdNetworkName, AdNetworkChartboost::Create);
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
