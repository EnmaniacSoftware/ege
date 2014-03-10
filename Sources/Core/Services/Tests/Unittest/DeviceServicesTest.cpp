#include "TestFramework/Interface/TestBase.h"
#include <EGEMemory.h>
#include <EGEDebug.h>
#include <EGEString.h>
#include <EGEDeviceServices.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* KStringValueName  = "MyString";
const char* KStringValueValue = "This is value of MyString";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class DeviceServicesTest : public TestBase
{
  public:

    DeviceServicesTest();

  protected:

    static void SetUpTestCase();
    static void TearDownTestCase();

  protected:

    virtual void SetUp();
    virtual void TearDown();

  protected:

    /*! Device services object. */
    DeviceServices* m_services;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DeviceServicesTest::DeviceServicesTest() : TestBase()
                                         , m_services(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DeviceServicesTest::SetUpTestCase()
{
  EXPECT_TRUE(MemoryManager::Initialize());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DeviceServicesTest::TearDownTestCase()
{
  MemoryManager::Deinitialize();
  remove("confidential.sqlite");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DeviceServicesTest::SetUp()
{
  EXPECT_TRUE(NULL == m_services);

  // allocate
  m_services = ege_new PLATFORM_CLASSNAME(DeviceServices)();
  EXPECT_TRUE(NULL != m_services);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DeviceServicesTest::TearDown()
{
  EXPECT_TRUE(NULL != m_services);
  EGE_DELETE(m_services);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DeviceServicesTest, StoreAndRetrieveConfidentialStringValue)
{
  // store string value
  EXPECT_EQ(EGE_SUCCESS, m_services->storeConfidentialValue(KStringValueName, KStringValueValue));

  // retrieve string value
  String stringValue;
  EXPECT_EQ(EGE_SUCCESS, m_services->retrieveConfidentialValue(KStringValueName, stringValue));

  // compare
  EXPECT_STREQ(KStringValueValue, stringValue.toAscii());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DeviceServicesTest, OverwriteConfidentialValue)
{
  String stringValue;

  const char* KNewStringValue = "This is new string value";

  // store string value
  EXPECT_EQ(EGE_SUCCESS, m_services->storeConfidentialValue(KStringValueName, KStringValueValue));

  // retrieve string value and compare
  EXPECT_EQ(EGE_SUCCESS, m_services->retrieveConfidentialValue(KStringValueName, stringValue));
  EXPECT_STREQ(KStringValueValue, stringValue.toAscii());

  // store (overwrite) new value
  EXPECT_EQ(EGE_SUCCESS, m_services->storeConfidentialValue(KStringValueName, KNewStringValue));

  // retrieve string value and compare
  EXPECT_EQ(EGE_SUCCESS, m_services->retrieveConfidentialValue(KStringValueName, stringValue));
  EXPECT_STREQ(KNewStringValue, stringValue.toAscii());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------