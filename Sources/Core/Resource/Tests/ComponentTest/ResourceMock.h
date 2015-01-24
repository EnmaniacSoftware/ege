#ifndef EGE_CORE_RESOURCE_TESTS_COMPONENTTEST_RESOURCE_MOCK_H
#define EGE_CORE_RESOURCE_TESTS_COMPONENTTEST_RESOURCE_MOCK_H

#include "Core/Resource/Interface/Resource.h"
#include <EGEXml.h>
#include <EGEString.h>
#include <gmock/gmock.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceGroup;

EGE_DECLARE_SMART_CLASS(ResourceData, PResourceData)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceMock : public IResource
{
  public:

    ResourceMock(Engine& engine, ResourceGroup* group);
    virtual ~ResourceMock();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    static const char* KResourceMockName;

  public:

    static IResource* CreateResource(Engine& engine, ResourceGroup* group);

  public:

    MOCK_CONST_METHOD0(name, const String& ());
    
    MOCK_METHOD2(create, EGEResult (const String&, const PObject&));
    MOCK_METHOD0(load, EGEResult ());
    MOCK_METHOD0(unload, void ());

  public:

    /*! Sets state to loaded. 
     *  @note Google Mock requires that this method has to have the same return value as the method for which it is to be called: load().
     */
    EGEResult setLoaded();
    /*! Sets state to unloded. 
     *  @note Google Mock requires that this method has to have the same return value as the method for which it is to be called: unload().
     */
    void setUnloaded();
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCE_TESTS_COMPONENTTEST_RESOURCE_MOCK_H
