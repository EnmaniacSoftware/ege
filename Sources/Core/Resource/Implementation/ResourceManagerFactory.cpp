#include "Core/Resource/Interface/ResourceManagerFactory.h"
#include "Core/Resource/Interface/Loader/ResourceLoaderFactory.h"
#include "Core/Resource/Interface/Loader/ResourceLoaderXML.h"
#include "Core/Resource/Interface/MultiThread/ResourceManagerMultiThread.h"
#include "Core/Resource/Interface/SingleThread/ResourceManagerSingleThread.h"
#include "EGEDebug.h"
#include "EGEResourceLoader.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* KResourceManagerFactoryDebugName = "EGEResourceManagerFactory";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static EngineModule<IResourceManager>* CreateSingleThreadResourceManager(Engine& engine, IResourceLoader& loader)
{
  return ege_new ResourceManagerSingleThread(engine, loader);
}

static EngineModule<IResourceManager>* CreateMultipleThreadResourceManager(Engine& engine, IResourceLoader& loader)
{
  return ege_new ResourceManagerMultiThread(engine, loader);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceManagerFactory)
EGE_DEFINE_DELETE_OPERATORS(ResourceManagerFactory)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManagerFactory::ResourceManagerFactory(Engine& engine) 
: Factory1<EngineModule<IResourceManager>*, IResourceLoader&>(engine)
, m_loaderFactory(NULL)
{
  // create loader factory
  m_loaderFactory = ege_new ResourceLoaderFactory(engine);

  // create resource loader object
  const String typeName = engine.configurationDictionary().value(KConfigParamResourceLoaderTypeName, KResourceLoaderXMLName);
  m_resourceLoader = m_loaderFactory->createInstance(typeName);

  // register default interfaces
  if ((EGE_SUCCESS != registerInterface(KResourceManagerSingleThreadName, CreateSingleThreadResourceManager)) ||
      (EGE_SUCCESS != registerInterface(KResourceManagerMultiThreadName, CreateMultipleThreadResourceManager)))
  {
    egeWarning(KResourceManagerFactoryDebugName) << "Could not register default ResourceManager interfaces!";
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManagerFactory::~ResourceManagerFactory()
{
  EGE_DELETE(m_resourceLoader);
  EGE_DELETE(m_loaderFactory);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLoaderFactory* ResourceManagerFactory::loaderFactory() const
{
  return m_loaderFactory;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IResourceLoader& ResourceManagerFactory::loader() const
{
  return *m_resourceLoader;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

