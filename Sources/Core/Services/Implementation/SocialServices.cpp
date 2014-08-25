#include "Core/Services/Interface/SocialServices.h"
#include "Core/Services/Interface/AdNetworkRegistry.h"
#include "Core/Engine/Interface/EngineInternal.h"
#include "Core/Engine/Interface/EngineInstance.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* KSocialServicesDebugName = "EGESocialServices";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(SocialServices)
EGE_DEFINE_DELETE_OPERATORS(SocialServices)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SocialServices::SocialServices(Engine& engine) 
: m_engine(engine)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SocialServices::~SocialServices()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Engine& SocialServices::engine() const
{
  return m_engine;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AdNetwork* SocialServices::adNetwork() const
{
  EngineInternal& engineInternal = static_cast<EngineInternal&>(reinterpret_cast<EngineInstance&>(engine()));

  return engineInternal.adNetwork();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END