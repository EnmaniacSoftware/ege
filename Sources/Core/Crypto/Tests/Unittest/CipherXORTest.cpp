#include "TestFramework/Interface/TestBase.h"
#include <EGECipher.h>
#include <EGEDataBuffer.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static char* KPlainText = "This is a simple plain text to be compressed and decompressed\n"
                          "Let hope we can do it without loosing any data!!!!";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const int KRepetitionsCount = 20;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class CipherXORTest : public TestBase
{
  protected:

    CipherXORTest();
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CipherXORTest::CipherXORTest() : TestBase(0.0001f)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(CipherXORTest, EncryptDecryptViaDataBuffers)
{
  CipherKey key("Dummy key");
  Cipher encryptor(EXOR, EEncrypt, key);
  Cipher decryptor(EXOR, EDecrypt, key);

  // test cipher objects
  EXPECT_TRUE(encryptor.isValid());
  EXPECT_TRUE(decryptor.isValid());

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
  Cipher encryptor(EXOR, EEncrypt, key);
  Cipher decryptor(EXOR, EDecrypt, key);

  // test cipher objects
  EXPECT_TRUE(encryptor.isValid());
  EXPECT_TRUE(decryptor.isValid());

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
