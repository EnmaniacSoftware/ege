#ifndef EGE_CORE_STRING_TESTS_UNITTESTS_TEXTSTREAMTESTBASE_H
#define EGE_CORE_STRING_TESTS_UNITTESTS_TEXTSTREAMTESTBASE_H

#include "TestFramework/Interface/TestBase.h"
#include "TestFramework/Interface/IODeviceMock.h"
#include <EGETextStream.h>

#include <gmock/gmock.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class TextStreamTestBase : public TestBase
{
  protected:

    TextStreamTestBase();
    virtual ~TextStreamTestBase();

    virtual void SetUp();
    virtual void TearDown();

    /*! Forces data stream state to non-good. */
    void makeDataStreamNotGood();

  protected:

    IODeviceMock m_device;
    TextStream m_stream;

  protected:

    const static std::string KWhiteSpaces;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Action which appends data from method call into the provided string.
 *  @param  arg0  Source buffer.
 *  @param  arg1  Size of the source buffer.
 *  @param  value String to append data into.
 */
ACTION_P(AppendFromBufferAtArg0WithLengthAtArg1, value) 
{ 
  std::string* string = value;

  *string += std::string(reinterpret_cast<const char*>(arg0), static_cast<unsigned int>(arg1));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Action which copies data from the begining of a string into method's buffer.
 *  @param  arg0  Destination buffer.
 *  @param  arg1  Size of the source and destination buffers.
 *  @param  value Source string data is copied from. Copied data is removed from the string.
 */
ACTION_P(CopyToBufferAtArg0WithLengthAtArg1AndRemove, value) 
{ 
  std::string* string = value;

  // copy into the buffer
  memcpy(arg0, string->c_str(), static_cast<size_t>(arg1));

  // remove the copied part from the value
  *string = string->substr(static_cast<unsigned int>(arg1));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_STRING_TESTS_UNITTESTS_TEXTSTREAMTESTBASE_H