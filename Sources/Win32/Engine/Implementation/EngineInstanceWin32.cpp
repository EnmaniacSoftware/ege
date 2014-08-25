#include "Core/Audio/Interface/AudioManagerFactory.h"
#include "Core/Audio/Interface/OpenAL/AudioManagerOpenAL.h"
#include "Win32/Engine/Interface/EngineInstanceWin32.h"
#include "Win32/Services/Interface/SocialServicesWin32.h"
#include "EGEDictionary.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EngineInstanceWin32::EngineInstanceWin32()
: EngineInstance()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EngineInstanceWin32::~EngineInstanceWin32()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult EngineInstanceWin32::construct(const Dictionary& configParamDictionary)
{
  EGEResult result;

  // make local copy for modifications
  Dictionary specificConfigParamDictionary(configParamDictionary);

  // register audio interfaces
  result = audioManagerFactory()->registerInterface(KOpenALAudioManagerName, AudioManagerOpenAL::Create);
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

  // create social services
  m_socialServices = ege_new SocialServicesWin32(*this);

  // call base class
  return (EGE_SUCCESS == result) ? EngineInstance::construct(specificConfigParamDictionary) : result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END