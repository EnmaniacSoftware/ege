#include "DataStreamTestBase.h"

#include <gmock/gmock.h>

using namespace ::testing;

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
class DataStreamIntegralOperatorsTest : public DataStreamTestBase
{
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TYPED_TEST_CASE_P(DataStreamIntegralOperatorsTest);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TYPED_TEST_P(DataStreamIntegralOperatorsTest, WriteReadSequence_LittleEndian)
{
  const s64 KDataLength = sizeof(TypeParam);

  // set endiannes
  m_stream.setByteOrdering(ELittleEndian);

  // generate random data
  TypeParam value = 0;
  for (s64 i = 0; i < KDataLength; ++i)
  {
    const u8 byte = static_cast<u8>(rand() & 0xff);

    value |= static_cast<TypeParam>(byte) << (i * 8);
  }

  TypeParam writeBuffer = 0;
  TypeParam readValue;

  // set expectations
  EXPECT_CALL(m_device, write(_, KDataLength))
    .WillOnce(DoAll(CopyFromBufferAtArg0WithLengthAtArg1(&writeBuffer), 
                    Return(KDataLength)));
  EXPECT_CALL(m_device, read(_, KDataLength))
    .WillOnce(DoAll(CopyToBufferAtArg0WithLengthAtArg1(&writeBuffer), 
                    Return(KDataLength)));

  // write
  m_stream << value;

  EXPECT_EQ(value, reverseByteOrder(writeBuffer, true));
  EXPECT_TRUE(m_stream.isGood());

  // read
  m_stream >> readValue;

  EXPECT_EQ(value, readValue);
  EXPECT_TRUE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TYPED_TEST_P(DataStreamIntegralOperatorsTest, WriteReadSequence_BigEndian)
{
  const s64 KDataLength = sizeof(TypeParam);

  // set endiannes
  m_stream.setByteOrdering(EBigEndian);

  // generate random data
  TypeParam value = 0;
  for (s64 i = 0; i < KDataLength; ++i)
  {
    const u8 byte = static_cast<u8>(rand() & 0xff);

    value |= static_cast<TypeParam>(byte) << (i * 8);
  }

  TypeParam writeBuffer = 0;
  TypeParam readValue;

  // set expectations
  EXPECT_CALL(m_device, write(_, KDataLength))
    .WillOnce(DoAll(CopyFromBufferAtArg0WithLengthAtArg1(&writeBuffer), // writeBuffer contains data in stream's endianess
                    Return(KDataLength)));
  EXPECT_CALL(m_device, read(_, KDataLength))
    .WillOnce(DoAll(CopyToBufferAtArg0WithLengthAtArg1(&writeBuffer),   // initially provides data in stream's endianess
                    Return(KDataLength)));

  // write
  m_stream << value;

  EXPECT_EQ(value, reverseByteOrder(writeBuffer, true));
  EXPECT_TRUE(m_stream.isGood());

  // read
  m_stream >> readValue;

  EXPECT_EQ(value, readValue);
  EXPECT_TRUE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TYPED_TEST_P(DataStreamIntegralOperatorsTest, FailedWrite_LittleEndian)
{
  // check if initially good
  EXPECT_TRUE(m_stream.isGood());

  // set endiannes
  m_stream.setByteOrdering(ELittleEndian);

  // set expectations
  EXPECT_CALL(m_device, write(_, _))
    .WillOnce(Return(-1));

  // make a failed write 
  m_stream << static_cast<TypeParam>(0);

  // check if noticed
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TYPED_TEST_P(DataStreamIntegralOperatorsTest, FailedWrite_BigEndian)
{
  // check if initially good
  EXPECT_TRUE(m_stream.isGood());

  // set endiannes
  m_stream.setByteOrdering(EBigEndian);

  // set expectations
  EXPECT_CALL(m_device, write(_, _))
    .WillOnce(Return(-1));

  // make a failed write 
  m_stream << static_cast<TypeParam>(0);

  // check if noticed
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TYPED_TEST_P(DataStreamIntegralOperatorsTest, FailedRead_LittleEndian)
{
  // check if initially good
  EXPECT_TRUE(m_stream.isGood());

  // set endiannes
  m_stream.setByteOrdering(ELittleEndian);

  // set expectations
  EXPECT_CALL(m_device, read(_, _))
    .WillOnce(Return(-1));

  // make a failed read
  TypeParam value;
  m_stream >> value;

  // check if noticed
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TYPED_TEST_P(DataStreamIntegralOperatorsTest, FailedRead_BigEndian)
{
  // check if initially good
  EXPECT_TRUE(m_stream.isGood());

  // set endiannes
  m_stream.setByteOrdering(EBigEndian);

  // set expectations
  EXPECT_CALL(m_device, read(_, _))
    .WillOnce(Return(-1));

  // make a failed read
  TypeParam value;
  m_stream >> value;

  // check if noticed
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TYPED_TEST_P(DataStreamIntegralOperatorsTest, NoWriteWhenFailed_LittleEndian)
{
  makeDataStreamNotGood();

  // set endiannes
  m_stream.setByteOrdering(ELittleEndian);

  // set expectations
  EXPECT_CALL(m_device, write(_, _))
    .Times(0);

  // make a failed write 
  m_stream << static_cast<TypeParam>(0);

  // check if still bad
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TYPED_TEST_P(DataStreamIntegralOperatorsTest, NoWriteWhenFailed_BigEndian)
{
  makeDataStreamNotGood();

  // set endiannes
  m_stream.setByteOrdering(EBigEndian);

  // set expectations
  EXPECT_CALL(m_device, write(_, _))
    .Times(0);

  // make a failed write 
  m_stream << static_cast<TypeParam>(0);

  // check if still bad
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TYPED_TEST_P(DataStreamIntegralOperatorsTest, NoReadWhenFailed_LittleEndian)
{
  makeDataStreamNotGood();

  // set endiannes
  m_stream.setByteOrdering(ELittleEndian);

  // set expectations
  EXPECT_CALL(m_device, read(_, _))
    .Times(0);

  // make a failed read
  TypeParam value;
  m_stream >> value;

  // check if still bad
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TYPED_TEST_P(DataStreamIntegralOperatorsTest, NoReadWhenFailed_BigEndian)
{
  makeDataStreamNotGood();

  // set endiannes
  m_stream.setByteOrdering(EBigEndian);

  // set expectations
  EXPECT_CALL(m_device, read(_, _))
    .Times(0);

  // make a failed read
  TypeParam value;
  m_stream >> value;

  // check if still bad
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
REGISTER_TYPED_TEST_CASE_P(DataStreamIntegralOperatorsTest, WriteReadSequence_LittleEndian, WriteReadSequence_BigEndian, 
                                                            FailedWrite_LittleEndian, FailedWrite_BigEndian,
                                                            FailedRead_LittleEndian, FailedRead_BigEndian,
                                                            NoWriteWhenFailed_LittleEndian, NoWriteWhenFailed_BigEndian,
                                                            NoReadWhenFailed_LittleEndian, NoReadWhenFailed_BigEndian);

typedef ::testing::Types<u8, s8, u16, s16, u32, s32, s64, u64> IntegralTypes;
INSTANTIATE_TYPED_TEST_CASE_P(Integral, DataStreamIntegralOperatorsTest, IntegralTypes);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

