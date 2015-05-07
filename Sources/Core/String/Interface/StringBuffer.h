#ifndef EGE_CORE_STRING_STRINGBUFFER_H
#define EGE_CORE_STRING_STRINGBUFFER_H

/*! Convinient buffer for text data storage. Allows writing and reading and acts as an I/O device for streaming purposes.
 */

#include "EGE.h"
#include "EGEString.h"
#include "EGEIODevice.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_PREDECLARE_SMART_CLASS(StringBuffer, PStringBuffer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class StringBuffer : public Object
                   , public IODevice
{
  public:
  
    StringBuffer();
    virtual ~StringBuffer();
  
    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS
  
  public:

    /*! Retuens string object. */
    const String& string() const;
    
    /*! Returns buffer size in bytes. */
    s32 size() const;

    /*! @see IODevice::read. */
    s64 read(void* data, s64 length) override;
    /*! @see IODevice::write. */
    s64 write(const void* data, s64 length) override;

    /*! Clears buffer. */
    void clear();

  private:
  
    /*! Underlying buffer. */
    String m_buffer;
    /*! Read pointer offset (0-based). */
    s32 m_readOffset;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_STRING_STRINGBUFFER_H