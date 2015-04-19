#include "DataStreamTestBase.h"

#include <gmock/gmock.h>

using namespace ::testing;

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Action which increments a given value.
 *  @param  value Pointer to value to be incremented.
 */
ACTION_P(Increment, value) 
{ 
  s32* val = value;
  (*val)++;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Action which copies data from a buffer into method's buffer.
 *  @param  arg0      Destination buffer.
 *  @param  arg1      Number of bytes to copy.
 *  @param  value     Source buffer.
 *  @param  fromIndex Starting index within source buffer copy should start from.
 */
ACTION_P2(CopyToBufferAtArg0WithLengthAtArg1FromIndex, value, fromIndex) 
{ 
  s32* index = fromIndex;
  memcpy(arg0, value + *index, static_cast<size_t>(arg1));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class DataStreamNonIntegralOperatorsTest : public DataStreamTestBase
                                         , public ::testing::WithParamInterface<Endianness>
{
  protected:

    virtual void SetUp();
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DataStreamNonIntegralOperatorsTest::SetUp()
{
  m_stream.setByteOrdering(GetParam());
  EXPECT_EQ(GetParam(), m_stream.byteOrdering());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(DataStreamNonIntegralOperatorsTest, Bool)
{
  const s64 KDataLength = sizeof(bool);

  // generate random data
  const bool value = (rand() & 0x1) == 0x1;

  bool writeBuffer = 0;
  bool readValue;

  // set expectations
  EXPECT_CALL(m_device, write(_, KDataLength))
    .WillOnce(DoAll(CopyFromBufferAtArg0WithLengthAtArg1(&writeBuffer), 
                    Return(KDataLength)));
  EXPECT_CALL(m_device, read(_, KDataLength))
    .WillOnce(DoAll(CopyToBufferAtArg0WithLengthAtArg1(&writeBuffer), 
                    Return(KDataLength)));

  // write
  m_stream << value;

  EXPECT_EQ(value, writeBuffer);
  EXPECT_TRUE(m_stream.isGood());

  // read
  m_stream >> readValue;

  EXPECT_EQ(value, readValue);
  EXPECT_TRUE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(DataStreamNonIntegralOperatorsTest, Bool_FailedWrite)
{
  // check if initially good
  EXPECT_TRUE(m_stream.isGood());

  // set expectations
  EXPECT_CALL(m_device, write(_, _))
    .WillOnce(Return(-1));

  // make a failed write 
  m_stream << true;

  // check if noticed
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(DataStreamNonIntegralOperatorsTest, Bool_FailedRead)
{
  // check if initially good
  EXPECT_TRUE(m_stream.isGood());

  // set expectations
  EXPECT_CALL(m_device, read(_, _))
    .WillOnce(Return(-1));

  // make a failed read 
  bool value;
  m_stream >> value;

  // check if noticed
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(DataStreamNonIntegralOperatorsTest, Bool_NoWriteWhenFailed)
{
  makeDataStreamNotGood();

  // set expectations
  EXPECT_CALL(m_device, write(_, _))
    .Times(0);

  // make a write 
  m_stream << true;

  // check if still bad
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(DataStreamNonIntegralOperatorsTest, Bool_NoReadWhenFailed)
{
  makeDataStreamNotGood();

  // set expectations
  EXPECT_CALL(m_device, read(_, _))
    .Times(0);

  // make read
  bool value;
  m_stream >> value;

  // check if still bad
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(DataStreamNonIntegralOperatorsTest, Float32)
{
  const s64 KDataLength = sizeof(float32);

  // generate random data
  const float32 value = random(100.0f);

  float32 writeBuffer = 0;
  float32 readValue;

  // set expectations
  EXPECT_CALL(m_device, write(_, KDataLength))
    .WillOnce(DoAll(CopyFromBufferAtArg0WithLengthAtArg1(&writeBuffer), 
                    Return(KDataLength)));
  EXPECT_CALL(m_device, read(_, KDataLength))
    .WillOnce(DoAll(CopyToBufferAtArg0WithLengthAtArg1(&writeBuffer), 
                    Return(KDataLength)));

  // write
  m_stream << value;

  EXPECT_EQ(value, writeBuffer);
  EXPECT_TRUE(m_stream.isGood());

  // read
  m_stream >> readValue;

  EXPECT_EQ(value, readValue);
  EXPECT_TRUE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(DataStreamNonIntegralOperatorsTest, Float32_FailedWrite)
{
  // check if initially good
  EXPECT_TRUE(m_stream.isGood());

  // set expectations
  EXPECT_CALL(m_device, write(_, _))
    .WillOnce(Return(-1));

  // make a failed write 
  m_stream << 0.0f;

  // check if noticed
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(DataStreamNonIntegralOperatorsTest, Float32_FailedRead)
{
  // check if initially good
  EXPECT_TRUE(m_stream.isGood());

  // set expectations
  EXPECT_CALL(m_device, read(_, _))
    .WillOnce(Return(-1));

  // make a failed read 
  float32 value;
  m_stream >> value;

  // check if noticed
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(DataStreamNonIntegralOperatorsTest, Float32_NoWriteWhenFailed)
{
  makeDataStreamNotGood();

  // set expectations
  EXPECT_CALL(m_device, write(_, _))
    .Times(0);

  // make a write 
  m_stream << 0.0f;

  // check if still bad
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(DataStreamNonIntegralOperatorsTest, Float32_NoReadWhenFailed)
{
  makeDataStreamNotGood();

  // set expectations
  EXPECT_CALL(m_device, read(_, _))
    .Times(0);

  // make read
  float32 value;
  m_stream >> value;

  // check if still bad
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(DataStreamNonIntegralOperatorsTest, Float64)
{
  const s64 KDataLength = sizeof(float64);

  // generate random data
  const float64 value = random(100.0f);

  float64 writeBuffer = 0;
  float64 readValue;

  // set expectations
  EXPECT_CALL(m_device, write(_, KDataLength))
    .WillOnce(DoAll(CopyFromBufferAtArg0WithLengthAtArg1(&writeBuffer), 
                    Return(KDataLength)));
  EXPECT_CALL(m_device, read(_, KDataLength))
    .WillOnce(DoAll(CopyToBufferAtArg0WithLengthAtArg1(&writeBuffer), 
                    Return(KDataLength)));

  // write
  m_stream << value;

  EXPECT_EQ(value, writeBuffer);
  EXPECT_TRUE(m_stream.isGood());

  // read
  m_stream >> readValue;

  EXPECT_EQ(value, readValue);
  EXPECT_TRUE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(DataStreamNonIntegralOperatorsTest, Float64_FailedWrite)
{
  // check if initially good
  EXPECT_TRUE(m_stream.isGood());

  // set expectations
  EXPECT_CALL(m_device, write(_, _))
    .WillOnce(Return(-1));

  // make a failed write 
  m_stream << 0.0;

  // check if noticed
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(DataStreamNonIntegralOperatorsTest, Float64_FailedRead)
{
  // check if initially good
  EXPECT_TRUE(m_stream.isGood());

  // set expectations
  EXPECT_CALL(m_device, read(_, _))
    .WillOnce(Return(-1));

  // make a failed read 
  float64 value;
  m_stream >> value;

  // check if noticed
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(DataStreamNonIntegralOperatorsTest, Float64_NoWriteWhenFailed)
{
  makeDataStreamNotGood();

  // set expectations
  EXPECT_CALL(m_device, write(_, _))
    .Times(0);

  // make a write 
  m_stream << 0.0;

  // check if still bad
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(DataStreamNonIntegralOperatorsTest, Float64_NoReadWhenFailed)
{
  makeDataStreamNotGood();

  // set expectations
  EXPECT_CALL(m_device, read(_, _))
    .Times(0);

  // make read
  float64 value;
  m_stream >> value;

  // check if still bad
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(DataStreamNonIntegralOperatorsTest, CString)
{
  // data
  const char* value = "This is some text";

  const s64 KDataLength = static_cast<s64>(strlen(value)) + 1;

  char writeBuffer[256];
  char readValue[256];

  s32 readIndex = 0;

  // set expectations
  EXPECT_CALL(m_device, write(_, KDataLength))
    .WillOnce(DoAll(CopyFromBufferAtArg0WithLengthAtArg1(&writeBuffer), 
                    Return(KDataLength)));
  EXPECT_CALL(m_device, read(_, 1))
    .Times(static_cast<int>(KDataLength))
    .WillRepeatedly(DoAll(CopyToBufferAtArg0WithLengthAtArg1FromIndex(writeBuffer, &readIndex),
                          Increment(&readIndex),
                          Return(1)));

  // write
  m_stream << value;

  EXPECT_STREQ(value, writeBuffer);
  EXPECT_TRUE(m_stream.isGood());

  // read
  m_stream >> readValue;

  EXPECT_STREQ(value, readValue);
  EXPECT_TRUE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(DataStreamNonIntegralOperatorsTest, CString_FailedWrite)
{
  // check if initially good
  EXPECT_TRUE(m_stream.isGood());

  // set expectations
  EXPECT_CALL(m_device, write(_, _))
    .WillOnce(Return(-1));

  // make a failed write 
  m_stream << "Some dummy text";

  // check if noticed
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(DataStreamNonIntegralOperatorsTest, CString_FailedRead)
{
  // check if initially good
  EXPECT_TRUE(m_stream.isGood());

  // set expectations
  EXPECT_CALL(m_device, read(_, _))
    .WillOnce(Return(-1));

  // make a failed read 
  char value[64];
  m_stream >> value;

  // check if noticed
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(DataStreamNonIntegralOperatorsTest, CString_NoWriteWhenFailed)
{
  makeDataStreamNotGood();

  // set expectations
  EXPECT_CALL(m_device, write(_, _))
    .Times(0);

  // make a write 
  m_stream << "This is dummy text";

  // check if still bad
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(DataStreamNonIntegralOperatorsTest, CString_NoReadWhenFailed)
{
  makeDataStreamNotGood();

  // set expectations
  EXPECT_CALL(m_device, read(_, _))
    .Times(0);

  // make read
  char value[64];
  m_stream >> value;

  // check if still bad
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
INSTANTIATE_TEST_CASE_P(NonIntegral, DataStreamNonIntegralOperatorsTest, ::testing::Values(ELittleEndian, EBigEndian));
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
