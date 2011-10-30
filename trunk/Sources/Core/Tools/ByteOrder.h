#ifndef EGE_CORE_BYTEORDER_H
#define EGE_CORE_BYTEORDER_H

#include <EGE.h>
#include <EGEByteOrder.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ByteOrder
{
  public:
    
    /* Converts bytes into value with specified byte ordering. */
    static void Convert(s16& value, const u8* bytesData, EGEByteOrder::Ordering ordering);
    /* Converts bytes into value with specified byte ordering. */
    static void Convert(u16& value, const u8* bytesData, EGEByteOrder::Ordering ordering);
    /* Converts bytes into value with specified byte ordering. */
    static void Convert(s32& value, const u8* bytesData, EGEByteOrder::Ordering ordering);
    /* Converts bytes into value with specified byte ordering. */
    static void Convert(u32& value, const u8* bytesData, EGEByteOrder::Ordering ordering);
    /* Converts bytes into value with specified byte ordering. */
    static void Convert(s64& value, const u8* bytesData, EGEByteOrder::Ordering ordering);
    /* Converts bytes into value with specified byte ordering. */
    static void Convert(u64& value, const u8* bytesData, EGEByteOrder::Ordering ordering);

    /* Converts value into stream of bytes with specified byte ordering. */
    static void Convert(u8* bytesData, s16 value, EGEByteOrder::Ordering ordering);
    /* Converts value into stream of bytes with specified byte ordering. */
    static void Convert(u8* bytesData, u16 value, EGEByteOrder::Ordering ordering);
    /* Converts value into stream of bytes with specified byte ordering. */
    static void Convert(u8* bytesData, s32 value, EGEByteOrder::Ordering ordering);
    /* Converts value into stream of bytes with specified byte ordering. */
    static void Convert(u8* bytesData, u32 value, EGEByteOrder::Ordering ordering);
    /* Converts value into stream of bytes with specified byte ordering. */
    static void Convert(u8* bytesData, s64 value, EGEByteOrder::Ordering ordering);
    /* Converts value into stream of bytes with specified byte ordering. */
    static void Convert(u8* bytesData, u64 value, EGEByteOrder::Ordering ordering);
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_BYTEORDER_H