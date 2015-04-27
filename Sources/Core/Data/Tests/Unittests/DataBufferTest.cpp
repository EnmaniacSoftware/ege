#include "TestFramework/Interface/TestBase.h"
#include <EGEDataBuffer.h>

using namespace ::testing;

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class DataBufferTest : public TestBase
{
  protected:
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DataBufferTest, Construct)
{
  DataBuffer buffer1;
  EXPECT_EQ(0, buffer1.size());
  EXPECT_EQ(0, buffer1.capacity());
  EXPECT_EQ(0, buffer1.writeOffset());
  EXPECT_EQ(0, buffer1.readOffset());
  EXPECT_EQ(NULL, buffer1.data());

  const s64 KBufferSize = 100;
  DataBuffer buffer2(KBufferSize);
  EXPECT_EQ(KBufferSize, buffer2.size());
  EXPECT_EQ(KBufferSize, buffer2.capacity());
  EXPECT_EQ(0, buffer2.writeOffset());
  EXPECT_EQ(0, buffer2.readOffset());
  EXPECT_NE(reinterpret_cast<void*>(NULL), buffer2.data());

  DataBuffer buffer3(buffer2);
  EXPECT_EQ(KBufferSize, buffer3.size());
  EXPECT_EQ(KBufferSize, buffer3.capacity());
  EXPECT_EQ(0, buffer3.writeOffset());
  EXPECT_EQ(0, buffer3.readOffset());
  EXPECT_NE(reinterpret_cast<void*>(NULL), buffer3.data());

  const s64 KRawBufferSize = 10;
  char rawBuffer[KRawBufferSize];
  DataBuffer buffer4(rawBuffer, KRawBufferSize);
  EXPECT_EQ(KRawBufferSize, buffer4.size());
  EXPECT_EQ(KRawBufferSize, buffer4.capacity());
  EXPECT_EQ(0, buffer4.writeOffset());
  EXPECT_EQ(0, buffer4.readOffset());
  EXPECT_EQ(rawBuffer, buffer4.data());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DataBufferTest, Size)
{
  DataBuffer buffer;
  EXPECT_EQ(0, buffer.size());
  EXPECT_EQ(0, buffer.capacity());

  // set size
  const s64 KSize = 1024;
  EXPECT_EQ(EGE_SUCCESS, buffer.setSize(KSize));
  EXPECT_EQ(KSize, buffer.size());
  EXPECT_EQ(KSize, buffer.capacity());

  // resize to greater value
  EXPECT_EQ(EGE_SUCCESS, buffer.setSize(KSize * 2));
  EXPECT_EQ(KSize * 2, buffer.size());
  EXPECT_EQ(KSize * 2, buffer.capacity());

  // resize to smaller value
  EXPECT_EQ(EGE_SUCCESS, buffer.setSize(KSize / 2));
  EXPECT_EQ(KSize / 2, buffer.size());
  EXPECT_EQ(KSize * 2, buffer.capacity());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DataBufferTest, SetSize_NotMutable)
{
  const s64 KBufferSize = 256;
  char rawBuffer[KBufferSize];

  DataBuffer buffer(rawBuffer, KBufferSize);
  EXPECT_EQ(KBufferSize, buffer.size());

  // try to resize to a greater value
  EXPECT_EQ(EGE_ERROR_NOT_SUPPORTED, buffer.setSize(KBufferSize * 2));
  
  // try to resize to a smaller value
  EXPECT_EQ(EGE_ERROR_NOT_SUPPORTED, buffer.setSize(KBufferSize / 2));

  // verify size and didnt change
  EXPECT_EQ(KBufferSize, buffer.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DataBufferTest, Capacity)
{
  DataBuffer buffer;
  EXPECT_EQ(0, buffer.capacity());

  // set capacity
  const s64 KCapacity = 1024;
  EXPECT_EQ(EGE_SUCCESS, buffer.setCapacity(KCapacity));
  EXPECT_EQ(KCapacity, buffer.capacity());

  // set capacity to a greater value
  EXPECT_EQ(EGE_SUCCESS, buffer.setCapacity(KCapacity * 2));
  EXPECT_EQ(KCapacity * 2, buffer.capacity());

  // set capacity to a smaller value
  EXPECT_EQ(EGE_SUCCESS, buffer.setCapacity(KCapacity / 2));
  EXPECT_EQ(KCapacity * 2, buffer.capacity());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DataBufferTest, SetCapacity_NotMutable)
{
  const s64 KBufferSize = 256;
  char rawBuffer[KBufferSize];

  DataBuffer buffer(rawBuffer, KBufferSize);
  EXPECT_EQ(KBufferSize, buffer.capacity());

  // try to increase capacity
  EXPECT_EQ(EGE_ERROR_NOT_SUPPORTED, buffer.setCapacity(KBufferSize * 2));
  
  // try to decrease capacity
  EXPECT_EQ(EGE_ERROR_NOT_SUPPORTED, buffer.setCapacity(KBufferSize / 2));

  // verify capacity and didnt change
  EXPECT_EQ(KBufferSize, buffer.capacity());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DataBufferTest, Data)
{
  const s64 KBufferSize = 256;
  char rawBuffer[KBufferSize];

  DataBuffer buffer(rawBuffer, KBufferSize);

  // get pointer to the begining of the buffer
  EXPECT_EQ(rawBuffer, buffer.data(0));

  // get some pointer within limits
  EXPECT_EQ(rawBuffer + KBufferSize / 2, buffer.data(KBufferSize / 2));

  // get pointer with negative offset
  EXPECT_EQ(NULL, buffer.data(-100));

  // get pointer outside the capacity limit
  EXPECT_EQ(NULL, buffer.data(KBufferSize));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DataBufferTest, Read_Buffer)
{
  const char* KRawBuffer = "This is my test buffer";
  const s64 KRawBufferLength = strlen(KRawBuffer);

  DataBuffer buffer(KRawBuffer, KRawBufferLength);
  DataBuffer readBuffer;

  EXPECT_EQ(0, buffer.readOffset());
  EXPECT_EQ(0, buffer.writeOffset());
  EXPECT_EQ(0, readBuffer.readOffset());
  EXPECT_EQ(0, readBuffer.writeOffset());

  // read 5 bytes
  EXPECT_EQ(5, buffer.read(readBuffer, 5));
  
  EXPECT_EQ(5, buffer.readOffset());
  EXPECT_EQ(0, buffer.writeOffset());
  EXPECT_EQ(0, readBuffer.readOffset());
  EXPECT_EQ(5, readBuffer.writeOffset());
  EXPECT_EQ(5, readBuffer.size());
  EXPECT_EQ(0, memcmp(KRawBuffer, readBuffer.data(), 5));

  // read another 10 bytes
  EXPECT_EQ(10, buffer.read(readBuffer, 10));
  
  EXPECT_EQ(15, buffer.readOffset());
  EXPECT_EQ(0, buffer.writeOffset());
  EXPECT_EQ(0, readBuffer.readOffset());
  EXPECT_EQ(15, readBuffer.writeOffset());
  EXPECT_EQ(15, readBuffer.size());
  EXPECT_EQ(0, memcmp(KRawBuffer, readBuffer.data(), 15));

  // read another 10 bytes
  // NOTE: expect end of the buffer
  const s64 KExpectedBytesRead = KRawBufferLength - buffer.readOffset();
  EXPECT_EQ(KExpectedBytesRead, buffer.read(readBuffer, 10));
  
  EXPECT_EQ(KRawBufferLength, buffer.readOffset());
  EXPECT_EQ(0, buffer.writeOffset());
  EXPECT_EQ(0, readBuffer.readOffset());
  EXPECT_EQ(KRawBufferLength, readBuffer.writeOffset());
  EXPECT_EQ(KRawBufferLength, readBuffer.size());
  EXPECT_EQ(0, memcmp(KRawBuffer, readBuffer.data(), static_cast<size_t>(KRawBufferLength)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DataBufferTest, Write_Buffer)
{
  const char* KRawBuffer = "This is my test buffer";
  const s64 KRawBufferLength = strlen(KRawBuffer);

  DataBuffer buffer(KRawBuffer, KRawBufferLength);
  DataBuffer writeBuffer;

  EXPECT_EQ(0, buffer.readOffset());
  EXPECT_EQ(0, buffer.writeOffset());
  EXPECT_EQ(0, writeBuffer.readOffset());
  EXPECT_EQ(0, writeBuffer.writeOffset());

  // write 5 bytes
  EXPECT_EQ(5, writeBuffer.write(buffer, 5));

  EXPECT_EQ(5, buffer.readOffset());
  EXPECT_EQ(0, buffer.writeOffset());
  EXPECT_EQ(0, writeBuffer.readOffset());
  EXPECT_EQ(5, writeBuffer.writeOffset());
  EXPECT_EQ(5, writeBuffer.size());
  EXPECT_EQ(0, memcmp(KRawBuffer, writeBuffer.data(), 5));

  // write another 10 bytes
  EXPECT_EQ(10, writeBuffer.write(buffer, 10));

  EXPECT_EQ(15, buffer.readOffset());
  EXPECT_EQ(0, buffer.writeOffset());
  EXPECT_EQ(0, writeBuffer.readOffset());
  EXPECT_EQ(15, writeBuffer.writeOffset());
  EXPECT_EQ(15, writeBuffer.size());
  EXPECT_EQ(0, memcmp(KRawBuffer, writeBuffer.data(), 15));

  // write another 10 bytes
  // NOTE: expect end of the buffer
  const s64 KExpectedBytesRead = KRawBufferLength - writeBuffer.writeOffset();
  EXPECT_EQ(KExpectedBytesRead, writeBuffer.write(buffer, 10));

  EXPECT_EQ(KRawBufferLength, buffer.readOffset());
  EXPECT_EQ(0, buffer.writeOffset());
  EXPECT_EQ(0, writeBuffer.readOffset());
  EXPECT_EQ(KRawBufferLength, writeBuffer.writeOffset());
  EXPECT_EQ(KRawBufferLength, writeBuffer.size());
  EXPECT_EQ(0, memcmp(KRawBuffer, writeBuffer.data(), static_cast<size_t>(KRawBufferLength)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DataBufferTest, WriteAll_Buffer)
{
  const char* KRawBuffer = "This is my test buffer";
  const s64 KRawBufferLength = strlen(KRawBuffer);

  DataBuffer buffer(KRawBuffer, KRawBufferLength);
  DataBuffer writeBuffer;

  EXPECT_EQ(0, buffer.readOffset());
  EXPECT_EQ(0, buffer.writeOffset());
  EXPECT_EQ(0, writeBuffer.readOffset());
  EXPECT_EQ(0, writeBuffer.writeOffset());

  // write all bytes
  EXPECT_EQ(KRawBufferLength, writeBuffer.write(buffer));

  EXPECT_EQ(KRawBufferLength, buffer.readOffset());
  EXPECT_EQ(0, buffer.writeOffset());
  EXPECT_EQ(0, writeBuffer.readOffset());
  EXPECT_EQ(KRawBufferLength, writeBuffer.writeOffset());
  EXPECT_EQ(KRawBufferLength, writeBuffer.size());
  EXPECT_EQ(0, memcmp(KRawBuffer, writeBuffer.data(), static_cast<size_t>(KRawBufferLength)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DataBufferTest, Write_Buffer_NotMutable)
{
  const char* KRawBuffer = "This is my test buffer";
  const s64 KRawBufferLength = strlen(KRawBuffer);

  DataBuffer buffer(KRawBuffer, KRawBufferLength);
  DataBuffer writeBuffer(KRawBuffer, KRawBufferLength);

  // try to write
  EXPECT_GT(0, writeBuffer.write(buffer));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DataBufferTest, Read_Raw)
{
  const char* KRawBuffer = "This is my test buffer";
  const s64 KRawBufferLength = strlen(KRawBuffer);

  char writeBuffer[256];
  memset(writeBuffer, 0, sizeof (writeBuffer));

  DataBuffer buffer(KRawBuffer, KRawBufferLength);

  EXPECT_EQ(0, buffer.readOffset());
  EXPECT_EQ(0, buffer.writeOffset());

  // read 5 bytes
  EXPECT_EQ(5, buffer.read(writeBuffer, 5));
  
  EXPECT_EQ(5, buffer.readOffset());
  EXPECT_EQ(0, buffer.writeOffset());
  EXPECT_EQ(0, memcmp(KRawBuffer, writeBuffer, 5));

  // read another 10 bytes
  EXPECT_EQ(10, buffer.read(writeBuffer + 5, 10));
  
  EXPECT_EQ(15, buffer.readOffset());
  EXPECT_EQ(0, buffer.writeOffset());
  EXPECT_EQ(0, memcmp(KRawBuffer, writeBuffer, 15));

  // read another 10 bytes
  // NOTE: expect end of the buffer
  const s64 KExpectedBytesRead = KRawBufferLength - buffer.readOffset();
  EXPECT_EQ(KExpectedBytesRead, buffer.read(writeBuffer + 15, 10));
  
  EXPECT_EQ(KRawBufferLength, buffer.readOffset());
  EXPECT_EQ(0, memcmp(KRawBuffer, writeBuffer, static_cast<size_t>(KRawBufferLength)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DataBufferTest, Write_Raw)
{
  const char* KRawBuffer = "This is my test buffer";
  const s64 KRawBufferLength = strlen(KRawBuffer);

  DataBuffer buffer;

  EXPECT_EQ(0, buffer.readOffset());
  EXPECT_EQ(0, buffer.writeOffset());

  // write 5 bytes
  EXPECT_EQ(5, buffer.write(KRawBuffer, 5));

  EXPECT_EQ(0, buffer.readOffset());
  EXPECT_EQ(5, buffer.writeOffset());
  EXPECT_EQ(5, buffer.size());
  EXPECT_EQ(0, memcmp(KRawBuffer, buffer.data(), 5));

  // write another 10 bytes
  EXPECT_EQ(10, buffer.write(KRawBuffer + 5, 10));

  EXPECT_EQ(0, buffer.readOffset());
  EXPECT_EQ(15, buffer.writeOffset());
  EXPECT_EQ(15, buffer.size());
  EXPECT_EQ(0, memcmp(KRawBuffer, buffer.data(), 15));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DataBufferTest, Write_Raw_NotMutable)
{
  const char* KRawBuffer = "This is my test buffer";
  const s64 KRawBufferLength = strlen(KRawBuffer);

  DataBuffer writeBuffer(KRawBuffer, KRawBufferLength);

  // try to write
  EXPECT_GT(0, writeBuffer.write(KRawBuffer, KRawBufferLength));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DataBufferTest, Clear)
{
  const char* KRawBuffer = "This is my test buffer";
  const s64 KRawBufferLength = strlen(KRawBuffer);

  DataBuffer buffer;
  EXPECT_EQ(KRawBufferLength, buffer.write(KRawBuffer, KRawBufferLength));
  EXPECT_EQ(KRawBufferLength, buffer.size());

  buffer.clear();
  EXPECT_EQ(0, buffer.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DataBufferTest, Clear_NotMutable)
{
  const char* KRawBuffer = "This is my test buffer";
  const s64 KRawBufferLength = strlen(KRawBuffer);

  DataBuffer buffer(KRawBuffer, KRawBufferLength);
  EXPECT_EQ(KRawBufferLength, buffer.size());

  buffer.clear();

  EXPECT_EQ(0, buffer.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------