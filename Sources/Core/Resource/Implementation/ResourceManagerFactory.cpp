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
class SingleThreadCreateFunctor : public Factory<EngineModule<IResourceManager>*>::CreateInstanceFunctor
{
  public:

    SingleThreadCreateFunctor(Engine& engine, IResourceLoader& loader) : m_engine(engine), m_loader(loader) {}

    EngineModule<IResourceManager>* operator()() const override { return ege_new ResourceManagerSingleThread(m_engine, m_loader); }

  private:

    Engine& m_engine;
    IResourceLoader& m_loader;
};

class MultiThreadCreateFunctor : public Factory<EngineModule<IResourceManager>*>::CreateInstanceFunctor
{
  public:

    MultiThreadCreateFunctor(Engine& engine, IResourceLoader& loader) : m_engine(engine), m_loader(loader) {}

    EngineModule<IResourceManager>* operator()() const override { return ege_new ResourceManagerMultiThread(m_engine, m_loader); }

  private:

    Engine& m_engine;
    IResourceLoader& m_loader;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceManagerFactory)
EGE_DEFINE_DELETE_OPERATORS(ResourceManagerFactory)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManagerFactory::ResourceManagerFactory(Engine& engine) 
: Factory<EngineModule<IResourceManager>*>(engine)
, m_loaderFactory(NULL)
{
  // create loader factory
  m_loaderFactory = ege_new ResourceLoaderFactory(engine);

  // create resource loader object
  const String typeName = engine.configurationDictionary().value(KConfigParamResourceLoaderTypeName, KResourceLoaderXMLName);
  m_resourceLoader = m_loaderFactory->createInstance(typeName);

  // register default interfaces
  if ((EGE_SUCCESS != registerInterface(KResourceManagerSingleThreadName, ege_new SingleThreadCreateFunctor(engine, *m_resourceLoader))) ||
      (EGE_SUCCESS != registerInterface(KResourceManagerMultiThreadName, ege_new MultiThreadCreateFunctor(engine, *m_resourceLoader))))
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

EGE_NAMESPACE_END

