#ifndef EGE_CORE_DATA_SERIALIZABLE_H
#define EGE_CORE_DATA_SERIALIZABLE_H

/** Interface for serializable objects. 
  */

#include "EGE.h"
#include "EGEString.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ISerializable
{
  public:

    virtual ~ISerializable() {}
  
    virtual ISerializable& operator << (u8 value) = 0;
    virtual ISerializable& operator << (s8 value) = 0;
    virtual ISerializable& operator << (u16 value) = 0;
    virtual ISerializable& operator << (s16 value) = 0;
    virtual ISerializable& operator << (u32 value) = 0;
    virtual ISerializable& operator << (s32 value) = 0;
    virtual ISerializable& operator << (u64 value) = 0;
    virtual ISerializable& operator << (s64 value) = 0;
    virtual ISerializable& operator << (bool value) = 0;
    virtual ISerializable& operator << (float32 value) = 0;
    virtual ISerializable& operator << (float64 value) = 0;
    virtual ISerializable& operator << (const char* value) = 0;
    virtual ISerializable& operator << (const String& value) = 0;

    virtual ISerializable& operator >> (u8& value) = 0;
    virtual ISerializable& operator >> (s8& value) = 0;
    virtual ISerializable& operator >> (u16& value) = 0;
    virtual ISerializable& operator >> (s16& value) = 0;
    virtual ISerializable& operator >> (u32& value) = 0;
    virtual ISerializable& operator >> (s32& value) = 0;
    virtual ISerializable& operator >> (u64& value) = 0;
    virtual ISerializable& operator >> (s64& value) = 0;
    virtual ISerializable& operator >> (bool& value) = 0;
    virtual ISerializable& operator >> (float32& value) = 0;
    virtual ISerializable& operator >> (float64& value) = 0;
    virtual ISerializable& operator >> (const char* value) = 0;
    virtual ISerializable& operator >> (String& value) = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DATA_SERIALIZABLE_H