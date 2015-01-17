#ifndef EGE_CORE_RESOURCE_TESTS_COMPONENTTEST_RESOURCEMANAGERSINGLETHREAD_MOCK_H
#define EGE_CORE_RESOURCE_TESTS_COMPONENTTEST_RESOURCEMANAGERSINGLETHREAD_MOCK_H

#include <gmock/gmock.h>
#include "Core/Resource/Interface/SingleThread/ResourceManagerSingleThread.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceManagerSingleThreadMock : public ResourceManagerSingleThread
{
  public:

    ResourceManagerSingleThreadMock(Engine& engine, IResourceLoader& loader) : ResourceManagerSingleThread(engine, loader) {}

    MOCK_METHOD0(createDefaultResources, bool ());
  //  MOCK_METHOD0(destroyDefaultResources, void ());
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCE_TESTS_COMPONENTTEST_RESOURCEMANAGERSINGLETHREAD_MOCK_H