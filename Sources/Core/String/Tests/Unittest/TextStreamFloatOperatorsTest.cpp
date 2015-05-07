#include "TextStreamTestBase.h"

#include <gmock/gmock.h>

using namespace ::testing;

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const s32 KMaxPrecision = 6;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
class TextStreamFloatOperatorsTest : public TextStreamTestBase
{
  protected:

    /*! Converts 32-bit float value to a string. */
    std::string convertToString(float32 value, s32 precision) const;
    /*! Converts 64-bit float value to a string. */
    std::string convertToString(float64 value, s32 precision) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::string TextStreamFloatOperatorsTest<float32>::convertToString(float32 value, s32 precision) const
{
  std::stringstream precisionBuffer;
  precisionBuffer << "%." << precision << "f";

  char buffer[32];
  sprintf(buffer, precisionBuffer.str().c_str(), value);

  return buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::string TextStreamFloatOperatorsTest<float64>::convertToString(float64 value, s32 precision) const
{
  std::stringstream precisionBuffer;
  precisionBuffer << "%." << precision << "f";

  char buffer[32];
  sprintf(buffer, precisionBuffer.str().c_str(), value);

  return buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TYPED_TEST_CASE_P(TextStreamFloatOperatorsTest);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TYPED_TEST_P(TextStreamFloatOperatorsTest, Float_Exact)
{
  // repeat for a number of different precisions
  for (s32 precision = 0; precision <= KMaxPrecision; ++precision)
  {
    // setup desired precision
    m_stream.setFloatPrecision(precision);

    // generate random data
    TypeParam value = random(1000);

    // create expectation
    const std::string KExpectedValue = convertToString(value, precision);

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

    EXPECT_EQ(KExpectedValue, convertToString(readValue, precision));
    // NOTE: difference in read and original value should not be greater than 1
    EXPECT_GE(1, fabs(readValue - value));
    EXPECT_TRUE(m_stream.isGood());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TYPED_TEST_P(TextStreamFloatOperatorsTest, Float_PrependedWhiteSpaces)
{
  // repeat for a number of different precisions
  for (s32 precision = 0; precision <= KMaxPrecision; ++precision)
  {
    // setup desired precision
    m_stream.setFloatPrecision(precision);

    // generate random data
    TypeParam value = random(1000);

    // create expectation
    const std::string KExpectedValue = convertToString(value, precision);

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

    EXPECT_EQ(KExpectedValue, convertToString(readValue, precision));
    // NOTE: difference in read and original value should not be greater than 1
    EXPECT_GE(1, fabs(readValue - value));
    EXPECT_TRUE(m_stream.isGood());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TYPED_TEST_P(TextStreamFloatOperatorsTest, Float_WhiteSpaceAfter)
{
  // repeat for a number of different precisions
  for (s32 precision = 0; precision <= KMaxPrecision; ++precision)
  {
    // setup desired precision
    m_stream.setFloatPrecision(precision);

    // generate random data
    TypeParam value = random(1000);

    // create expectation
    const std::string KExpectedValue = convertToString(value, precision);

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

    EXPECT_EQ(KExpectedValue, convertToString(readValue, precision));
    // NOTE: difference in read and original value should not be greater than 1
    EXPECT_GE(1, fabs(readValue - value));
    EXPECT_TRUE(m_stream.isGood());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TYPED_TEST_P(TextStreamFloatOperatorsTest, Float_FailedWrite)
{
  // check if initially good
  EXPECT_TRUE(m_stream.isGood());

  // set expectations
  EXPECT_CALL(m_device, write(_, _))
    .WillOnce(Return(-1));

  // make a failed write
  m_stream << TypeParam();

  // check if noticed
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TYPED_TEST_P(TextStreamFloatOperatorsTest, Float_FailedRead)
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
TYPED_TEST_P(TextStreamFloatOperatorsTest, Float_NoWriteWhenFailed)
{
  makeDataStreamNotGood();

  // set expectations
  EXPECT_CALL(m_device, write(_, _))
    .Times(0);

  // make a write 
  m_stream << TypeParam();

  // check if still bad
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TYPED_TEST_P(TextStreamFloatOperatorsTest, Float_NoReadWhenFailed)
{
  makeDataStreamNotGood();

  // set expectations
  EXPECT_CALL(m_device, read(_, _))
    .Times(0);

  // make read
  TypeParam value;
  m_stream >> value;

  // check if still bad
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
REGISTER_TYPED_TEST_CASE_P(TextStreamFloatOperatorsTest, Float_Exact,
                                                         Float_PrependedWhiteSpaces,
                                                         Float_WhiteSpaceAfter,
                                                         Float_FailedWrite,
                                                         Float_FailedRead,
                                                         Float_NoWriteWhenFailed,
                                                         Float_NoReadWhenFailed);

typedef ::testing::Types<float32, float64> FloatTypes;
INSTANTIATE_TYPED_TEST_CASE_P(Float, TextStreamFloatOperatorsTest, FloatTypes);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------