#include "Core/Resource/Interface/Loader/ResourceLoaderFactory.h"
#include "Core/Resource/Interface/Loader/ResourceLoaderXML.h"
#include "EGEDebug.h"
#include "EGEResourceLoader.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* KResourceLoaderFactoryDebugName = "EGEResourceLoaderFactory";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static IResourceLoader* CreateResourceLoaderXML(Engine& engine)
{
  return ege_new ResourceLoaderXML(engine);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceLoaderFactory)
EGE_DEFINE_DELETE_OPERATORS(ResourceLoaderFactory)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLoaderFactory::ResourceLoaderFactory(Engine& engine) 
: Factory<IResourceLoader*>(engine)
{
  // register default interfaces
  if (EGE_SUCCESS != registerInterface(KResourceLoaderXMLName, CreateResourceLoaderXML))
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

