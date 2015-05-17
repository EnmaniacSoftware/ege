#include "Core/Resource/Tests/ComponentTest/ResourceManagerTestBase.h"
#include "Core/Engine/Interface/IEngineModule.h"
#include <EGEMemory.h>
#include <EGETimer.h>
#include <EGEResourceManager.h>

#include <gmock/gmock.h>

using namespace ::testing;

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* ResourceManagerTestBase::KResourceGroup1           = "group-1";
const char* ResourceManagerTestBase::KResourceNonExistingGroup = "non-existing-group-name";
const char* ResourceManagerTestBase::KResourceGroup3           = "group-3";

const s64 ResourceManagerTestBase::KDefaultGroupLoadTimeoutMs = 5000;

const s32 ResourceManagerTestBase::KGroupCount = 3;
const s32 ResourceManagerTestBase::KGroup1ResourceCount = 2;
const s32 ResourceManagerTestBase::KGroup3ResourceCount = 1;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManagerTestBase::ResourceManagerTestBase() 
: m_loader(NULL)
, m_resourceFactory(NULL)
, m_resourceManager(NULL)
, m_totalProgress(0)
, m_currentProgress(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManagerTestBase::~ResourceManagerTestBase()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerTestBase::SetUpTestCase()
{
  EXPECT_TRUE(MemoryManager::Initialize());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerTestBase::TearDownTestCase()
{
  MemoryManager::Deinitialize();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerTestBase::SetUp()
{
  EXPECT_TRUE(NULL == m_loader);
  EXPECT_TRUE(NULL == m_resourceFactory);
  
  m_loader          = ege_new ResourceLoaderXML(m_engine);
  m_resourceFactory = ege_new ResourceFactoryMock(m_engine);
  m_resourceManager = createResourceManager(m_engine, *m_loader);

  IEngineModule* module = dynamic_cast<IEngineModule*>(m_resourceManager);
  EXPECT_EQ(EGE_SUCCESS, module->construct());

  EXPECT_CALL(m_engine, resourceFactory())
    .WillRepeatedly(Return(m_resourceFactory));
  EXPECT_CALL(m_engine, resourceManager())
    .Times(AtMost(KGroupCount))
    .WillRepeatedly(Return(m_resourceManager));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerTestBase::TearDown()
{
  EXPECT_TRUE(NULL != m_loader);
  EXPECT_TRUE(NULL != m_resourceFactory);
  EXPECT_TRUE(NULL != m_resourceManager);

  // shut manager down
  IEngineModule* module = dynamic_cast<IEngineModule*>(m_resourceManager);
  module->onShutdown();

  // wait until shutdown
  while (EModuleStateClosed != module->state())
  {
    module->update(Time());
  }

  EGE_DELETE(m_resourceManager);
  EGE_DELETE(m_resourceFactory);
  EGE_DELETE(m_loader);

  m_loadedGroups.clear();
  m_unloadedGroups.clear();
  m_totalProgress = 0;
  m_currentProgress = 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IResourceManager* ResourceManagerTestBase::resourceManager() const
{
  return m_resourceManager;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerTestBase::onGroupLoaded(const String& groupName, EGEResult result)
{
  EGE_UNUSED(result)

  m_loadedGroups << groupName;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerTestBase::onGroupUnloaded(const String& groupName, EGEResult result)
{
  EGE_UNUSED(result)

  m_unloadedGroups << groupName;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerTestBase::onGroupLoadedAndLoadAnother(const String& groupName, EGEResult result)
{
  // process as normal
  onGroupLoaded(groupName, result);
  
  // load another group if this is not what is scheduled here
  if (KResourceGroup3 != groupName)
  {
    EXPECT_EQ(EGE_SUCCESS, resourceManager()->loadGroup(KResourceGroup3));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerTestBase::onGroupLoadedAndUnloadSelfIfGroup1(const String& groupName, EGEResult result)
{
  // process as normal
  onGroupLoaded(groupName, result);

  // unload self if group 1
  if (KResourceGroup1 == groupName)
  {
    resourceManager()->unloadGroup(groupName);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerTestBase::onGroupUnloadedAndLoadAnother(const String& groupName, EGEResult result)
{
  // process as normal
  onGroupUnloaded(groupName, result);

  // load another group
  EXPECT_EQ(EGE_SUCCESS, resourceManager()->loadGroup(KResourceGroup3));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerTestBase::onGroupLoadedAndUnloadGroup1IfGroup3(const String& groupName, EGEResult result)
{
  // process as normal
  onGroupLoaded(groupName, result);

  // unload group 1 if just loaded group 3
  if (KResourceGroup3 == groupName)
  {
    resourceManager()->unloadGroup(KResourceGroup1);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerTestBase::onGroupUnloadedAndUnloadGroup3IfGroup1(const String& groupName, EGEResult result)
{
  // process as normal
  onGroupUnloaded(groupName, result);

  // unload another group
  if (KResourceGroup1 == groupName)
  {
    resourceManager()->unloadGroup(KResourceGroup3);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerTestBase::onProgress(s32 processed, s32 total)
{
  m_totalProgress = total;
  m_currentProgress = processed;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerTestBase::onProgressLoadGroup3AfterFirstResource(s32 processed, s32 total)
{
  if (1 == processed)
  {
    EXPECT_EQ(EGE_SUCCESS, resourceManager()->loadGroup(KResourceGroup3));
  }

  onProgress(processed, total);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerTestBase::onProgressUnloadGroup1AfterFirstResource(s32 processed, s32 total)
{
  if (1 == processed)
  {
    resourceManager()->unloadGroup(KResourceGroup1);
  }

  onProgress(processed, total);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerTestBase::waitUntilGroupsAreLoaded(const StringList& groupNames, s64 timeoutMs)
{
  IEngineModule* module = dynamic_cast<IEngineModule*>(resourceManager());

  bool allLoaded = false;
  while ((0 < timeoutMs) && ! allLoaded)
  {
    const s64 timestamp = Timer::GetMiliseconds();

    // update resource manager
    module->update(Time());

    // check if all groups loaded
    allLoaded = true;
    for (StringList::ConstIterator it = groupNames.begin(); it != groupNames.end(); ++it)
    {
      allLoaded = allLoaded && m_loadedGroups.contains(*it);
    }

    timeoutMs -= Timer::GetMiliseconds() - timestamp;
  }

  EXPECT_TRUE(allLoaded) << "Not all requested groups could be loaded!";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceManagerTestBase::waitUntilGroupsAreUnloaded(const StringList& groupNames, s64 timeoutMs)
{
  IEngineModule* module = dynamic_cast<IEngineModule*>(resourceManager());

  bool allLoaded = false;
  while ((0 < timeoutMs) && ! allLoaded)
  {
    const s64 timestamp = Timer::GetMiliseconds();

    // update resource manager
    module->update(Time());

    // check if all groups loaded
    allLoaded = true;
    for (StringList::ConstIterator it = groupNames.begin(); it != groupNames.end(); ++it)
    {
      allLoaded = allLoaded && m_unloadedGroups.contains(*it);
    }

    timeoutMs -= Timer::GetMiliseconds() - timestamp;
  }

  EXPECT_TRUE(allLoaded) << "Not all requested groups could be unloaded!";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<ResourceMock*> ResourceManagerTestBase::setupResourceFactory()
{
  // NOTE: this should correspond to number of unique resource items in resources.xml
  //       These objects ownership is transferred to ResourceManager
  ResourceMock* resource1 = ege_new ResourceMock(m_engine, NULL);
  ResourceMock* resource2 = ege_new ResourceMock(m_engine, NULL);
  ResourceMock* resource3 = ege_new ResourceMock(m_engine, NULL);

  std::vector<ResourceMock*> resources;
  resources.push_back(resource1);
  resources.push_back(resource2);
  resources.push_back(resource3);

  EXPECT_CALL(*m_resourceFactory, createInstance(_, _))
    .WillOnce(Return(resource1))
    .WillOnce(Return(resource2))
    .WillOnce(Return(resource3));

  for (std::vector<ResourceMock*>::iterator it = resources.begin(); it != resources.end(); ++it)
  {
    ResourceMock* resource = *it;

    EXPECT_CALL(*resource, create(_, _))
      .WillOnce(Return(EGE_SUCCESS));
    EXPECT_CALL(*resource, unload())
      .WillOnce(InvokeWithoutArgs(resource, &ResourceMock::setUnloaded));
  }

  return resources;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
