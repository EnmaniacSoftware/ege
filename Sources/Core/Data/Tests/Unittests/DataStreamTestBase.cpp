#include "DataStreamTestBase.h"

using namespace ::testing;

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataStreamTestBase::DataStreamTestBase()
: TestBase()
, m_stream(&m_device)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataStreamTestBase::~DataStreamTestBase()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DataStreamTestBase::SetUp()
{
 // m_stream.setByteOrdering(GetParam());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DataStreamTestBase::TearDown()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool DataStreamTestBase::isBigEndian() const
{
  const int KOne = 1;
  
  return (*(char*) &KOne) == 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool DataStreamTestBase::isReverseNeeded(bool matchHost) const
{
  // check if host endianess should be matched
  if (matchHost)
  {
    // check if different
    if ((isBigEndian() && (EBigEndian != m_stream.byteOrdering())) ||
        ( ! isBigEndian() && (ELittleEndian != m_stream.byteOrdering())))
    {
      // required
      return true;
    }
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u8 DataStreamTestBase::reverseByteOrder(u8 value, bool /*matchHost*/) const
{
  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s8 DataStreamTestBase::reverseByteOrder(s8 value, bool /*matchHost*/) const
{
  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u16 DataStreamTestBase::reverseByteOrder(u16 value, bool matchHost) const
{
  u8 b1 = value & 0xff;
  u8 b2 = (value >> 8) & 0xff;

  return isReverseNeeded(matchHost) ? (static_cast<u16>(b2) | (static_cast<u16>(b1) << 8)) : value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s16 DataStreamTestBase::reverseByteOrder(s16 value, bool matchHost) const
{
  u8 b1 = value & 0xff;
  u8 b2 = (value >> 8) & 0xff;

  return isReverseNeeded(matchHost) ? (static_cast<s16>(b2) | (static_cast<s16>(b1) << 8)) : value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 DataStreamTestBase::reverseByteOrder(u32 value, bool matchHost) const
{
  u8 b1 = value & 0xff;
  u8 b2 = (value >> 8) & 0xff;
  u8 b3 = (value >> 16) & 0xff;
  u8 b4 = (value >> 24) & 0xff;

  return isReverseNeeded(matchHost) ? (static_cast<u32>(b4) | (static_cast<u32>(b3) << 8) | (static_cast<u32>(b2) << 16) | (static_cast<u32>(b1) << 24))
                                    : value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 DataStreamTestBase::reverseByteOrder(s32 value, bool matchHost) const
{
  u8 b1 = value & 0xff;
  u8 b2 = (value >> 8) & 0xff;
  u8 b3 = (value >> 16) & 0xff;
  u8 b4 = (value >> 24) & 0xff;

  return isReverseNeeded(matchHost) ? (static_cast<s32>(b4) | (static_cast<s32>(b3) << 8) | (static_cast<s32>(b2) << 16) | (static_cast<s32>(b1) << 24))
                                    : value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u64 DataStreamTestBase::reverseByteOrder(u64 value, bool matchHost) const
{
  u8 b1 = value & 0xff;
  u8 b2 = (value >> 8) & 0xff;
  u8 b3 = (value >> 16) & 0xff;
  u8 b4 = (value >> 24) & 0xff;
  u8 b5 = (value >> 32) & 0xff;
  u8 b6 = (value >> 40) & 0xff;
  u8 b7 = (value >> 48) & 0xff;
  u8 b8 = (value >> 56) & 0xff;

  return isReverseNeeded(matchHost) ? (static_cast<u64>(b8) | (static_cast<u64>(b7) << 8) | (static_cast<u64>(b6) << 16) | (static_cast<u64>(b5) << 24) |
                                      (static_cast<u64>(b4) << 32) | (static_cast<u64>(b3) << 40) | (static_cast<u64>(b2) << 48) | (static_cast<u64>(b1) << 56))
                                    : value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 DataStreamTestBase::reverseByteOrder(s64 value, bool matchHost) const
{
  u8 b1 = value & 0xff;
  u8 b2 = (value >> 8) & 0xff;
  u8 b3 = (value >> 16) & 0xff;
  u8 b4 = (value >> 24) & 0xff;
  u8 b5 = (value >> 32) & 0xff;
  u8 b6 = (value >> 40) & 0xff;
  u8 b7 = (value >> 48) & 0xff;
  u8 b8 = (value >> 56) & 0xff;

  return isReverseNeeded(matchHost) ? (static_cast<s64>(b8) | (static_cast<s64>(b7) << 8) | (static_cast<s64>(b6) << 16) | (static_cast<s64>(b5) << 24) |
                                      (static_cast<s64>(b4) << 32) | (static_cast<s64>(b3) << 40) | (static_cast<s64>(b2) << 48) | (static_cast<s64>(b1) << 56))
                                    : value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DataStreamTestBase::makeDataStreamNotGood()
{
  // NOTE: its a bit workaround as there is currently no way to access private data members to do it directly

  // set expectations
  EXPECT_CALL(m_device, write(_, _))
    .WillOnce(Return(-1));

  // make a failed write 
  m_stream.write(NULL, 1);

  // check if bad
  EXPECT_FALSE(m_stream.isGood());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

