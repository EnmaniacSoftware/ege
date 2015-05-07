#include "TextStreamTestBase.h"

#include <gmock/gmock.h>

using namespace ::testing;

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class TextStreamStringOperatorsTest : public TextStreamTestBase
{
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TextStreamStringOperatorsTest, CString_Exact)
{
  // data
  const std::string KExpectedValue = "This+is-some_text_no_whitespaces";

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
  m_stream << KExpectedValue.c_str();

  EXPECT_EQ(KExpectedValue, writeBuffer);
  EXPECT_TRUE(m_stream.isGood());

  // read
  char readValue[256];
  m_stream >> readValue;

  EXPECT_STREQ(KExpectedValue.c_str(), readValue);
  EXPECT_TRUE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TextStreamStringOperatorsTest, CString_PrependedWhiteSpaces)
{
  // data
  const std::string KExpectedValue = "This+is-some_text_no_whitespaces";

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
  m_stream << KExpectedValue.c_str();

  EXPECT_EQ(KExpectedValue, writeBuffer);
  EXPECT_TRUE(m_stream.isGood());

  // prepend read buffer with whitespaces
  writeBuffer = KWhiteSpaces + writeBuffer;

  // read
  char readValue[256];
  m_stream >> readValue;

  EXPECT_STREQ(KExpectedValue.c_str(), readValue);
  EXPECT_TRUE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TextStreamStringOperatorsTest, CString_WhiteSpaceAfter)
{
  // data
  const std::string KExpectedValue = "This+is-some_text_no_whitespaces";

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
  m_stream << KExpectedValue.c_str();

  EXPECT_EQ(KExpectedValue, writeBuffer);
  EXPECT_TRUE(m_stream.isGood());

  // prepend read buffer with whitespaces
  writeBuffer += KWhiteSpaces;

  // read
  char readValue[256];
  m_stream >> readValue;

  EXPECT_STREQ(KExpectedValue.c_str(), readValue);
  EXPECT_TRUE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TextStreamStringOperatorsTest, CString_FailedWrite)
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
TEST_F(TextStreamStringOperatorsTest, CString_FailedRead)
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
TEST_F(TextStreamStringOperatorsTest, CString_NoWriteWhenFailed)
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
TEST_F(TextStreamStringOperatorsTest, CString_NoReadWhenFailed)
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
TEST_F(TextStreamStringOperatorsTest, String_Exact)
{
  // data
  const std::string KExpectedValue = "This+is-some_text_no_whitespaces";

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
  m_stream << KExpectedValue.c_str();

  EXPECT_EQ(KExpectedValue, writeBuffer);
  EXPECT_TRUE(m_stream.isGood());

  // read
  String readValue;
  m_stream >> readValue;

  EXPECT_STREQ(KExpectedValue.c_str(), readValue.toAscii());
  EXPECT_TRUE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TextStreamStringOperatorsTest, String_PrependedWhiteSpaces)
{
  // data
  const std::string KExpectedValue = "This+is-some_text_no_whitespaces";

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
  m_stream << KExpectedValue.c_str();

  EXPECT_EQ(KExpectedValue, writeBuffer);
  EXPECT_TRUE(m_stream.isGood());

  // prepend read buffer with whitespaces
  writeBuffer = KWhiteSpaces + writeBuffer;

  // read
  String readValue;
  m_stream >> readValue;

  EXPECT_STREQ(KExpectedValue.c_str(), readValue.toAscii());
  EXPECT_TRUE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TextStreamStringOperatorsTest, String_WhiteSpaceAfter)
{
  // data
  const std::string KExpectedValue = "This+is-some_text_no_whitespaces";

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
  m_stream << KExpectedValue.c_str();

  EXPECT_EQ(KExpectedValue, writeBuffer);
  EXPECT_TRUE(m_stream.isGood());

  // prepend read buffer with whitespaces
  writeBuffer += KWhiteSpaces;

  // read
  String readValue;
  m_stream >> readValue;

  EXPECT_STREQ(KExpectedValue.c_str(), readValue.toAscii());
  EXPECT_TRUE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TextStreamStringOperatorsTest, String_FailedWrite)
{
  // check if initially good
  EXPECT_TRUE(m_stream.isGood());

  // set expectations
  EXPECT_CALL(m_device, write(_, _))
    .WillOnce(Return(-1));

  // make a failed write 
  m_stream << String("Some dummy text");

  // check if noticed
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TextStreamStringOperatorsTest, String_FailedRead)
{
  // check if initially good
  EXPECT_TRUE(m_stream.isGood());

  // set expectations
  EXPECT_CALL(m_device, read(_, _))
    .WillOnce(Return(-1));

  // make a failed read 
  String value;
  m_stream >> value;

  // check if noticed
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TextStreamStringOperatorsTest, String_NoWriteWhenFailed)
{
  makeDataStreamNotGood();

  // set expectations
  EXPECT_CALL(m_device, write(_, _))
    .Times(0);

  // make a write 
  m_stream << String("Some dummy text");

  // check if still bad
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TextStreamStringOperatorsTest, String_NoReadWhenFailed)
{
  makeDataStreamNotGood();

  // set expectations
  EXPECT_CALL(m_device, read(_, _))
    .Times(0);

  // make read
  String value;
  m_stream >> value;

  // check if still bad
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
