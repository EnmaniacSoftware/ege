#ifndef EGE_CORE_TESTFRAMEWORK_RESOURCEFACTORY_MOCK_H
#define EGE_CORE_TESTFRAMEWORK_RESOURCEFACTORY_MOCK_H

#include <gmock/gmock.h>
#include <EGEResourceFactory.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceFactoryMock : public ResourceFactory
{
  public:

    ResourceFactoryMock(Engine& engine) : ResourceFactory(engine) {}

    MOCK_METHOD2(registerInterface, EGEResult (const String&, egeFactoryCreateFunction));
    MOCK_CONST_METHOD2(createInstance, IResource* (const String&, ResourceGroup*));
    MOCK_CONST_METHOD1(isInterfaceRegistered, const String& (bool));
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_CORE_TESTFRAMEWORK_RESOURCEFACTORY_MOCK_H