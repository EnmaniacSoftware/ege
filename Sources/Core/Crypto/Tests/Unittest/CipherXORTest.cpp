#include "TestFramework/Interface/TestBase.h"
#include <EGECipher.h>
#include <EGEDataBuffer.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static char* KPlainText = "This is a simple plain text to be encrypted and decrypted\n"
                          "Lets hope we can do it without loosing any data!!!!";
static char* KPlainText2 = "This is a another simple plain text to be encrypted and decrypted\n"
                           "Lets hope we can do it without loosing any data as well!!!!";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class CipherXORTest : public TestBase
{
  protected:

    static void SetUpTestCase();
    static void TearDownTestCase();

  protected:

    CipherXORTest();
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CipherXORTest::CipherXORTest() : TestBase(0.0001f)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void CipherXORTest::SetUpTestCase()
{
  EXPECT_TRUE(MemoryManager::Initialize());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void CipherXORTest::TearDownTestCase()
{
  MemoryManager::Deinitialize();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(CipherXORTest, EncryptDecryptViaDataBuffers)
{
  CipherKey key("Dummy key");
  CipherXOR encryptor(EEncrypt, key);
  CipherXOR decryptor(EDecrypt, key);

  // encrypt data
  DataBuffer dataIn(KPlainText, strlen(KPlainText));
  EXPECT_EQ(EGE_SUCCESS, encryptor.addData(dataIn));

  // get encrypted data
  PDataBuffer encryptedData = encryptor.result();

  // decrypt
  EXPECT_EQ(EGE_SUCCESS, decryptor.addData(encryptedData));

  // get decrypted data
  PDataBuffer decryptedData = decryptor.result();

  // compare results
  EXPECT_EQ(dataIn.size(), decryptedData->size());
  EXPECT_EQ(0, memcmp(decryptedData->data(), dataIn.data(), static_cast<size_t>(dataIn.size())));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(CipherXORTest, EncryptDecryptViaRawData)
{
  CipherKey key("Dummy key");
  CipherXOR encryptor(EEncrypt, key);
  CipherXOR decryptor(EDecrypt, key);

  // encrypt data
  EXPECT_EQ(EGE_SUCCESS, encryptor.addData(KPlainText, strlen(KPlainText)));

  // get encrypted data
  PDataBuffer encryptedData = encryptor.result();

  // decrypt
  EXPECT_EQ(EGE_SUCCESS, decryptor.addData(reinterpret_cast<const char*>(encryptedData->data()), static_cast<s32>(encryptedData->size())));

  // get decrypted data
  PDataBuffer decryptedData = decryptor.result();

  // compare results
  EXPECT_EQ(static_cast<s64>(strlen(KPlainText)), decryptedData->size());
  EXPECT_EQ(0, memcmp(decryptedData->data(), KPlainText, strlen(KPlainText)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(CipherXORTest, Reset)
{
  CipherKey key("Dummy key");
  CipherXOR encryptor1(EEncrypt, key);
  CipherXOR encryptor2(EEncrypt, key);
  CipherXOR decryptor(EDecrypt, key);

  // encrypt data
  EXPECT_EQ(EGE_SUCCESS, encryptor1.addData(KPlainText, strlen(KPlainText)));
  EXPECT_EQ(EGE_SUCCESS, encryptor2.addData(KPlainText2, strlen(KPlainText2)));

  // get encrypted data
  PDataBuffer encryptedData1 = encryptor1.result();
  PDataBuffer encryptedData2 = encryptor2.result();

  // decrypt first
  EXPECT_EQ(EGE_SUCCESS, decryptor.addData(reinterpret_cast<const char*>(encryptedData1->data()), static_cast<s32>(encryptedData1->size())));

  // get decrypted data
  PDataBuffer decryptedData = decryptor.result();

  // compare results
  EXPECT_EQ(static_cast<s64>(strlen(KPlainText)), decryptedData->size());
  EXPECT_EQ(0, memcmp(decryptedData->data(), KPlainText, strlen(KPlainText)));

  // reset decryptor
  decryptor.reset();

  // decrypt second
  EXPECT_EQ(EGE_SUCCESS, decryptor.addData(reinterpret_cast<const char*>(encryptedData2->data()), static_cast<s32>(encryptedData2->size())));

  // get decrypted data
  decryptedData = decryptor.result();

  // compare results
  EXPECT_EQ(static_cast<s64>(strlen(KPlainText2)), decryptedData->size());
  EXPECT_EQ(0, memcmp(decryptedData->data(), KPlainText2, strlen(KPlainText2)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
