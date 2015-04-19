#include "DataStreamTestBase.h"

#include <gmock/gmock.h>

using namespace ::testing;

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class DataStreamTest : public DataStreamTestBase
                     , public ::testing::WithParamInterface<Endianness>
{
  protected:

    virtual void SetUp();
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DataStreamTest::SetUp()
{
  m_stream.setByteOrdering(GetParam());
  EXPECT_EQ(GetParam(), m_stream.byteOrdering());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(DataStreamTest, Read)
{
  // data
  const char* value = "This is some text";

  const s64 KDataLength = static_cast<s64>(strlen(value));

  char readBuffer[256];
  memset(readBuffer, 0, sizeof (readBuffer));

  // set expectations
  EXPECT_CALL(m_device, read(_, KDataLength))
    .WillOnce(DoAll(CopyToBufferAtArg0WithLengthAtArg1(value), 
                    Return(KDataLength)));

  // read
  EXPECT_EQ(KDataLength, m_stream.read(readBuffer, KDataLength));

  EXPECT_STREQ(value, readBuffer);
  EXPECT_TRUE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(DataStreamTest, FailedRead)
{
  // check if initially good
  EXPECT_TRUE(m_stream.isGood());

  // set expectations
  EXPECT_CALL(m_device, read(_, _))
    .WillOnce(Return(-1));

  // make a failed read 
  EXPECT_GT(0, m_stream.read(NULL, 1));

  // check if noticed
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(DataStreamTest, NoReadWhenFailed)
{
  makeDataStreamNotGood();

  // set expectations
  EXPECT_CALL(m_device, read(_, _))
    .Times(0);

  // make a read 
  EXPECT_GT(0, m_stream.read(NULL, 1));

  // check if still bad
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(DataStreamTest, Write)
{
  // data
  const char* value = "This is some text";

  const s64 KDataLength = static_cast<s64>(strlen(value));

  char writeBuffer[256];
  memset(writeBuffer, 0, sizeof (writeBuffer));

  // set expectations
  EXPECT_CALL(m_device, write(value, KDataLength))
    .WillOnce(DoAll(CopyFromBufferAtArg0WithLengthAtArg1(&writeBuffer), 
                    Return(KDataLength)));

  // read
  EXPECT_EQ(KDataLength, m_stream.write(value, KDataLength));

  EXPECT_STREQ(value, writeBuffer);
  EXPECT_TRUE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(DataStreamTest, FailedWrite)
{
  // check if initially good
  EXPECT_TRUE(m_stream.isGood());

  // set expectations
  EXPECT_CALL(m_device, write(_, _))
    .WillOnce(Return(-1));

  // make a failed write 
  EXPECT_GT(0, m_stream.write(NULL, 1));

  // check if noticed
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(DataStreamTest, NoWriteWhenFailed)
{
  makeDataStreamNotGood();

  // set expectations
  EXPECT_CALL(m_device, write(_, _))
    .Times(0);

  // make a read 
  EXPECT_GT(0, m_stream.write(NULL, 1));

  // check if still bad
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
INSTANTIATE_TEST_CASE_P(NonOperators, DataStreamTest, ::testing::Values(ELittleEndian, EBigEndian));
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
