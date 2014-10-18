#include "Core/Resource/Interface/ResourceManagerFactory.h"
#include "Core/Resource/Interface/MultiThread/ResourceManagerMultiThread.h"
#include "Core/Resource/Interface/SingleThread/ResourceManagerSingleThread.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* KResourceManagerFactoryDebugName = "EGEResourceManagerFactory";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceManagerFactory)
EGE_DEFINE_DELETE_OPERATORS(ResourceManagerFactory)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManagerFactory::ResourceManagerFactory(Engine& engine) 
: Factory<EngineModule<IResourceManager>*, ResourceManagerCreateFunc>(engine)
{
  // register default interfaces
  if ((EGE_SUCCESS != registerInterface(KResourceManagerSingleThreadName, ResourceManagerSingleThread::Create)) ||
      (EGE_SUCCESS != registerInterface(KResourceManagerMultiThreadName, ResourceManagerMultiThread::Create)))
  {
    egeWarning(KResourceManagerFactoryDebugName) << "Could not register default ResourceManager interfaces!";
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManagerFactory::~ResourceManagerFactory()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

