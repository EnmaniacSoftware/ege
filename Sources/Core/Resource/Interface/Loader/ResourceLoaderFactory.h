#ifndef EGE_CORE_RESOURCE_RESOURCELOADERFACTORY_H
#define EGE_CORE_RESOURCE_RESOURCELOADERFACTORY_H

/*! Factory for resource loader instances.
 */

#include "EGE.h"
#include "EGEEngine.h"
#include "Core/Factory/Interface/Factory.h"
#include "Core/Resource/Interface/Loader/IResourceLoader.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceLoaderFactory : public Factory<IResourceLoader*>
{
  public:

    ResourceLoaderFactory(Engine& engine);
    virtual ~ResourceLoaderFactory();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCE_RESOURCELOADERFACTORY_H