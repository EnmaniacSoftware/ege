#include "TextStreamTestBase.h"

#include <gmock/gmock.h>

using namespace ::testing;

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class TextStreamBooleanOperatorsTest : public TextStreamTestBase
                                     , public ::testing::WithParamInterface<bool>
{
  protected:

    /*! Converts boolean value to a string. */
    std::string convertToString(bool value) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::string TextStreamBooleanOperatorsTest::convertToString(bool value) const
{
  return value ? "true" : "false";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(TextStreamBooleanOperatorsTest, Bool_Exact)
{
  // generate random data
  const bool value = GetParam();

  std::string writeBuffer;

  // create expectation
  const std::string KExpectedValue = convertToString(value);

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
  bool readValue;
  m_stream >> readValue;

  EXPECT_EQ(value, readValue);
  EXPECT_TRUE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(TextStreamBooleanOperatorsTest, Bool_PrependedWhiteSpaces)
{
  // generate random data
  const bool value = GetParam();

  std::string writeBuffer;

  // create expectation
  const std::string KExpectedValue = convertToString(value);

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
  bool readValue;
  m_stream >> readValue;

  EXPECT_EQ(value, readValue);
  EXPECT_TRUE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(TextStreamBooleanOperatorsTest, Bool_WhiteSpaceAfter)
{
  // generate random data
  const bool value = GetParam();

  std::string writeBuffer;

  // create expectation
  const std::string KExpectedValue = convertToString(value);

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
  bool readValue;
  m_stream >> readValue;

  EXPECT_EQ(value, readValue);
  EXPECT_TRUE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(TextStreamBooleanOperatorsTest, FailedWrite)
{
  // check if initially good
  EXPECT_TRUE(m_stream.isGood());

  // set expectations
  EXPECT_CALL(m_device, write(_, _))
    .WillOnce(Return(-1));

  // make a failed write 
  m_stream << false;

  // check if noticed
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(TextStreamBooleanOperatorsTest, FailedRead)
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
TEST_P(TextStreamBooleanOperatorsTest, NoWriteWhenFailed)
{
  makeDataStreamNotGood();

  // set expectations
  EXPECT_CALL(m_device, write(_, _))
    .Times(0);

  // make a failed write 
  m_stream << false;

  // check if still bad
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(TextStreamBooleanOperatorsTest, NoReadWhenFailed)
{
  makeDataStreamNotGood();

  // set expectations
  EXPECT_CALL(m_device, read(_, _))
    .Times(0);

  // make a failed read
  bool value;
  m_stream >> value;

  // check if still bad
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
INSTANTIATE_TEST_CASE_P(Boolean, TextStreamBooleanOperatorsTest, ::testing::Values(true, false));
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
