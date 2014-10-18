#include "Core/Resource/Interface/ResourceManagerFactory.h"
#include "Core/Resource/Interface/MultiThread/ResourceManagerMultiThread.h"
#include "Core/Resource/Interface/SingleThread/ResourceManagerSingleThread.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* KResourceManagerFactoryDebugName = "EGEResourceManagerFactory";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static EngineModule<IResourceManager>* CreateSingleThreadInstance(Engine& engine)
{
  return ege_new ResourceManagerSingleThread(engine);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static EngineModule<IResourceManager>* CreateMultiThreadInstance(Engine& engine)
{
  return ege_new ResourceManagerMultiThread(engine);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceManagerFactory)
EGE_DEFINE_DELETE_OPERATORS(ResourceManagerFactory)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManagerFactory::ResourceManagerFactory(Engine& engine) 
: Factory<EngineModule<IResourceManager>*, ResourceManagerCreateFunc>(engine)
{
  // register default interfaces
  if ((EGE_SUCCESS != registerInterface(KResourceManagerSingleThreadName, CreateSingleThreadInstance)) ||
      (EGE_SUCCESS != registerInterface(KResourceManagerMultiThreadName, CreateMultiThreadInstance)))
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

