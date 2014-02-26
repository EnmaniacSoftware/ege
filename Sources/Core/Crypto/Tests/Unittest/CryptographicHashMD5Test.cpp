#include "TestFramework/Interface/TestBase.h"
#include <EGEHash.h>
#include <EGEDataBuffer.h>
#include <stdlib.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static char* KPlainText     = "This is a simple plain text to be hashed! Lets hope we can do it without loosing any data!!!!";
static char* KPlainTextMD5  = "2bfae63ad200d56748e35fadc88f1cec";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class CryptographicHashMD5Test : public TestBase
{
  protected:

    static void SetUpTestCase();
    static void TearDownTestCase();

  protected:

    CryptographicHashMD5Test();
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CryptographicHashMD5Test::CryptographicHashMD5Test() : TestBase(0.0001f)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void CryptographicHashMD5Test::SetUpTestCase()
{
  EXPECT_TRUE(MemoryManager::Initialize());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void CryptographicHashMD5Test::TearDownTestCase()
{
  MemoryManager::Deinitialize();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(CryptographicHashMD5Test, HashFromDataBuffer)
{
  CryptographicHash md5(EMD5);

  // check validity
  EXPECT_TRUE(md5.isValid());

  // add data into hasher
  DataBuffer data(KPlainText, strlen(KPlainText));
  EXPECT_EQ(EGE_SUCCESS, md5.addData(data));

  // get actual result
  PDataBuffer actualResult = md5.result();

  // compare
  EXPECT_EQ(strlen(KPlainTextMD5) / 2, actualResult->size());
  for (s64 i = 0; i < actualResult->size(); ++i)
  {
    // retrieve expected value
    int value = 0;
    EXPECT_EQ(1, sscanf(KPlainTextMD5 + i * 2, "%2x", &value));

    // retrieve actual value
    int actualValue = *reinterpret_cast<unsigned char*>(actualResult->data(i));

    // test
    EXPECT_EQ(value, actualValue);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(CryptographicHashMD5Test, HashFromString)
{
  CryptographicHash md5(EMD5);

  // check validity
  EXPECT_TRUE(md5.isValid());

  // add data into hasher
  EXPECT_EQ(EGE_SUCCESS, md5.addData(KPlainText, static_cast<s32>(strlen(KPlainText))));

  // get actual result
  PDataBuffer actualResult = md5.result();

  // compare
  EXPECT_EQ(strlen(KPlainTextMD5) / 2, actualResult->size());
  for (s64 i = 0; i < actualResult->size(); ++i)
  {
    // retrieve expected value
    int value = 0;
    EXPECT_EQ(1, sscanf(KPlainTextMD5 + i * 2, "%2x", &value));

    // retrieve actual value
    int actualValue = *reinterpret_cast<unsigned char*>(actualResult->data(i));

    // test
    EXPECT_EQ(value, actualValue);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
