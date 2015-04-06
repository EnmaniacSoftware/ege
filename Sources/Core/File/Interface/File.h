#ifndef EGE_CORE_FILE_H
#define EGE_CORE_FILE_H

#include "EGE.h"
#include "EGEString.h"
#include "EGEDataBuffer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Available file open modes. */
enum FileMode
{
  EFileModeReadOnly = 0,            /*!< Read only. TFile needs to exist. */
  EFileModeWriteOnly,               /*!< Write only. TFile will be created if does not exist. Otherwise, content is reset. */
  EFileModeWriteAppend              /*!< Write only. TFile will be created if does not exist. TFile pointer moved to an end of content. */
};

/*! Available file seek modes. */
enum FileSeek
{
  EFileSeekBegin = 0,
  EFileSeekCurrent,
  EFileSeekEnd
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
class TFile : public Object
{
  public:

    TFile(const String& filePath);
   ~TFile();

    EGE_DEFINE_NEW_OPERATORS_INLINE
    EGE_DEFINE_DELETE_OPERATORS_INLINE

  public:

    /*! Opens the given. 
     *  @param  mode  TFile open mode.
     *  @return EGE_SUCCESS on success. Otherwise, EGE_ERROR_IO.
     */
    EGEResult open(FileMode mode);
    /*! Closes file. */
    void close();
    /*! Reads data from file into buffer.
     *  @param  dst   Buffer to write aquired data to.
     *  @param  size  Size of data (in bytes) to read from the file. If negative, all data will be read.
     *  @return Returns number of bytes read. This corresponds to number of bytes written to the buffer.
     */
    s64 read(const PDataBuffer& dst, s64 size = -1);
    /*! Writes given amount of data from destination buffer to a file.
     *  @param  src   Buffer the data is to be read from.
     *  @param  size  Number of bytes to read from the buffer. If negative, all data will be read.
     *  @return Returns number of bytes written to a file.
     */
    s64 write(const PDataBuffer& src, s64 size = -1);
    /*! Sets new position within file. 
     *  @param  offset  Offset to move the file pointer by. This can be negative.
     *  @param  mode    Initial origin from where the offset is to be applied.
     *  @return Old file pointer position. In case of an error, negative value is returned.
     */
    s64 seek(s64 offset, FileSeek mode);
    /*! Returns current position in file. 
     *  @note Negative value is returned in case of an error. 
     */
    s64 tell();
    /*! Returns TRUE if file is opened. */
    bool isOpen() const;
    /*! Returns file path. */
    const String& filePath() const;

    // TAGE - remove as well ??
    TFile& operator << (u8 value);
    TFile& operator << (s8 value);
    TFile& operator << (u16 value);
    TFile& operator << (s16 value);
    TFile& operator << (u32 value);
    TFile& operator << (s32 value);
    TFile& operator << (u64 value);
    TFile& operator << (s64 value);
    TFile& operator << (bool value);
    TFile& operator << (float32 value);
    TFile& operator << (float64 value);
    TFile& operator >> (u8& value);
    TFile& operator >> (s8& value);
    TFile& operator >> (u16& value);
    TFile& operator >> (s16& value);
    TFile& operator >> (u32& value);
    TFile& operator >> (s32& value);
    TFile& operator >> (u64& value);
    TFile& operator >> (s64& value);
    TFile& operator >> (bool& value);
    TFile& operator >> (float32& value);
    TFile& operator >> (float64& fValue);

  private:

    /*! Implementation. */
    T m_impl;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TFile<T>::TFile(const String& filePath)
: Object(EGE_OBJECT_UID_FILE)
, m_impl(filePath) 
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TFile<T>::~TFile()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
EGEResult TFile<T>::open(FileMode mode)
{
  return m_impl.open(mode);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TFile<T>::close()
{
  m_impl.close();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
s64 TFile<T>::read(const PDataBuffer& dst, s64 size)
{
  return m_impl.read(dst, size);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
s64 TFile<T>::write(const PDataBuffer& src, s64 size)
{
  return m_impl.write(src, size);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
s64 TFile<T>::seek(s64 offset, FileSeek mode)
{
  return m_impl.seek(offset, mode);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
s64 TFile<T>::tell()
{
  return m_impl.tell();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TFile<T>::isOpen() const
{
  return m_impl.isOpen();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
const String& TFile<T>::filePath() const
{
  return m_impl.filePath();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TFile<T>& TFile<T>::operator << (u8 value)
{
  DataBuffer buf(&value, sizeof (value));
  m_impl.write(buf, sizeof (value));

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TFile<T>& TFile<T>::operator << (s8 value)
{
  DataBuffer buf(&value, sizeof (value));
  m_impl.write(buf, sizeof (value));

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TFile<T>& TFile<T>::operator << (u16 value)
{
  u8 data[2] = { static_cast<u8>(value & 0x00ff), static_cast<u8>((value & 0xff00) >> 8) };

  DataBuffer buf(&data, sizeof (data));
  m_impl.write(buf, sizeof (value));

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TFile<T>& TFile<T>::operator << (s16 value)
{
  s8 data[2] = { static_cast<s8>(value & 0x00ff), static_cast<s8>((value & 0xff00) >> 8) };

  DataBuffer buf(&data, sizeof (data));
  m_impl.write(buf, sizeof (value));

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TFile<T>& TFile<T>::operator << (u32 value)
{
  u8 data[4] = { (u8)(value & 0x000000ff), (u8)((value & 0x0000ff00) >> 8), (u8)((value & 0x00ff0000) >> 16), (u8)((value & 0xff000000) >> 24) };

  DataBuffer buf(&data, sizeof (data));
  m_impl.write(buf, sizeof (value));

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TFile<T>& TFile<T>::operator << (s32 value)
{
  u8 data[4] = { (u8)(value & 0x000000ff), (u8)((value & 0x0000ff00) >> 8), (u8)((value & 0x00ff0000) >> 16), (u8)((value & 0xff000000) >> 24) };

  DataBuffer buf(&data, sizeof (data));
  m_impl.write(buf, sizeof (value));

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TFile<T>& TFile<T>::operator << (u64 value)
{
  u8 data[8] = { (u8)((value & 0x00000000000000ffLL) >> 00), (u8)((value & 0x000000000000ff00LL) >> 8), 
                  (u8)((value & 0x0000000000ff0000LL) >> 16), (u8)((value & 0x00000000ff000000LL) >> 24),
                  (u8)((value & 0x000000ff00000000LL) >> 32), (u8)((value & 0x0000ff0000000000LL) >> 40), 
                  (u8)((value & 0x00ff000000000000LL) >> 48), (u8)((value & 0xff00000000000000LL) >> 56) };

  DataBuffer buf(&data, sizeof (data));
  m_impl.write(buf, sizeof (value));

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TFile<T>& TFile<T>::operator << (s64 value)
{
  u8 data[8] = { (u8)((value & 0x00000000000000ffLL) >> 0), (u8)((value & 0x000000000000ff00LL) >> 8), 
                  (u8)((value & 0x0000000000ff0000LL) >> 16), (u8)((value & 0x00000000ff000000LL) >> 24),
                  (u8)((value & 0x000000ff00000000LL) >> 32), (u8)((value & 0x0000ff0000000000LL) >> 40), 
                  (u8)((value & 0x00ff000000000000LL) >> 48), (u8)((value & 0xff00000000000000LL) >> 56) };

  DataBuffer buf(&data, sizeof (data));
  m_impl.write(buf, sizeof (value));

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TFile<T>& TFile<T>::operator << (bool value)
{
  return this->operator<< ((u8)(value ? 1 : 0));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TFile<T>& TFile<T>::operator << (float32 value)
{
  DataBuffer buf(&value, sizeof (value));
  m_impl.write(buf, sizeof (value));

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TFile<T>& TFile<T>::operator << (float64 value)
{
  DataBuffer buf(&value, sizeof (value));
  m_impl.write(buf, sizeof (value));

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TFile<T>& TFile<T>::operator >> (u8& value)
{
  DataBuffer buf;
  m_impl.read(buf, sizeof (value));
  buf >> value;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TFile<T>& TFile<T>::operator >> (s8& value)
{
  DataBuffer buf;
  m_impl.read(buf, sizeof (value));
  buf >> value;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TFile<T>& TFile<T>::operator >> (u16& value)
{
  DataBuffer buf;
  m_impl.read(buf, sizeof (value));
  buf >> value;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TFile<T>& TFile<T>::operator >> (s16& value)
{
  DataBuffer buf;
  m_impl.read(buf, sizeof (value));
  buf >> value;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TFile<T>& TFile<T>::operator >> (u32& value)
{
  DataBuffer buf;
  m_impl.read(buf, sizeof (value));
  buf >> value;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TFile<T>& TFile<T>::operator >> (s32& value)
{
  DataBuffer buf;
  m_impl.read(buf, sizeof (value));
  buf >> value;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TFile<T>& TFile<T>::operator >> (u64& value)
{
  DataBuffer buf;
  m_impl.read(buf, sizeof (value));
  buf >> value;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TFile<T>& TFile<T>::operator >> (s64& value)
{
  DataBuffer buf;
  m_impl.read(buf, sizeof (value));
  buf >> value;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TFile<T>& TFile<T>::operator >> (bool& value)
{
  u8 data = 0;
  this->operator>>(data);
  value = (1 == data);

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TFile<T>& TFile<T>::operator >> (float32& value)
{
  DataBuffer buf;
  m_impl.read(buf, sizeof (value));
  buf >> value;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TFile<T>& TFile<T>::operator >> (float64& value)
{
  DataBuffer buf;
  m_impl.read(buf, sizeof (value));
  buf >> value;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_FILE_H