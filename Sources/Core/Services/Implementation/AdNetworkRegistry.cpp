#include "Core/Services/Interface/AdNetworkRegistry.h"
#include "Core/Services/Implementation/AdNetworkNull.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* KAdNetworkRegistryDebugName = "EGEAdNetworkRegistry";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static AdNetwork* CreateAdNetworkNull(Engine& engine)
{
  return ege_new AdNetworkNull(engine);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(AdNetworkRegistry)
EGE_DEFINE_DELETE_OPERATORS(AdNetworkRegistry)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AdNetworkRegistry::AdNetworkRegistry(Engine& engine) 
: Factory<AdNetwork*>(engine)
{
  // register default interfaces
  if (EGE_SUCCESS != registerInterface(KDefaultAdNetworkName, CreateAdNetworkNull))
  {
    egeWarning(KAdNetworkRegistryDebugName) << "Could not register default AdNetwork interfaces!";
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AdNetworkRegistry::~AdNetworkRegistry()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END
