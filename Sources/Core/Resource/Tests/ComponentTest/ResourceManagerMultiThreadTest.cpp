#include "Core/Resource/Tests/ComponentTest/ResourceManagerTestBase.h"
#include "Core/Resource/Tests/ComponentTest/ResourceManagerMultiThreadMock.h"
#include <EGEResources.h> // ResourceGroup
#include <EGEString.h>
#include <EGETimer.h>

#include <gmock/gmock.h>

using namespace ::testing;

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceManagerMultiThreadTest : public ResourceManagerTestBase
{
  public:

    ResourceManagerMultiThreadTest();

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
ResourceManagerMultiThreadTest::ResourceManagerMultiThreadTest() 
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerMultiThreadTest::SetUp()
{
  // call base class
  ResourceManagerTestBase::SetUp();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerMultiThreadTest::TearDown()
{
  // call base class
  ResourceManagerTestBase::TearDown();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IResourceManager* ResourceManagerMultiThreadTest::createResourceManager(Engine& engine, IResourceLoader& loader)
{
  IResourceManager* resourceManager = ege_new ResourceManagerMultiThreadMock(engine, loader);

  EXPECT_CALL(*static_cast<ResourceManagerMultiThreadMock*>(resourceManager), createDefaultResources())
    .WillOnce(Return(true));

  return resourceManager;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerMultiThreadTest::waitUntilGroupsAreLoaded(const StringList& groupNames, s64 timeoutMs)
{
  // clear up the loaded group
  m_loadedGroups.clear();

  // call base class
  ResourceManagerTestBase::waitUntilGroupsAreLoaded(groupNames, timeoutMs);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerMultiThreadTest::waitUntilGroupsAreUnloaded(const StringList& groupNames, s64 timeoutMs)
{
  // clear up the unloaded group
  m_unloadedGroups.clear();

  // call base class
  ResourceManagerTestBase::waitUntilGroupsAreUnloaded(groupNames, timeoutMs);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ResourceManagerMultiThreadTest, AddResourcesNoDataDirectory)
{
  EXPECT_CALL(m_engine, resourceManager())
    .Times(0);

  EXPECT_EQ(EGE_ERROR_NOT_FOUND, resourceManager()->addResources("resources.xml", false));
  EXPECT_EQ(EGE_ERROR_NOT_FOUND, resourceManager()->addResources("resources.xml", true));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ResourceManagerMultiThreadTest, DISABLED_AddResourcesInvalidDataDirectory)
{
  resourceManager()->addDataDirectory("SomeNonExistingDirectory");
  EXPECT_EQ(EGE_ERROR_NOT_FOUND, resourceManager()->addResources("resources.xml", false));
  EXPECT_EQ(EGE_ERROR_NOT_FOUND, resourceManager()->addResources("resources.xml", true));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ResourceManagerMultiThreadTest, AddResourcesValidDataDirectory)
{
  setupResourceFactory();

  resourceManager()->addDataDirectory("Resource-test");
  EXPECT_EQ(EGE_ERROR_NOT_FOUND, resourceManager()->addResources("resources.xml", false));
  EXPECT_EQ(EGE_SUCCESS, resourceManager()->addResources("resources.xml", true));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ResourceManagerMultiThreadTest, AddInvalidResources)
{
  resourceManager()->addDataDirectory("Resource-test");
  EXPECT_EQ(EGE_ERROR_NOT_FOUND, resourceManager()->addResources("non-existing-resources.xml", false));
  EXPECT_EQ(EGE_ERROR_NOT_FOUND, resourceManager()->addResources("non-existing-resources.xml", true));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ResourceManagerMultiThreadTest, LoadExistingGroup)
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
  EXPECT_EQ(EGE_ERROR_NOT_FOUND, resourceManager()->addResources("resources.xml", false));
  EXPECT_EQ(EGE_SUCCESS, resourceManager()->addResources("resources.xml", true));

  EXPECT_EQ(EGE_SUCCESS, resourceManager()->loadGroup(KResourceGroup1));
  waitUntilGroupsAreLoaded(StringList() << KResourceGroup1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ResourceManagerMultiThreadTest, LoadNonExistingGroup)
{
  // setup resource factory
  setupResourceFactory();

  ege_connect(resourceManager(), signalGroupLoaded, this, ResourceManagerTestBase::onGroupLoaded);

  resourceManager()->addDataDirectory("Resource-test");
  EXPECT_EQ(EGE_ERROR_NOT_FOUND, resourceManager()->addResources("resources.xml", false));
  EXPECT_EQ(EGE_SUCCESS, resourceManager()->addResources("resources.xml", true));

  EXPECT_EQ(EGE_ERROR, resourceManager()->loadGroup(KResourceNonExistingGroup));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ResourceManagerMultiThreadTest, UnloadGroup)
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
  EXPECT_EQ(EGE_ERROR_NOT_FOUND, resourceManager()->addResources("resources.xml", false));
  EXPECT_EQ(EGE_SUCCESS, resourceManager()->addResources("resources.xml", true));

  EXPECT_EQ(EGE_SUCCESS, resourceManager()->loadGroup(KResourceGroup1));
  waitUntilGroupsAreLoaded(StringList() << KResourceGroup1);

  resourceManager()->unloadGroup(KResourceGroup1);
  waitUntilGroupsAreUnloaded(StringList() << KResourceGroup1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ResourceManagerMultiThreadTest, UnloadNonExistingGroup)
{
  // setup resource factory
  setupResourceFactory();

  ege_connect(resourceManager(), signalGroupUnloaded, this, ResourceManagerTestBase::onGroupUnloaded);

  resourceManager()->addDataDirectory("Resource-test");
  EXPECT_EQ(EGE_ERROR_NOT_FOUND, resourceManager()->addResources("resources.xml", false));
  EXPECT_EQ(EGE_SUCCESS, resourceManager()->addResources("resources.xml", true));

  resourceManager()->unloadGroup(KResourceNonExistingGroup);
  waitUntilGroupsAreUnloaded(StringList());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ResourceManagerMultiThreadTest, LoadProgress)
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
TEST_F(ResourceManagerMultiThreadTest, UnloadProgress)
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
TEST_F(ResourceManagerMultiThreadTest, LoadProgressWithDependency)
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
TEST_F(ResourceManagerMultiThreadTest, UnloadProgressWithDependency)
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
TEST_F(ResourceManagerMultiThreadTest, LoadUnloadBeforeUpdate)
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