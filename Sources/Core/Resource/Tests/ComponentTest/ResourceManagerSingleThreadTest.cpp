#include "Core/Resource/Tests/ComponentTest/ResourceManagerTestBase.h"
#include "Core/Resource/Tests/ComponentTest/ResourceManagerSingleThreadMock.h"
#include <EGEMemory.h>
#include <EGETimer.h>
#include <EGEResourceFactory.h>

#include <vector>

using namespace ::testing;

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceManagerSingleThreadTest : public ResourceManagerTestBase
{
  public:

    ResourceManagerSingleThreadTest();

  protected:

    /*! @see ResourceManagerTestBase::SetUp. */
    virtual void SetUp();
    /*! @see ResourceManagerTestBase::TearDown. */
    virtual void TearDown();

    /*! @see ResourceManagerTestBase::createResourceManager. */
    IResourceManager* createResourceManager(Engine& engine, IResourceLoader& loader) override;

    /*! @see ResourceManagerTestBase::waitUntilGroupsAreLoaded. */
    void waitUntilGroupsAreLoaded(const StringList& groupNames, s64 timeoutMs = KDefaultGroupLoadTimeoutMs) override;
    /*! @see ResourceManagerTestBase::waitUntilGroupsAreUnloaded. */
    void waitUntilGroupsAreUnloaded(const StringList& groupNames, s64 timeoutMs = KDefaultGroupLoadTimeoutMs) override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManagerSingleThreadTest::ResourceManagerSingleThreadTest()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerSingleThreadTest::SetUp()
{
  // call base class
  ResourceManagerTestBase::SetUp();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerSingleThreadTest::TearDown()
{
  // call base class
  ResourceManagerTestBase::TearDown();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IResourceManager* ResourceManagerSingleThreadTest::createResourceManager(Engine& engine, IResourceLoader& loader)
{
  IResourceManager* resourceManager = ege_new ResourceManagerSingleThreadMock(engine, loader);

  EXPECT_CALL(*static_cast<ResourceManagerSingleThreadMock*>(resourceManager), createDefaultResources())
    .WillOnce(Return(true));

  return resourceManager;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerSingleThreadTest::waitUntilGroupsAreLoaded(const StringList& groupNames, s64 timeoutMs)
{
  // clear up the loaded group
  m_loadedGroups.clear();

  // call base class
  ResourceManagerTestBase::waitUntilGroupsAreLoaded(groupNames, timeoutMs);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerSingleThreadTest::waitUntilGroupsAreUnloaded(const StringList& groupNames, s64 timeoutMs)
{
  // clear up the unloaded group
  m_unloadedGroups.clear();

  // call base class
  ResourceManagerTestBase::waitUntilGroupsAreUnloaded(groupNames, timeoutMs);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ResourceManagerSingleThreadTest, AddResourcesNoDataDirectory)
{
  EXPECT_CALL(m_engine, resourceManager())
    .Times(0);

  EXPECT_EQ(EGE_ERROR_NOT_FOUND, resourceManager()->addResources("resources.xml", false));
  EXPECT_EQ(EGE_ERROR_NOT_FOUND, resourceManager()->addResources("resources.xml", true));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ResourceManagerSingleThreadTest, AddResourcesInvalidDataDirectory)
{
  resourceManager()->addDataDirectory("SomeNonExistingDirectory");
  EXPECT_EQ(EGE_ERROR_NOT_FOUND, resourceManager()->addResources("resources.xml", false));
  EXPECT_EQ(EGE_ERROR_NOT_FOUND, resourceManager()->addResources("resources.xml", true));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ResourceManagerSingleThreadTest, AddResourcesValidDataDirectory)
{
  setupResourceFactory();

  resourceManager()->addDataDirectory("Resource-test");
  EXPECT_EQ(EGE_ERROR_NOT_FOUND, resourceManager()->addResources("resources.xml", false));
  EXPECT_EQ(EGE_SUCCESS, resourceManager()->addResources("resources.xml", true));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ResourceManagerSingleThreadTest, AddInvalidResources)
{
  resourceManager()->addDataDirectory("Resource-test");
  EXPECT_EQ(EGE_ERROR_NOT_FOUND, resourceManager()->addResources("non-existing-resources.xml", false));
  EXPECT_EQ(EGE_ERROR_NOT_FOUND, resourceManager()->addResources("non-existing-resources.xml", true));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ResourceManagerSingleThreadTest, LoadExistingGroup)
{
  // setup resource factory
  std::vector<ResourceMock*> resources = setupResourceFactory();

  // set expectations on loading
  EXPECT_CALL(*resources[0], load())
    .WillOnce(Return(EGE_SUCCESS));
  EXPECT_CALL(*resources[1], load())
    .WillOnce(Return(EGE_SUCCESS));

  ege_connect(resourceManager(), signalGroupLoaded, this, ResourceManagerTestBase::onGroupLoaded);

  resourceManager()->addDataDirectory("Resource-test");
  EXPECT_EQ(EGE_SUCCESS, resourceManager()->addResources("resources.xml", true));

  EXPECT_EQ(EGE_SUCCESS, resourceManager()->loadGroup(KResourceGroup1));
  waitUntilGroupsAreLoaded(StringList() << KResourceGroup1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ResourceManagerSingleThreadTest, LoadNonExistingGroup)
{
  // setup resource factory
  setupResourceFactory();

  ege_connect(resourceManager(), signalGroupLoaded, this, ResourceManagerTestBase::onGroupLoaded);

  resourceManager()->addDataDirectory("Resource-test");
  EXPECT_EQ(EGE_SUCCESS, resourceManager()->addResources("resources.xml", true));

  EXPECT_EQ(EGE_ERROR, resourceManager()->loadGroup(KResourceNonExistingGroup));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ResourceManagerSingleThreadTest, UnloadGroup)
{
  // setup resource factory
  std::vector<ResourceMock*> resources = setupResourceFactory();

  // set expectations on loading
  EXPECT_CALL(*resources[0], load())
    .WillOnce(Return(EGE_SUCCESS));
  EXPECT_CALL(*resources[1], load())
    .WillOnce(Return(EGE_SUCCESS));

  ege_connect(resourceManager(), signalGroupLoaded, this, ResourceManagerTestBase::onGroupLoaded);
  ege_connect(resourceManager(), signalGroupUnloaded, this, ResourceManagerTestBase::onGroupUnloaded);

  resourceManager()->addDataDirectory("Resource-test");
  EXPECT_EQ(EGE_SUCCESS, resourceManager()->addResources("resources.xml", true));

  EXPECT_EQ(EGE_SUCCESS, resourceManager()->loadGroup(KResourceGroup1));
  waitUntilGroupsAreLoaded(StringList() << KResourceGroup1);

  resourceManager()->unloadGroup(KResourceGroup1);
  waitUntilGroupsAreUnloaded(StringList() << KResourceGroup1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ResourceManagerSingleThreadTest, UnloadNonExistingGroup)
{
  // setup resource factory
  setupResourceFactory();

  ege_connect(resourceManager(), signalGroupUnloaded, this, ResourceManagerTestBase::onGroupUnloaded);

  resourceManager()->addDataDirectory("Resource-test");
  EXPECT_EQ(EGE_SUCCESS, resourceManager()->addResources("resources.xml", true));

  resourceManager()->unloadGroup(KResourceNonExistingGroup);
  waitUntilGroupsAreUnloaded(StringList());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ResourceManagerSingleThreadTest, LoadProgress)
{
  // setup resource factory
  std::vector<ResourceMock*> resources = setupResourceFactory();

  // set expectations on loading
  EXPECT_CALL(*resources[0], load())
    .WillOnce(Return(EGE_SUCCESS));
  EXPECT_CALL(*resources[1], load())
    .WillOnce(Return(EGE_SUCCESS));

  ege_connect(resourceManager(), signalGroupLoaded, this, ResourceManagerTestBase::onGroupLoaded);
  ege_connect(resourceManager(), signalProgress, this, ResourceManagerTestBase::onProgress);

  resourceManager()->addDataDirectory("Resource-test");
  EXPECT_EQ(EGE_SUCCESS, resourceManager()->addResources("resources.xml", true));

  EXPECT_EQ(EGE_SUCCESS, resourceManager()->loadGroup(KResourceGroup1));
  waitUntilGroupsAreLoaded(StringList() << KResourceGroup1);

  EXPECT_EQ(KGroup1ResourceCount, m_totalProgress);
  EXPECT_EQ(KGroup1ResourceCount, m_currentProgress);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ResourceManagerSingleThreadTest, UnloadProgress)
{
  // setup resource factory
  std::vector<ResourceMock*> resources = setupResourceFactory();

  // set expectations on loading
  EXPECT_CALL(*resources[0], load())
    .WillOnce(Return(EGE_SUCCESS));
  EXPECT_CALL(*resources[1], load())
    .WillOnce(Return(EGE_SUCCESS));

  ege_connect(resourceManager(), signalGroupLoaded, this, ResourceManagerTestBase::onGroupLoaded);
  ege_connect(resourceManager(), signalGroupUnloaded, this, ResourceManagerTestBase::onGroupUnloaded);
  ege_connect(resourceManager(), signalProgress, this, ResourceManagerTestBase::onProgress);

  resourceManager()->addDataDirectory("Resource-test");
  EXPECT_EQ(EGE_SUCCESS, resourceManager()->addResources("resources.xml", true));

  EXPECT_EQ(EGE_SUCCESS, resourceManager()->loadGroup(KResourceGroup1));
  waitUntilGroupsAreLoaded(StringList() << KResourceGroup1);

  resourceManager()->unloadGroup(KResourceGroup1);
  waitUntilGroupsAreUnloaded(StringList() << KResourceGroup1);

  EXPECT_EQ(KGroup1ResourceCount, m_totalProgress);
  EXPECT_EQ(KGroup1ResourceCount, m_currentProgress);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ResourceManagerSingleThreadTest, LoadProgressWithDependency)
{
  // setup resource factory
  std::vector<ResourceMock*> resources = setupResourceFactory();

  // set expectations on loading
  EXPECT_CALL(*resources[0], load())
    .WillOnce(Return(EGE_SUCCESS));
  EXPECT_CALL(*resources[1], load())
    .WillOnce(Return(EGE_SUCCESS));
  EXPECT_CALL(*resources[2], load())
    .WillOnce(Return(EGE_SUCCESS));

  ege_connect(resourceManager(), signalGroupLoaded, this, ResourceManagerTestBase::onGroupLoaded);
  ege_connect(resourceManager(), signalProgress, this, ResourceManagerTestBase::onProgress);

  resourceManager()->addDataDirectory("Resource-test");
  EXPECT_EQ(EGE_SUCCESS, resourceManager()->addResources("resources.xml", true));

  EXPECT_EQ(EGE_SUCCESS, resourceManager()->loadGroup(KResourceGroup3));
  waitUntilGroupsAreLoaded(StringList() << KResourceGroup3);

  EXPECT_EQ(KGroup1ResourceCount + KGroup3ResourceCount, m_totalProgress);
  EXPECT_EQ(KGroup1ResourceCount + KGroup3ResourceCount, m_currentProgress);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ResourceManagerSingleThreadTest, UnloadProgressWithDependency)
{
  // setup resource factory
  std::vector<ResourceMock*> resources = setupResourceFactory();

  // set expectations on loading
  EXPECT_CALL(*resources[0], load())
    .WillOnce(Return(EGE_SUCCESS));
  EXPECT_CALL(*resources[1], load())
    .WillOnce(Return(EGE_SUCCESS));
  EXPECT_CALL(*resources[2], load())
    .WillOnce(Return(EGE_SUCCESS));

  ege_connect(resourceManager(), signalGroupLoaded, this, ResourceManagerTestBase::onGroupLoaded);
  ege_connect(resourceManager(), signalGroupUnloaded, this, ResourceManagerTestBase::onGroupUnloaded);
  ege_connect(resourceManager(), signalProgress, this, ResourceManagerTestBase::onProgress);

  resourceManager()->addDataDirectory("Resource-test");
  EXPECT_EQ(EGE_SUCCESS, resourceManager()->addResources("resources.xml", true));

  EXPECT_EQ(EGE_SUCCESS, resourceManager()->loadGroup(KResourceGroup3));
  waitUntilGroupsAreLoaded(StringList() << KResourceGroup3);

  resourceManager()->unloadGroup(KResourceGroup3);
  waitUntilGroupsAreUnloaded(StringList() << KResourceGroup3);

  EXPECT_EQ(KGroup1ResourceCount + KGroup3ResourceCount, m_totalProgress);
  EXPECT_EQ(KGroup1ResourceCount + KGroup3ResourceCount, m_currentProgress);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ResourceManagerSingleThreadTest, LoadUnloadBeforeUpdate)
{
  // setup resource factory
  std::vector<ResourceMock*> resources = setupResourceFactory();

  // set expectations on loading
  EXPECT_CALL(*resources[0], load())
    .Times(0);
  EXPECT_CALL(*resources[1], load())
    .Times(0);

  ege_connect(resourceManager(), signalGroupLoaded, this, ResourceManagerTestBase::onGroupLoaded);
  ege_connect(resourceManager(), signalGroupUnloaded, this, ResourceManagerTestBase::onGroupUnloaded);
  ege_connect(resourceManager(), signalProgress, this, ResourceManagerTestBase::onProgress);

  resourceManager()->addDataDirectory("Resource-test");
  EXPECT_EQ(EGE_SUCCESS, resourceManager()->addResources("resources.xml", true));

  EXPECT_EQ(EGE_SUCCESS, resourceManager()->loadGroup(KResourceGroup1));
  resourceManager()->unloadGroup(KResourceGroup1);

  waitUntilGroupsAreLoaded(StringList());

  EXPECT_EQ(0, m_totalProgress);
  EXPECT_EQ(0, m_currentProgress);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ResourceManagerSingleThreadTest, LoadAnotherDuringLoadComplete)
{
  // setup resource factory
  std::vector<ResourceMock*> resources = setupResourceFactory();

  // set expectations on loading
  EXPECT_CALL(*resources[0], load())
    .WillOnce(DoAll(IgnoreResult(InvokeWithoutArgs(resources[0], &ResourceMock::setLoaded)), 
                    Return(EGE_SUCCESS)));
  EXPECT_CALL(*resources[1], load())
    .WillOnce(DoAll(IgnoreResult(InvokeWithoutArgs(resources[1], &ResourceMock::setLoaded)), 
                    Return(EGE_SUCCESS)));
  EXPECT_CALL(*resources[2], load())
    .WillOnce(Return(EGE_SUCCESS));
  
  ege_connect(resourceManager(), signalGroupLoaded, this, ResourceManagerTestBase::onGroupLoadedAndLoadAnother);
  ege_connect(resourceManager(), signalProgress, this, ResourceManagerTestBase::onProgress);

  resourceManager()->addDataDirectory("Resource-test");
  EXPECT_EQ(EGE_SUCCESS, resourceManager()->addResources("resources.xml", true));

  EXPECT_EQ(EGE_SUCCESS, resourceManager()->loadGroup(KResourceGroup1));

  waitUntilGroupsAreLoaded(StringList() << KResourceGroup1 << KResourceGroup3);

  // expect the following progress:
  // - the group 3 (which is group 1 and group 3)
  EXPECT_EQ(KGroup1ResourceCount + KGroup3ResourceCount, m_totalProgress);
  EXPECT_EQ(KGroup1ResourceCount + KGroup3ResourceCount, m_currentProgress);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ResourceManagerSingleThreadTest, UnloadDuringLoadComplete)
{
  // setup resource factory
  std::vector<ResourceMock*> resources = setupResourceFactory();

  // set expectations on loading
  EXPECT_CALL(*resources[0], load())
    .WillOnce(DoAll(IgnoreResult(InvokeWithoutArgs(resources[0], &ResourceMock::setLoaded)), 
                    Return(EGE_SUCCESS)));
  EXPECT_CALL(*resources[1], load())
    .WillOnce(DoAll(IgnoreResult(InvokeWithoutArgs(resources[1], &ResourceMock::setLoaded)), 
                    Return(EGE_SUCCESS)));
  
  ege_connect(resourceManager(), signalGroupLoaded, this, ResourceManagerTestBase::onGroupLoadedAndUnloadSelfIfGroup1);
  ege_connect(resourceManager(), signalGroupUnloaded, this, ResourceManagerTestBase::onGroupUnloaded);
  ege_connect(resourceManager(), signalProgress, this, ResourceManagerTestBase::onProgress);

  resourceManager()->addDataDirectory("Resource-test");
  EXPECT_EQ(EGE_SUCCESS, resourceManager()->addResources("resources.xml", true));

  EXPECT_EQ(EGE_SUCCESS, resourceManager()->loadGroup(KResourceGroup1));

  waitUntilGroupsAreUnloaded(StringList() << KResourceGroup1);

  // expect the following progress:
  // - group 1 for unload part only
  EXPECT_EQ(KGroup1ResourceCount, m_totalProgress);
  EXPECT_EQ(KGroup1ResourceCount, m_currentProgress);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ResourceManagerSingleThreadTest, LoadAnotherDuringUnloadComplete)
{
  // setup resource factory
  std::vector<ResourceMock*> resources = setupResourceFactory();

  // set expectations on loading
  EXPECT_CALL(*resources[0], load())
    .Times(2)
    .WillRepeatedly(Return(EGE_SUCCESS));
  EXPECT_CALL(*resources[1], load())
    .Times(2)
    .WillRepeatedly(Return(EGE_SUCCESS));
  EXPECT_CALL(*resources[2], load())
    .WillOnce(Return(EGE_SUCCESS));
  
  ege_connect(resourceManager(), signalGroupLoaded, this, ResourceManagerTestBase::onGroupLoadedAndUnloadSelfIfGroup1);
  ege_connect(resourceManager(), signalGroupUnloaded, this, ResourceManagerTestBase::onGroupUnloadedAndLoadAnother);
  ege_connect(resourceManager(), signalProgress, this, ResourceManagerTestBase::onProgress);

  resourceManager()->addDataDirectory("Resource-test");
  EXPECT_EQ(EGE_SUCCESS, resourceManager()->addResources("resources.xml", true));

  EXPECT_EQ(EGE_SUCCESS, resourceManager()->loadGroup(KResourceGroup1));

  waitUntilGroupsAreLoaded(StringList() << KResourceGroup3);

  // expect the following progress:
  // - load for the group 3 (which is group 1 and group 3)
  EXPECT_EQ(KGroup1ResourceCount + KGroup3ResourceCount, m_totalProgress);
  EXPECT_EQ(KGroup1ResourceCount + KGroup3ResourceCount, m_currentProgress);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ResourceManagerSingleThreadTest, UnloadAnotherDuringUnloadComplete)
{
  // setup resource factory
  std::vector<ResourceMock*> resources = setupResourceFactory();

  // set expectations on loading
  EXPECT_CALL(*resources[0], load())
    .WillOnce(Return(EGE_SUCCESS));
  EXPECT_CALL(*resources[1], load())
    .WillOnce(Return(EGE_SUCCESS));
  EXPECT_CALL(*resources[2], load())
    .WillOnce(Return(EGE_SUCCESS));
  
  ege_connect(resourceManager(), signalGroupLoaded, this, ResourceManagerTestBase::onGroupLoadedAndUnloadGroup1IfGroup3);
  ege_connect(resourceManager(), signalGroupUnloaded, this, ResourceManagerTestBase::onGroupUnloadedAndUnloadGroup3IfGroup1);
  ege_connect(resourceManager(), signalProgress, this, ResourceManagerTestBase::onProgress);

  resourceManager()->addDataDirectory("Resource-test");
  EXPECT_EQ(EGE_SUCCESS, resourceManager()->addResources("resources.xml", true));

  EXPECT_EQ(EGE_SUCCESS, resourceManager()->loadGroup(KResourceGroup3));

  waitUntilGroupsAreUnloaded(StringList() << KResourceGroup1 << KResourceGroup3);

  // expect the following progress:
  // - unload for the group 3 (which is group 1 and group 3)
  EXPECT_EQ(KGroup1ResourceCount + KGroup3ResourceCount, m_totalProgress);
  EXPECT_EQ(KGroup1ResourceCount + KGroup3ResourceCount, m_currentProgress);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ResourceManagerSingleThreadTest, LoadAnotherDuringLoadingProgress)
{
  // setup resource factory
  std::vector<ResourceMock*> resources = setupResourceFactory();

  // set expectations on loading
  EXPECT_CALL(*resources[0], load())
    .WillOnce(DoAll(IgnoreResult(InvokeWithoutArgs(resources[0], &ResourceMock::setLoaded)), 
                    Return(EGE_SUCCESS)));
  {
    InSequence s;

    // keep reporting resource is still being loaded so main thread is able to proceed with new load request
    EXPECT_CALL(*resources[1], load())
      .Times(100)
      .WillRepeatedly(Return(EGE_WAIT));

    EXPECT_CALL(*resources[1], load())
      .WillOnce(DoAll(IgnoreResult(InvokeWithoutArgs(resources[1], &ResourceMock::setLoaded)),
                      Return(EGE_SUCCESS)));
  }

  EXPECT_CALL(*resources[2], load())
    .WillOnce(Return(EGE_SUCCESS));
  
  ege_connect(resourceManager(), signalGroupLoaded, this, ResourceManagerTestBase::onGroupLoaded);
  ege_connect(resourceManager(), signalProgress, this, ResourceManagerTestBase::onProgressLoadGroup3AfterFirstResource);

  resourceManager()->addDataDirectory("Resource-test");
  EXPECT_EQ(EGE_SUCCESS, resourceManager()->addResources("resources.xml", true));

  EXPECT_EQ(EGE_SUCCESS, resourceManager()->loadGroup(KResourceGroup1));

  waitUntilGroupsAreLoaded(StringList() << KResourceGroup1 << KResourceGroup3);

  // expect the following progress:
  // - load for the group 1
  // - load for the group 3 (which is group 1 and group 3)
  EXPECT_EQ(KGroup1ResourceCount + (KGroup1ResourceCount + KGroup3ResourceCount), m_totalProgress);
  EXPECT_EQ(KGroup1ResourceCount + (KGroup1ResourceCount + KGroup3ResourceCount), m_currentProgress);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ResourceManagerSingleThreadTest, UnloadAnotherDuringLoadingProgress)
{
  // setup resource factory
  std::vector<ResourceMock*> resources = setupResourceFactory();

  // set expectations on loading
  EXPECT_CALL(*resources[0], load())
    .WillOnce(DoAll(IgnoreResult(InvokeWithoutArgs(resources[0], &ResourceMock::setLoaded)), 
                    Return(EGE_SUCCESS)));
  {
    InSequence s;

    // keep reporting resource is still being loaded so main thread is able to proceed with new load request
    EXPECT_CALL(*resources[1], load())
      .Times(100)
      .WillRepeatedly(Return(EGE_WAIT));

    EXPECT_CALL(*resources[1], load())
      .WillOnce(DoAll(IgnoreResult(InvokeWithoutArgs(resources[1], &ResourceMock::setLoaded)),
                      Return(EGE_SUCCESS)));
  }

  EXPECT_CALL(*resources[2], load())
    .WillOnce(Return(EGE_SUCCESS));
  
  ege_connect(resourceManager(), signalGroupUnloaded, this, ResourceManagerTestBase::onGroupUnloaded);
  ege_connect(resourceManager(), signalProgress, this, ResourceManagerTestBase::onProgressUnloadGroup1AfterFirstResource);

  resourceManager()->addDataDirectory("Resource-test");
  EXPECT_EQ(EGE_SUCCESS, resourceManager()->addResources("resources.xml", true));

  EXPECT_EQ(EGE_SUCCESS, resourceManager()->loadGroup(KResourceGroup3));

  waitUntilGroupsAreUnloaded(StringList() << KResourceGroup1);

  // expect the following progress:
  // - load for the group 1
  // - load for the group 3 (which is group 1 and group 3)
  // - unload of the group 1
  EXPECT_EQ(KGroup1ResourceCount + (KGroup1ResourceCount + KGroup3ResourceCount), m_totalProgress);
  EXPECT_EQ(KGroup1ResourceCount + (KGroup1ResourceCount + KGroup3ResourceCount), m_currentProgress);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
