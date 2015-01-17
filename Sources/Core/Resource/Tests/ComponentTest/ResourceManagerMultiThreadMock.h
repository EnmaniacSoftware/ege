#ifndef EGE_CORE_RESOURCE_TESTS_COMPONENTTEST_RESOURCEMANAGERMULTITHREAD_MOCK_H
#define EGE_CORE_RESOURCE_TESTS_COMPONENTTEST_RESOURCEMANAGERMULTITHREAD_MOCK_H

#include <gmock/gmock.h>
#include "Core/Resource/Interface/MultiThread/ResourceManagerMultiThread.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceManagerMultiThreadMock : public ResourceManagerMultiThread
{
  public:

    ResourceManagerMultiThreadMock(Engine& engine, IResourceLoader& loader) : ResourceManagerMultiThread(engine, loader) {}

    MOCK_METHOD0(createDefaultResources, bool ());
  //  MOCK_METHOD0(destroyDefaultResources, void ());
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCE_TESTS_COMPONENTTEST_RESOURCEMANAGERMULTITHREAD_MOCK_H