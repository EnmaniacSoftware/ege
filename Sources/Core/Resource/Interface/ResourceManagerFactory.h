#ifndef EGE_CORE_RESOURCE_RESOURCEMANAGERFACTORY_H
#define EGE_CORE_RESOURCE_RESOURCEMANAGERFACTORY_H

/*! Factory for resource manager instances.
 */

#include "EGE.h"
#include "EGEEngine.h"
#include "Core/Engine/Interface/EngineModule.h"
#include "Core/Factory/Interface/Factory.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef EngineModule<IResourceManager>* (*ResourceManagerCreateFunc)(Engine& engine);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceManagerFactory : public Factory<EngineModule<IResourceManager>*, ResourceManagerCreateFunc>
{
  public:

    ResourceManagerFactory(Engine& engine);
    virtual ~ResourceManagerFactory();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCE_RESOURCEMANAGERFACTORY_H