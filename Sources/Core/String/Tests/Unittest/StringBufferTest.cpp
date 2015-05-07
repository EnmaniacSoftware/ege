#include "TestFramework/Interface/TestBase.h"
#include <EGEStringBuffer.h>

using namespace ::testing;

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class StringBufferTest : public TestBase
{
  protected:
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringBufferTest, Construct)
{
  StringBuffer buffer;
  EXPECT_EQ(0, buffer.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringBufferTest, Write)
{
  const char* KRawBuffers[] = { "This ", "is ", "my ", "test ", "buffer" };

  StringBuffer buffer;

  // write string by string
  for (size_t i = 0; i < sizeof (KRawBuffers) / sizeof (KRawBuffers[0]); ++i)
  {
    const char* rawBuffer = KRawBuffers[i];

    EXPECT_EQ(strlen(rawBuffer), buffer.write(rawBuffer, strlen(rawBuffer)));
  }

  // check the total length
  s32 totalLength = 0;
  for (size_t i = 0; i < sizeof (KRawBuffers) / sizeof (KRawBuffers[0]); ++i)
  {
    const char* rawBuffer = KRawBuffers[i];

    totalLength += strlen(rawBuffer);
  }

  EXPECT_EQ(totalLength, buffer.string().length());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringBufferTest, Read)
{
  const char* KRawBuffer = "This is my test buffer";
  const s32 KRawBufferLength = strlen(KRawBuffer);

  char readBuffer[256];
  memset(readBuffer, 0, sizeof (readBuffer));

  StringBuffer buffer;

  // write some data
  EXPECT_EQ(KRawBufferLength, buffer.write(KRawBuffer, KRawBufferLength));
  EXPECT_EQ(KRawBufferLength, buffer.string().length());

  // read 5 bytes
  EXPECT_EQ(5, buffer.read(readBuffer, 5));
  EXPECT_EQ(0, strncmp(readBuffer, KRawBuffer, 5));

  // read 10 more bytes
  EXPECT_EQ(10, buffer.read(readBuffer + 5, 10));
  EXPECT_EQ(0, strncmp(readBuffer, KRawBuffer, 15));

  // read 10 more bytes
  // NOTE: expect end of the buffer
  const s64 KExpectedBytesRead = KRawBufferLength - 15;
  EXPECT_EQ(KExpectedBytesRead, buffer.read(readBuffer + 15, 10));
  EXPECT_EQ(0, strncmp(readBuffer, KRawBuffer, KRawBufferLength));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringBufferTest, Size)
{
  const char* KRawBuffer = "This is my test buffer";
  const s32 KRawBufferLength = strlen(KRawBuffer);

  StringBuffer buffer;
  EXPECT_EQ(0, buffer.string().length());
  EXPECT_EQ(0, buffer.size());

  // write some data
  EXPECT_EQ(KRawBufferLength, buffer.write(KRawBuffer, KRawBufferLength));
  EXPECT_EQ(KRawBufferLength, buffer.string().length());
  EXPECT_EQ(KRawBufferLength, buffer.size());

  // write even more data
  EXPECT_EQ(KRawBufferLength, buffer.write(KRawBuffer, KRawBufferLength));
  EXPECT_EQ(KRawBufferLength * 2, buffer.string().length());
  EXPECT_EQ(KRawBufferLength * 2, buffer.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringBufferTest, Clear)
{
  const char* KRawBuffer = "This is my test buffer";
  const s32 KRawBufferLength = strlen(KRawBuffer);

  StringBuffer buffer;

  // write some data
  EXPECT_EQ(KRawBufferLength, buffer.write(KRawBuffer, KRawBufferLength));
  EXPECT_EQ(KRawBufferLength, buffer.string().length());
  EXPECT_EQ(KRawBufferLength, buffer.size());

  // clear
  buffer.clear();
  EXPECT_EQ(0, buffer.string().length());
  EXPECT_EQ(0, buffer.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------