#ifndef EGE_CORE_RESOURCE_TESTS_COMPONENTTEST_RESOURCEMANAGERTESTBASE_H
#define EGE_CORE_RESOURCE_TESTS_COMPONENTTEST_RESOURCEMANAGERTESTBASE_H

#include "TestFramework/Interface/TestBase.h"
#include "TestFramework/Interface/EngineMock.h"
#include "TestFramework/Interface/ResourceFactoryMock.h"
#include "Core/Resource/Interface/Loader/ResourceLoaderXML.h"
#include "ResourceMock.h"
#include <EGEString.h>
#include <EGEStringList.h>
#include <EGESignal.h>

#include <gmock/gmock.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceManagerTestBase : public TestBase
{
  public:

    ResourceManagerTestBase();
    virtual ~ResourceManagerTestBase();

  public:

    static const char* KResourceGroup1;
    static const char* KResourceNonExistingGroup;
    static const char* KResourceGroup3;
    static const s64 KDefaultGroupLoadTimeoutMs;
    static const s32 KGroupCount;
    static const s32 KGroup1ResourceCount;
    static const s32 KGroup3ResourceCount;

  public slots:

    /*! Slot called when group is successfully loaded.
     *  @param  groupName Name of the loaded group. 
     *  @param  result    Operation result.
     */
    void onGroupLoaded(const String& groupName, EGEResult result);
    /*! Slot called when group is successfully unloaded.
     *  @param  groupName Name of the unloaded group. 
     *  @param  result    Operation result.
     */
    void onGroupUnloaded(const String& groupName, EGEResult result);
    /*! Slot called when resource has been loaded/unloaded.
     *  @param processed Number of already processed resources
     *  @param total     Total number resources to process
     */
    void onProgress(s32 processed, s32 total);
    /*! Slot called when resource has been loaded/unloaded.
     *  @param processed Number of already processed resources
     *  @param total     Total number resources to process
     *  @note This schedules loading of the group 3 when called first time.
     */
    void onProgressLoadGroup3AfterFirstResource(s32 processed, s32 total);
    /*! Slot called when resource has been loaded/unloaded.
     *  @param processed Number of already processed resources
     *  @param total     Total number resources to process
     *  @note This schedules unloading of the group 1 when called first time.
     */
    void onProgressUnloadGroup1AfterFirstResource(s32 processed, s32 total);

    /*! Slot called when group is successfully loaded. Schedules loading of another group.
     *  @param  groupName Name of the loaded group. 
     *  @param  result    Operation result.
     */
    void onGroupLoadedAndLoadAnother(const String& groupName, EGEResult result);
    /*! Slot called when group is successfully loaded. Schedules unloading of group 1 if loaded group is 3.
     *  @param  groupName Name of the loaded group. 
     *  @param  result    Operation result.
     */
    void onGroupLoadedAndUnloadGroup1IfGroup3(const String& groupName, EGEResult result);
    /*! Slot called when group is successfully loaded. Schedules self unloading of if loaded group is 1.
     *  @param  groupName Name of the loaded group. 
     *  @param  result    Operation result.
     */
    void onGroupLoadedAndUnloadSelfIfGroup1(const String& groupName, EGEResult result);

    /*! Slot called when group is successfully unloaded. Schedules loading of another group.
     *  @param  groupName Name of the loaded group. 
     *  @param  result    Operation result.
     */
    void onGroupUnloadedAndLoadAnother(const String& groupName, EGEResult result);
    /*! Slot called when group is successfully unloaded. Schedules unloading of group 3 if unloaded group is 1.
     *  @param  groupName Name of the loaded group. 
     *  @param  result    Operation result.
     */
    void onGroupUnloadedAndUnloadGroup3IfGroup1(const String& groupName, EGEResult result);

  protected:

    static void SetUpTestCase();
    static void TearDownTestCase();

  protected:

    virtual void SetUp();
    virtual void TearDown();

    /*! Creates resource manager. */
    virtual IResourceManager* createResourceManager(Engine& engine, IResourceLoader& loader) = 0;

    /*! Returns resource manager interface. */
    IResourceManager* resourceManager() const;

    /*! Waits, updating the resource manager until all of the groups are loaded or given timeout passes.
     *  @param  groupNames  List of groups to be loaded.
     *  @param  timeoutM    Timeout (in ms) after which waiting is terminated.
     */
    virtual void waitUntilGroupsAreLoaded(const StringList& groupNames, s64 timeoutMs = KDefaultGroupLoadTimeoutMs);
    /*! Waits, updating the resource manager until all of the groups are unloaded or given timeout passes.
     *  @param  groupNames  List of groups to be unloaded.
     *  @param  timeoutM    Timeout (in ms) after which waiting is terminated.
     */
    virtual void waitUntilGroupsAreUnloaded(const StringList& groupNames, s64 timeoutMs = KDefaultGroupLoadTimeoutMs);

    /*! Sets up resource factory. 
     *  @return Returns array of all available resources.
     */
    std::vector<ResourceMock*> setupResourceFactory();

  protected:

    /*! Engine mock. */
    ::testing::StrictMock<EngineMock> m_engine;
    /*! Resource loader. */
    ResourceLoaderXML* m_loader;
    /*! Resource factory. 
     *  @note Cannot use any of the Mocking classes as they do not support non-const parameters in constructors.
     */
    ResourceFactoryMock* m_resourceFactory;
    /*! Resource manager. */
    IResourceManager* m_resourceManager;
    /*! List of recently loaded groups. */
    StringList m_loadedGroups;
    /*! List of recently unloaded groups. */
    StringList m_unloadedGroups;
    /*! Total progress. */
    s32 m_totalProgress;
    /*! Current progress. */
    s32 m_currentProgress;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCE_TESTS_COMPONENTTEST_RESOURCEMANAGERTESTBASE_H