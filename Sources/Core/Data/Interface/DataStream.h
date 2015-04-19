#ifndef EGE_CORE_DATA_DATASTREAM_H
#define EGE_CORE_DATA_DATASTREAM_H

/*! This class provides serialization of binary data.
 */

#include "EGE.h"
#include "EGEByteOrder.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IODevice;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class DataStream
{
  public:

    DataStream(IODevice* device);
   ~DataStream();

  operators:

    DataStream& operator << (u8 value);
    DataStream& operator << (s8 value);
    DataStream& operator << (u16 value);
    DataStream& operator << (s16 value);
    DataStream& operator << (u32 value);
    DataStream& operator << (s32 value);
    DataStream& operator << (u64 value);
    DataStream& operator << (s64 value);
    DataStream& operator << (bool value);
    DataStream& operator << (float32 value);
    DataStream& operator << (float64 value);
    DataStream& operator << (const char* value);

    DataStream& operator >> (u8& value);
    DataStream& operator >> (s8& value);
    DataStream& operator >> (u16& value);
    DataStream& operator >> (s16& value);
    DataStream& operator >> (u32& value);
    DataStream& operator >> (s32& value);
    DataStream& operator >> (u64& value);
    DataStream& operator >> (s64& value);
    DataStream& operator >> (bool& value);
    DataStream& operator >> (float32& value);
    DataStream& operator >> (float64& value);
    DataStream& operator >> (char* value);

  public:

    /*! Writes data into the stream.
     *  @param  data    Data source.
     *  @param  length  Number of bytes to write to the stream from the given source.
     *  @return Number of bytes actually written to the stream. Negative value is returned in case of an error.
     */
    s64 write(const void* data, s64 length);

    /*! Reads data from the stream.
     *  @param  data    Buffer where read data is to be stored.
     *  @param  length  Number of bytes to be read from the stream and stored in the buffer.
     *  @return Number of bytes actually stored in the buffer. Negative value is returned in case of an error.
     */
    s64 read(void* data, s64 length);

    /*! Sets byte ordering. */
    void setByteOrdering(Endianness ordering);
    /*! Returns current byte ordering. */
    Endianness byteOrdering() const;

    /*! Returns status. */
    bool isGood() const;

  private:

    /*! Device. */
    IODevice* m_device;
    /*! Current byte ordering. */
    Endianness m_byteOrdering;
    /*! Status. */
    bool m_good;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DATA_DATASTREAM_H
