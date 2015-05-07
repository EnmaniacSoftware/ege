#ifndef EGE_CORE_DATA_TESTS_UNITTESTS_DATASTREAMTESTBASE_H
#define EGE_CORE_DATA_TESTS_UNITTESTS_DATASTREAMTESTBASE_H

#include "TestFramework/Interface/TestBase.h"
#include "TestFramework/Interface/IODeviceMock.h"
#include <EGEDataStream.h>

#include <gmock/gmock.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class DataStreamTestBase : public TestBase
{
  protected:

    DataStreamTestBase();
    virtual ~DataStreamTestBase();

    virtual void SetUp();
    virtual void TearDown();

    /*! Forces data stream state to non-good. */
    void makeDataStreamNotGood();

    /*! Returns TRUE if host is big endian. */
    bool isBigEndian() const;

    /*! Reverses byte order of a given value. */
    u8  reverseByteOrder(u8 value, bool matchHost = true) const;
    s8  reverseByteOrder(s8 value, bool matchHost = true) const;
    u16 reverseByteOrder(u16 value, bool matchHost = true) const;
    s16 reverseByteOrder(s16 value, bool matchHost = true) const;
    u32 reverseByteOrder(u32 value, bool matchHost = true) const;
    s32 reverseByteOrder(s32 value, bool matchHost = true) const;
    u64 reverseByteOrder(u64 value, bool matchHost = true) const;
    s64 reverseByteOrder(s64 value, bool matchHost = true) const;

  protected:

    IODeviceMock m_device;
    DataStream m_stream;

  private:

    /*! Returns TRUE if byte ordering of the stream data requires rearrangments due to host byte ordering. */
    bool isReverseNeeded(bool matchHost) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Action which copies data from method call into the provided (pre-allocated) buffer.
 *  @param  arg0  Source buffer.
 *  @param  arg1  Size of the source buffer.
 *  @param  value Output buffer.
 */
ACTION_P(CopyFromBufferAtArg0WithLengthAtArg1, value) 
{ 
  memcpy(value, arg0, static_cast<size_t>(arg1));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Action which copies data from a buffer into method's buffer.
 *  @param  arg0  Destination buffer.
 *  @param  arg1  Size of the source and destination buffers.
 *  @param  value Source buffer.
 */
ACTION_P(CopyToBufferAtArg0WithLengthAtArg1, value) 
{ 
  memcpy(arg0, value, static_cast<size_t>(arg1));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DATA_TESTS_UNITTESTS_DATASTREAMTESTBASE_H