#include "Core/Tools/ByteOrder.h"
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ByteOrder::Convert(s16& value, const u8* bytesData, Endianness ordering)
{
  EGE_ASSERT(bytesData);

  switch (ordering)
  {
    case BIG_ENDIAN:

      value = (bytesData[0] << 8) | bytesData[1];
      break;

    case LITTLE_ENDIAN:

      value = bytesData[0] | (bytesData[1] << 8);
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ByteOrder::Convert(u16& value, const u8* bytesData, Endianness ordering)
{
  EGE_ASSERT(bytesData);

  switch (ordering)
  {
    case BIG_ENDIAN:

      value = (bytesData[0] << 8) | bytesData[1];
      break;

    case LITTLE_ENDIAN:

      value = bytesData[0] | (bytesData[1] << 8);
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ByteOrder::Convert(s32& value, const u8* bytesData, Endianness ordering)
{
  EGE_ASSERT(bytesData);

  switch (ordering)
  {
    case BIG_ENDIAN:

      value = (bytesData[0] << 24) | (bytesData[1] << 16) | (bytesData[2] << 8) | bytesData[3];
      break;

    case LITTLE_ENDIAN:

      value = bytesData[0] | (bytesData[1] << 8) | (bytesData[2] << 16) | (bytesData[3] << 24);
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ByteOrder::Convert(u32& value, const u8* bytesData, Endianness ordering)
{
  EGE_ASSERT(bytesData);

  switch (ordering)
  {
    case BIG_ENDIAN:

      value = (bytesData[0] << 24) | (bytesData[1] << 16) | (bytesData[2] << 8) | bytesData[3];
      break;

    case LITTLE_ENDIAN:

      value = bytesData[0] | (bytesData[1] << 8) | (bytesData[2] << 16) | (bytesData[3] << 24);
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ByteOrder::Convert(s64& value, const u8* bytesData, Endianness ordering)
{
  EGE_ASSERT(bytesData);

  switch (ordering)
  {
    case BIG_ENDIAN:

      value = ((s64) bytesData[0] << 56) | ((s64) bytesData[1] << 48) | ((s64) bytesData[2] << 40) | ((s64) bytesData[3] << 32) | ((s64) bytesData[4] << 24) | 
              ((s64) bytesData[5] << 16) | ((s64) bytesData[6] << 8) | bytesData[7];
      break;

    case LITTLE_ENDIAN:

      value = bytesData[0] | ((s64) bytesData[1] << 8) | ((s64) bytesData[2] << 16) | ((s64) bytesData[3] << 24) | ((s64) bytesData[4] << 32) | 
              ((s64) bytesData[5] << 40) | ((s64) bytesData[6] << 48) | ((s64) bytesData[7] << 56);
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ByteOrder::Convert(u64& value, const u8* bytesData, Endianness ordering)
{
  EGE_ASSERT(bytesData);

  switch (ordering)
  {
    case BIG_ENDIAN:

      value = ((s64) bytesData[0] << 56) | ((s64) bytesData[1] << 48) | ((s64) bytesData[2] << 40) | ((s64) bytesData[3] << 32) | ((s64) bytesData[4] << 24) | 
              ((s64) bytesData[5] << 16) | ((s64) bytesData[6] << 8) | bytesData[7];
      break;

    case LITTLE_ENDIAN:

      value = bytesData[0] | ((s64) bytesData[1] << 8) | ((s64) bytesData[2] << 16) | ((s64) bytesData[3] << 24) | ((s64) bytesData[4] << 32) | 
              ((s64) bytesData[5] << 40) | ((s64) bytesData[6] << 48) | ((s64) bytesData[7] << 56);
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ByteOrder::Convert(u8* bytesData, s16 value, Endianness ordering)
{
  EGE_ASSERT(bytesData);

  switch (ordering)
  {
    case BIG_ENDIAN:

      *bytesData++ = static_cast<u8>((value & 0xff00) >> 8);
      *bytesData++ = static_cast<u8>((value & 0x00ff));
      break;

    case LITTLE_ENDIAN:

      *bytesData++ = static_cast<u8>((value & 0x00ff));
      *bytesData++ = static_cast<u8>((value & 0xff00) >> 8);
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ByteOrder::Convert(u8* bytesData, u16 value, Endianness ordering)
{
  EGE_ASSERT(bytesData);

  switch (ordering)
  {
    case BIG_ENDIAN:

      *bytesData++ = static_cast<u8>((value & 0xff00) >> 8);
      *bytesData++ = static_cast<u8>((value & 0x00ff));
      break;

    case LITTLE_ENDIAN:

      *bytesData++ = static_cast<u8>((value & 0x00ff));
      *bytesData++ = static_cast<u8>((value & 0xff00) >> 8);
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ByteOrder::Convert(u8* bytesData, s32 value, Endianness ordering)
{
  EGE_ASSERT(bytesData);

  switch (ordering)
  {
    case BIG_ENDIAN:

      *bytesData++ = static_cast<u8>((value & 0xff000000) >> 24);
      *bytesData++ = static_cast<u8>((value & 0x00ff0000) >> 16);
      *bytesData++ = static_cast<u8>((value & 0x0000ff00) >> 8);
      *bytesData++ = static_cast<u8>((value & 0x000000ff));
      break;

    case LITTLE_ENDIAN:

      *bytesData++ = static_cast<u8>((value & 0x000000ff));
      *bytesData++ = static_cast<u8>((value & 0x0000ff00) >> 8);
      *bytesData++ = static_cast<u8>((value & 0x00ff0000) >> 16);
      *bytesData++ = static_cast<u8>((value & 0xff000000) >> 24);
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ByteOrder::Convert(u8* bytesData, u32 value, Endianness ordering)
{
  EGE_ASSERT(bytesData);

  switch (ordering)
  {
    case BIG_ENDIAN:

      *bytesData++ = static_cast<u8>((value & 0xff000000) >> 24);
      *bytesData++ = static_cast<u8>((value & 0x00ff0000) >> 16);
      *bytesData++ = static_cast<u8>((value & 0x0000ff00) >> 8);
      *bytesData++ = static_cast<u8>((value & 0x000000ff));
      break;

    case LITTLE_ENDIAN:

      *bytesData++ = static_cast<u8>((value & 0x000000ff));
      *bytesData++ = static_cast<u8>((value & 0x0000ff00) >> 8);
      *bytesData++ = static_cast<u8>((value & 0x00ff0000) >> 16);
      *bytesData++ = static_cast<u8>((value & 0xff000000) >> 24);
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ByteOrder::Convert(u8* bytesData, s64 value, Endianness ordering)
{
  EGE_ASSERT(bytesData);

  switch (ordering)
  {
    case BIG_ENDIAN:

      *bytesData++ = static_cast<u8>((value & 0xff00000000000000LL) >> 56);
      *bytesData++ = static_cast<u8>((value & 0x00ff000000000000LL) >> 48);
      *bytesData++ = static_cast<u8>((value & 0x0000ff0000000000LL) >> 40);
      *bytesData++ = static_cast<u8>((value & 0x000000ff00000000LL) >> 32);
      *bytesData++ = static_cast<u8>((value & 0x00000000ff000000LL) >> 24);
      *bytesData++ = static_cast<u8>((value & 0x0000000000ff0000LL) >> 16);
      *bytesData++ = static_cast<u8>((value & 0x000000000000ff00LL) >> 8);
      *bytesData++ = static_cast<u8>((value & 0x00000000000000ffLL));
      break;

    case LITTLE_ENDIAN:

      *bytesData++ = static_cast<u8>((value & 0x00000000000000ffLL));
      *bytesData++ = static_cast<u8>((value & 0x000000000000ff00LL) >> 8);
      *bytesData++ = static_cast<u8>((value & 0x0000000000ff0000LL) >> 16);
      *bytesData++ = static_cast<u8>((value & 0x00000000ff000000LL) >> 24);
      *bytesData++ = static_cast<u8>((value & 0x000000ff00000000LL) >> 32);
      *bytesData++ = static_cast<u8>((value & 0x0000ff0000000000LL) >> 40);
      *bytesData++ = static_cast<u8>((value & 0x00ff000000000000LL) >> 48);
      *bytesData++ = static_cast<u8>((value & 0xff00000000000000LL) >> 56);
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ByteOrder::Convert(u8* bytesData, u64 value, Endianness ordering)
{
  EGE_ASSERT(bytesData);

  switch (ordering)
  {
    case BIG_ENDIAN:

      *bytesData++ = static_cast<u8>((value & 0xff00000000000000LL) >> 56);
      *bytesData++ = static_cast<u8>((value & 0x00ff000000000000LL) >> 48);
      *bytesData++ = static_cast<u8>((value & 0x0000ff0000000000LL) >> 40);
      *bytesData++ = static_cast<u8>((value & 0x000000ff00000000LL) >> 32);
      *bytesData++ = static_cast<u8>((value & 0x00000000ff000000LL) >> 24);
      *bytesData++ = static_cast<u8>((value & 0x0000000000ff0000LL) >> 16);
      *bytesData++ = static_cast<u8>((value & 0x000000000000ff00LL) >> 8);
      *bytesData++ = static_cast<u8>((value & 0x00000000000000ffLL));
      break;

    case LITTLE_ENDIAN:

      *bytesData++ = static_cast<u8>((value & 0x00000000000000ffLL));
      *bytesData++ = static_cast<u8>((value & 0x000000000000ff00LL) >> 8);
      *bytesData++ = static_cast<u8>((value & 0x0000000000ff0000LL) >> 16);
      *bytesData++ = static_cast<u8>((value & 0x00000000ff000000LL) >> 24);
      *bytesData++ = static_cast<u8>((value & 0x000000ff00000000LL) >> 32);
      *bytesData++ = static_cast<u8>((value & 0x0000ff0000000000LL) >> 40);
      *bytesData++ = static_cast<u8>((value & 0x00ff000000000000LL) >> 48);
      *bytesData++ = static_cast<u8>((value & 0xff00000000000000LL) >> 56);
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END