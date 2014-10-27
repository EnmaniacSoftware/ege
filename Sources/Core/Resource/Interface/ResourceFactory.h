#ifndef EGE_CORE_RESOURCE_RESOURCEFACTORY_H
#define EGE_CORE_RESOURCE_RESOURCEFACTORY_H

/*! Factory for resouces.
 */

#include "EGEEngine.h"
#include "EGEResource.h"
#include "Core/Factory/Interface/Factory.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceFactory : public Factory1<IResource*, ResourceGroup*>
{
  public:

    ResourceFactory(Engine& engine);
    virtual ~ResourceFactory();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCE_RESOURCEFACTORY_H