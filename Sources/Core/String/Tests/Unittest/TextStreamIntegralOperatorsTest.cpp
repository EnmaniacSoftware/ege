#include "TextStreamTestBase.h"
#include <string>
#include <gmock/gmock.h>

using namespace ::testing;

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
class TextStreamIntegralOperatorsTest : public TextStreamTestBase
{
  public:

    /*! Converts value to a string. */
    std::string convertToString(T value) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
std::string TextStreamIntegralOperatorsTest<T>::convertToString(T value) const
{
  std::stringstream ss;
  ss << value;
  return ss.str();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::string TextStreamIntegralOperatorsTest<u8>::convertToString(u8 value) const
{
  std::stringstream ss;
  ss << static_cast<int>(value);
  return ss.str();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::string TextStreamIntegralOperatorsTest<s8>::convertToString(s8 value) const
{
  std::stringstream ss;
  ss << static_cast<int>(value);
  return ss.str();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TYPED_TEST_CASE_P(TextStreamIntegralOperatorsTest);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TYPED_TEST_P(TextStreamIntegralOperatorsTest, WriteReadSequence_Exact)
{
  const s64 KDataLength = sizeof(TypeParam);

  // generate random data
  TypeParam value = 0;
  for (s64 i = 0; i < KDataLength; ++i)
  {
    const u8 byte = static_cast<u8>(rand() & 0xff);

    value |= static_cast<TypeParam>(byte) << (i * 8);
  }

  // create expectation
  const std::string KExpectedValue = convertToString(value);

  std::string writeBuffer;

  // set expectations
  EXPECT_CALL(m_device, write(_, KExpectedValue.size()))
    .WillOnce(DoAll(AppendFromBufferAtArg0WithLengthAtArg1(&writeBuffer), 
                    Return(KExpectedValue.size())));

  Expectation successfullReads = EXPECT_CALL(m_device, read(_, 1))
                                   .Times(KExpectedValue.size())
                                   .WillRepeatedly(DoAll(CopyToBufferAtArg0WithLengthAtArg1AndRemove(&writeBuffer), 
                                                         Return(1)));
  EXPECT_CALL(m_device, read(_, 1))
    .After(successfullReads)
    .WillOnce(Return(0));

  // write
  m_stream << value;

  EXPECT_EQ(KExpectedValue, writeBuffer);
  EXPECT_TRUE(m_stream.isGood());

  // read
  TypeParam readValue;
  m_stream >> readValue;

  EXPECT_EQ(value, readValue);
  EXPECT_TRUE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TYPED_TEST_P(TextStreamIntegralOperatorsTest, WriteReadSequence_PrependedWhiteSpaces)
{
  const s64 KDataLength = sizeof(TypeParam);

  // generate random data
  TypeParam value = 0;
  for (s64 i = 0; i < KDataLength; ++i)
  {
    const u8 byte = static_cast<u8>(rand() & 0xff);

    value |= static_cast<TypeParam>(byte) << (i * 8);
  }

  // create expectation
  const std::string KExpectedValue = convertToString(value);

  std::string writeBuffer;

  // set expectations
  EXPECT_CALL(m_device, write(_, KExpectedValue.size()))
    .WillOnce(DoAll(AppendFromBufferAtArg0WithLengthAtArg1(&writeBuffer), 
                    Return(KExpectedValue.size())));

  Expectation successfullReads = EXPECT_CALL(m_device, read(_, 1))
                                   .Times(KExpectedValue.size() + KWhiteSpaces.size())
                                   .WillRepeatedly(DoAll(CopyToBufferAtArg0WithLengthAtArg1AndRemove(&writeBuffer), 
                                                         Return(1)));
  EXPECT_CALL(m_device, read(_, 1))
    .After(successfullReads)
    .WillOnce(Return(0));

  // write
  m_stream << value;

  EXPECT_EQ(KExpectedValue, writeBuffer);
  EXPECT_TRUE(m_stream.isGood());

  // prepend read buffer with whitespaces
  writeBuffer = KWhiteSpaces + writeBuffer;

  // read
  TypeParam readValue;
  m_stream >> readValue;

  EXPECT_EQ(value, readValue);
  EXPECT_TRUE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TYPED_TEST_P(TextStreamIntegralOperatorsTest, WriteReadSequence_WhiteSpaceAfter)
{
  const s64 KDataLength = sizeof(TypeParam);

  // generate random data
  TypeParam value = 0;
  for (s64 i = 0; i < KDataLength; ++i)
  {
    const u8 byte = static_cast<u8>(rand() & 0xff);

    value |= static_cast<TypeParam>(byte) << (i * 8);
  }

  // create expectation
  const std::string KExpectedValue = convertToString(value);

  std::string writeBuffer;

  // set expectations
  EXPECT_CALL(m_device, write(_, KExpectedValue.size()))
    .WillOnce(DoAll(AppendFromBufferAtArg0WithLengthAtArg1(&writeBuffer), 
                    Return(KExpectedValue.size())));

  EXPECT_CALL(m_device, read(_, 1))
    .Times(KExpectedValue.size() + 1)
    .WillRepeatedly(DoAll(CopyToBufferAtArg0WithLengthAtArg1AndRemove(&writeBuffer), 
                          Return(1)));

  // write
  m_stream << value;

  EXPECT_EQ(KExpectedValue, writeBuffer);
  EXPECT_TRUE(m_stream.isGood());

  // prepend read buffer with whitespaces
  writeBuffer += KWhiteSpaces;

  // read
  TypeParam readValue;
  m_stream >> readValue;

  EXPECT_EQ(value, readValue);
  EXPECT_TRUE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TYPED_TEST_P(TextStreamIntegralOperatorsTest, FailedWrite)
{
  // check if initially good
  EXPECT_TRUE(m_stream.isGood());

  // set expectations
  EXPECT_CALL(m_device, write(_, _))
    .WillOnce(Return(-1));

  // make a failed write 
  m_stream << static_cast<TypeParam>(0);

  // check if noticed
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TYPED_TEST_P(TextStreamIntegralOperatorsTest, FailedRead)
{
  // check if initially good
  EXPECT_TRUE(m_stream.isGood());

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
TYPED_TEST_P(TextStreamIntegralOperatorsTest, NoWriteWhenFailed)
{
  makeDataStreamNotGood();

  // set expectations
  EXPECT_CALL(m_device, write(_, _))
    .Times(0);

  // make a failed write 
  m_stream << static_cast<TypeParam>(0);

  // check if still bad
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TYPED_TEST_P(TextStreamIntegralOperatorsTest, NoReadWhenFailed)
{
  makeDataStreamNotGood();

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
REGISTER_TYPED_TEST_CASE_P(TextStreamIntegralOperatorsTest, WriteReadSequence_Exact,
                                                            WriteReadSequence_PrependedWhiteSpaces,
                                                            WriteReadSequence_WhiteSpaceAfter,
                                                            FailedWrite,
                                                            FailedRead,
                                                            NoWriteWhenFailed,
                                                            NoReadWhenFailed);

typedef ::testing::Types<u8, s8, u16, s16, u32, s32, s64, u64> IntegralTypes;
INSTANTIATE_TYPED_TEST_CASE_P(Integral, TextStreamIntegralOperatorsTest, IntegralTypes);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
