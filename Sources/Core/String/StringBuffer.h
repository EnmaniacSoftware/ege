#ifndef EGE_CORE_STRING_STRINGBUFFER_H
#define EGE_CORE_STRING_STRINGBUFFER_H

#include "EGE.h"
#include "EGEString.h"
#include "Core/Data/Interface/Serializable.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(StringBuffer, PStringBuffer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class StringBuffer : public Object, public ISerializable
{
  public:
  
    StringBuffer();
    virtual ~StringBuffer();
  
    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS
  
    /*! Retuens string object. */
    const String& string() const;
  
    ISerializable& operator << (u8 value) override;
    ISerializable& operator << (s8 value) override;
    ISerializable& operator << (u16 value) override;
    ISerializable& operator << (s16 value) override;
    ISerializable& operator << (u32 value) override;
    ISerializable& operator << (s32 value) override;
    ISerializable& operator << (u64 value) override;
    ISerializable& operator << (s64 value) override;
    ISerializable& operator << (bool value) override;
    ISerializable& operator << (float32 value) override;
    ISerializable& operator << (float64 value) override;
    ISerializable& operator << (const char* value) override;
    ISerializable& operator << (const String& value) override;
  
    ISerializable& operator >> (u8& value) override;
    ISerializable& operator >> (s8& value) override;
    ISerializable& operator >> (u16& value) override;
    ISerializable& operator >> (s16& value) override;
    ISerializable& operator >> (u32& value) override;
    ISerializable& operator >> (s32& value) override;
    ISerializable& operator >> (u64& value) override;
    ISerializable& operator >> (s64& value) override;
    ISerializable& operator >> (bool& value) override;
    ISerializable& operator >> (float32& value) override;
    ISerializable& operator >> (float64& value) override;
    ISerializable& operator >> (const char* value) override;
    ISerializable& operator >> (String& value) override;
  
  private:
  
    /*! Underlying buffer. */
    String m_buffer;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_STRING_STRINGBUFFER_H