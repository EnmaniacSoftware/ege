#include "Core/Resource/Interface/Loader/ResourceLoaderFactory.h"
#include "Core/Resource/Interface/Loader/ResourceLoaderXML.h"
#include "EGEDebug.h"
#include "EGEResourceLoader.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* KResourceLoaderFactoryDebugName = "EGEResourceLoaderFactory";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceLoaderXMLCreateFunctor : public Factory<IResourceLoader*>::CreateInstanceFunctor
{
  public:

    ResourceLoaderXMLCreateFunctor(Engine& engine) : m_engine(engine) {}

    IResourceLoader* operator()() const override { return ege_new ResourceLoaderXML(m_engine); }

  private:

    Engine& m_engine;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceLoaderFactory)
EGE_DEFINE_DELETE_OPERATORS(ResourceLoaderFactory)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLoaderFactory::ResourceLoaderFactory(Engine& engine) 
: Factory<IResourceLoader*>(engine)
{
  // register default interfaces
  if (EGE_SUCCESS != registerInterface(KResourceLoaderXMLName, ege_new ResourceLoaderXMLCreateFunctor(engine)))
  {
    egeWarning(KResourceLoaderFactoryDebugName) << "Could not register default ResourceLoader interfaces!";
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLoaderFactory::~ResourceLoaderFactory()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

