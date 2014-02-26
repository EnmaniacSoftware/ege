#include "TestFramework/Interface/TestBase.h"
#include <EGEHash.h>
#include <EGEString.h>
#include <string>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const int KRepetitionsCount = 20;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class HashTest : public TestBase
{
  protected:

    HashTest();

    /*! Calculates hash value from given data.
     *  @param  data  Data from which hash is to be calculated.
     *  @param  size  Size of data.
     *  @return 32-bit hash.
     */
    u32 hash(const u8* data, s32 size) const;
    /*! Calculates hash value from given string.
     *  @param  string  Data from which hash is to be calculated.
     *  @return 32-bit hash.
     */
    u32 hash(const std::string& data) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
HashTest::HashTest() : TestBase(0.0001f)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 HashTest::hash(const u8* data, s32 size) const
{
  u32 h = 0;
  u32 g;

  while (size--) 
  {
    h = (h << 4) + *data++;
    g = h & 0xf0000000;
    h ^= g >> 23;
    h &= ~g;
  }

  return h;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 HashTest::hash(const std::string& data) const
{
  return hash(reinterpret_cast<const u8*>(data.c_str()), static_cast<s32>(data.length()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(HashTest, HashFromData)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const int dataSize = rand() + 1;
    u8* data = new u8[dataSize];

    EXPECT_TRUE(NULL!= data);

    // initialize data
    for (s32 i = 0; i < dataSize; ++i)
    {
      data[i] = static_cast<u8>(rand() % 256);
    }

    // calculate reference hash
    const u32 expectedHash = hash(data, dataSize);

    // calculate actual hash
    const u32 actualHash = Hash::FromData(data, dataSize);

    // cleanup
    delete [] data;

    // test
    EXPECT_EQ(expectedHash, actualHash);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(HashTest, HashFromString)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    const int dataSize = rand() + 1;

    std::string data;

    // initialize data
    for (s32 i = 0; i < dataSize; ++i)
    {
      data += static_cast<u8>(rand() % 128);
    }

    // calculate reference hash
    const u32 expectedHash = hash(data);

    // calculate actual hash
    const u32 actualHash = Hash::FromString(String(data));

    // test
    EXPECT_EQ(expectedHash, actualHash);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
