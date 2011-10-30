#include "Core/Tools/ByteOrder.h"
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts bytes into value with specified byte ordering. */
void ByteOrder::Convert(s16& value, const u8* bytesData, EGEByteOrder::Ordering ordering)
{
  EGE_ASSERT(bytesData);

  switch (ordering)
  {
    case EGEByteOrder::BIG_ENDIAN:

      value = (bytesData[0] << 8) | bytesData[1];
      break;

    case EGEByteOrder::LITTLE_ENDIAN:

      value = bytesData[0] | (bytesData[1] << 8);
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts bytes into value with specified byte ordering. */
void ByteOrder::Convert(u16& value, const u8* bytesData, EGEByteOrder::Ordering ordering)
{
  EGE_ASSERT(bytesData);

  switch (ordering)
  {
    case EGEByteOrder::BIG_ENDIAN:

      value = (bytesData[0] << 8) | bytesData[1];
      break;

    case EGEByteOrder::LITTLE_ENDIAN:

      value = bytesData[0] | (bytesData[1] << 8);
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts bytes into value with specified byte ordering. */
void ByteOrder::Convert(s32& value, const u8* bytesData, EGEByteOrder::Ordering ordering)
{
  EGE_ASSERT(bytesData);

  switch (ordering)
  {
    case EGEByteOrder::BIG_ENDIAN:

      value = (bytesData[0] << 24) | (bytesData[1] << 16) | (bytesData[2] << 8) | bytesData[3];
      break;

    case EGEByteOrder::LITTLE_ENDIAN:

      value = bytesData[0] | (bytesData[1] << 8) | (bytesData[2] << 16) | (bytesData[3] << 24);
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts bytes into value with specified byte ordering. */
void ByteOrder::Convert(u32& value, const u8* bytesData, EGEByteOrder::Ordering ordering)
{
  EGE_ASSERT(bytesData);

  switch (ordering)
  {
    case EGEByteOrder::BIG_ENDIAN:

      value = (bytesData[0] << 24) | (bytesData[1] << 16) | (bytesData[2] << 8) | bytesData[3];
      break;

    case EGEByteOrder::LITTLE_ENDIAN:

      value = bytesData[0] | (bytesData[1] << 8) | (bytesData[2] << 16) | (bytesData[3] << 24);
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts bytes into value with specified byte ordering. */
void ByteOrder::Convert(s64& value, const u8* bytesData, EGEByteOrder::Ordering ordering)
{
  EGE_ASSERT(bytesData);

  switch (ordering)
  {
    case EGEByteOrder::BIG_ENDIAN:

      value = ((s64) bytesData[0] << 56) | ((s64) bytesData[1] << 48) | ((s64) bytesData[2] << 40) | ((s64) bytesData[3] << 32) | ((s64) bytesData[4] << 24) | 
              ((s64) bytesData[5] << 16) | ((s64) bytesData[6] << 8) | bytesData[7];
      break;

    case EGEByteOrder::LITTLE_ENDIAN:

      value = bytesData[0] | ((s64) bytesData[1] << 8) | ((s64) bytesData[2] << 16) | ((s64) bytesData[3] << 24) | ((s64) bytesData[4] << 32) | 
              ((s64) bytesData[5] << 40) | ((s64) bytesData[6] << 48) | ((s64) bytesData[7] << 56);
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts bytes into value with specified byte ordering. */
void ByteOrder::Convert(u64& value, const u8* bytesData, EGEByteOrder::Ordering ordering)
{
  EGE_ASSERT(bytesData);

  switch (ordering)
  {
    case EGEByteOrder::BIG_ENDIAN:

      value = ((s64) bytesData[0] << 56) | ((s64) bytesData[1] << 48) | ((s64) bytesData[2] << 40) | ((s64) bytesData[3] << 32) | ((s64) bytesData[4] << 24) | 
              ((s64) bytesData[5] << 16) | ((s64) bytesData[6] << 8) | bytesData[7];
      break;

    case EGEByteOrder::LITTLE_ENDIAN:

      value = bytesData[0] | ((s64) bytesData[1] << 8) | ((s64) bytesData[2] << 16) | ((s64) bytesData[3] << 24) | ((s64) bytesData[4] << 32) | 
              ((s64) bytesData[5] << 40) | ((s64) bytesData[6] << 48) | ((s64) bytesData[7] << 56);
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts value into stream of bytes with specified byte ordering. */
void ByteOrder::Convert(u8* bytesData, s16 value, EGEByteOrder::Ordering ordering)
{
  EGE_ASSERT(bytesData);

  switch (ordering)
  {
    case EGEByteOrder::BIG_ENDIAN:

      *bytesData++ = static_cast<u8>((value & 0xff00) >> 8);
      *bytesData++ = static_cast<u8>((value & 0x00ff));
      break;

    case EGEByteOrder::LITTLE_ENDIAN:

      *bytesData++ = static_cast<u8>((value & 0x00ff));
      *bytesData++ = static_cast<u8>((value & 0xff00) >> 8);
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts value into stream of bytes with specified byte ordering. */
void ByteOrder::Convert(u8* bytesData, u16 value, EGEByteOrder::Ordering ordering)
{
  EGE_ASSERT(bytesData);

  switch (ordering)
  {
    case EGEByteOrder::BIG_ENDIAN:

      *bytesData++ = static_cast<u8>((value & 0xff00) >> 8);
      *bytesData++ = static_cast<u8>((value & 0x00ff));
      break;

    case EGEByteOrder::LITTLE_ENDIAN:

      *bytesData++ = static_cast<u8>((value & 0x00ff));
      *bytesData++ = static_cast<u8>((value & 0xff00) >> 8);
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts value into stream of bytes with specified byte ordering. */
void ByteOrder::Convert(u8* bytesData, s32 value, EGEByteOrder::Ordering ordering)
{
  EGE_ASSERT(bytesData);

  switch (ordering)
  {
    case EGEByteOrder::BIG_ENDIAN:

      *bytesData++ = static_cast<u8>((value & 0xff000000) >> 24);
      *bytesData++ = static_cast<u8>((value & 0x00ff0000) >> 16);
      *bytesData++ = static_cast<u8>((value & 0x0000ff00) >> 8);
      *bytesData++ = static_cast<u8>((value & 0x000000ff));
      break;

    case EGEByteOrder::LITTLE_ENDIAN:

      *bytesData++ = static_cast<u8>((value & 0x000000ff));
      *bytesData++ = static_cast<u8>((value & 0x0000ff00) >> 8);
      *bytesData++ = static_cast<u8>((value & 0x00ff0000) >> 16);
      *bytesData++ = static_cast<u8>((value & 0xff000000) >> 24);
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts value into stream of bytes with specified byte ordering. */
void ByteOrder::Convert(u8* bytesData, u32 value, EGEByteOrder::Ordering ordering)
{
  EGE_ASSERT(bytesData);

  switch (ordering)
  {
    case EGEByteOrder::BIG_ENDIAN:

      *bytesData++ = static_cast<u8>((value & 0xff000000) >> 24);
      *bytesData++ = static_cast<u8>((value & 0x00ff0000) >> 16);
      *bytesData++ = static_cast<u8>((value & 0x0000ff00) >> 8);
      *bytesData++ = static_cast<u8>((value & 0x000000ff));
      break;

    case EGEByteOrder::LITTLE_ENDIAN:

      *bytesData++ = static_cast<u8>((value & 0x000000ff));
      *bytesData++ = static_cast<u8>((value & 0x0000ff00) >> 8);
      *bytesData++ = static_cast<u8>((value & 0x00ff0000) >> 16);
      *bytesData++ = static_cast<u8>((value & 0xff000000) >> 24);
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts value into stream of bytes with specified byte ordering. */
void ByteOrder::Convert(u8* bytesData, s64 value, EGEByteOrder::Ordering ordering)
{
  EGE_ASSERT(bytesData);

  switch (ordering)
  {
    case EGEByteOrder::BIG_ENDIAN:

      *bytesData++ = static_cast<u8>((value & 0xff00000000000000) >> 56);
      *bytesData++ = static_cast<u8>((value & 0x00ff000000000000) >> 48);
      *bytesData++ = static_cast<u8>((value & 0x0000ff0000000000) >> 40);
      *bytesData++ = static_cast<u8>((value & 0x000000ff00000000) >> 32);
      *bytesData++ = static_cast<u8>((value & 0x00000000ff000000) >> 24);
      *bytesData++ = static_cast<u8>((value & 0x0000000000ff0000) >> 16);
      *bytesData++ = static_cast<u8>((value & 0x000000000000ff00) >> 8);
      *bytesData++ = static_cast<u8>((value & 0x00000000000000ff));
      break;

    case EGEByteOrder::LITTLE_ENDIAN:

      *bytesData++ = static_cast<u8>((value & 0x00000000000000ff));
      *bytesData++ = static_cast<u8>((value & 0x000000000000ff00) >> 8);
      *bytesData++ = static_cast<u8>((value & 0x0000000000ff0000) >> 16);
      *bytesData++ = static_cast<u8>((value & 0x00000000ff000000) >> 24);
      *bytesData++ = static_cast<u8>((value & 0x000000ff00000000) >> 32);
      *bytesData++ = static_cast<u8>((value & 0x0000ff0000000000) >> 40);
      *bytesData++ = static_cast<u8>((value & 0x00ff000000000000) >> 48);
      *bytesData++ = static_cast<u8>((value & 0xff00000000000000) >> 56);
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts value into stream of bytes with specified byte ordering. */
void ByteOrder::Convert(u8* bytesData, u64 value, EGEByteOrder::Ordering ordering)
{
  EGE_ASSERT(bytesData);

  switch (ordering)
  {
    case EGEByteOrder::BIG_ENDIAN:

      *bytesData++ = static_cast<u8>((value & 0xff00000000000000) >> 56);
      *bytesData++ = static_cast<u8>((value & 0x00ff000000000000) >> 48);
      *bytesData++ = static_cast<u8>((value & 0x0000ff0000000000) >> 40);
      *bytesData++ = static_cast<u8>((value & 0x000000ff00000000) >> 32);
      *bytesData++ = static_cast<u8>((value & 0x00000000ff000000) >> 24);
      *bytesData++ = static_cast<u8>((value & 0x0000000000ff0000) >> 16);
      *bytesData++ = static_cast<u8>((value & 0x000000000000ff00) >> 8);
      *bytesData++ = static_cast<u8>((value & 0x00000000000000ff));
      break;

    case EGEByteOrder::LITTLE_ENDIAN:

      *bytesData++ = static_cast<u8>((value & 0x00000000000000ff));
      *bytesData++ = static_cast<u8>((value & 0x000000000000ff00) >> 8);
      *bytesData++ = static_cast<u8>((value & 0x0000000000ff0000) >> 16);
      *bytesData++ = static_cast<u8>((value & 0x00000000ff000000) >> 24);
      *bytesData++ = static_cast<u8>((value & 0x000000ff00000000) >> 32);
      *bytesData++ = static_cast<u8>((value & 0x0000ff0000000000) >> 40);
      *bytesData++ = static_cast<u8>((value & 0x00ff000000000000) >> 48);
      *bytesData++ = static_cast<u8>((value & 0xff00000000000000) >> 56);
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
