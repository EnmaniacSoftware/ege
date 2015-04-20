#ifndef EGE_CORE_FILE_H
#define EGE_CORE_FILE_H

#include "EGE.h"
#include "EGEDataBuffer.h"
#include "EGEFileUtils.h"
#include "EGEIODevice.h"
#include "EGEMath.h"
#include "EGEString.h"

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
            , public IODevice
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
    /*! Returns TRUE if file is opened. */
    bool isOpen() const;
    /*! Closes file. */
    void close();

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

    /*! Returns file path. */
    const String& filePath() const;

    /*! Reads data from a file into a buffer.
     *  @param  dst   Buffer to write aquired data to.
     *  @param  size  Size of data (in bytes) to read from the file. If negative, all data will be read.
     *  @return Returns number of bytes read. This corresponds to number of bytes written to the buffer.
     *          Returns negative value in case of an error.
     */
    s64 read(const PDataBuffer& dst, s64 size = -1);

    /*! Writes given amount of data from a destination buffer to a file.
     *  @param  src   Buffer the data is to be read from.
     *  @param  size  Number of bytes to read from the buffer. If negative, all data will be read.
     *  @return Returns number of bytes written to a file.
     *          Returns negative value in case of an error.
     */
    s64 write(const PDataBuffer& src, s64 size = -1);
   
    /*! @see IODevice::read. */
    s64 read(void* data, s64 length) override;
    /*! @see IODevice::write. */
    s64 write(const void* data, s64 length) override;

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
  if ( ! isOpen())
  {
    // error!
    return -1;
  }

  // store current write offset in data buffer
  s64 writeOffset = dst->writeOffset();

  // check if entire file should be read
  if (0 > size)
  {
    size = FileUtils::Size(filePath());
  }

  // make sure buffer is big enough
  if (EGE_SUCCESS != dst->setSize(writeOffset + size))
  {
    // error!
    return -1;
  }

  // read data in
  const s64 readCount = m_impl.read(dst->data(writeOffset), size);

  // manually update write offset in buffer
  if (writeOffset != dst->setWriteOffset(writeOffset + readCount))
  {
    // error!
    return -1;
  }

  return readCount;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
s64 TFile<T>::write(const PDataBuffer& src, s64 size)
{
  if ( ! isOpen())
  {
    // error!
    return -1;
  }

  if (0 > size)
  {
    size = src->size();
  }

  EGE_ASSERT(0 <= size);

  // store current read offset from data buffer
  const s64 readOffset = src->readOffset();

  // dont allow to read beyond the size boundary of buffer
  size = Math::Min(size, src->size() - src->readOffset());

  // write bytes
  const s64 bytesWritten = m_impl.write(src->data(readOffset), size);

  // check if error
  if (0 > bytesWritten)
  {
    // error!
    return bytesWritten;
  }

  // manually update read offset in the buffer
  if (readOffset != src->setReadOffset(readOffset + bytesWritten))
  {
    // error!
    return -1;
  }

  return bytesWritten;
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
s64 TFile<T>::write(const void* data, s64 length)
{
  return m_impl.write(data, length);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
s64 TFile<T>::read(void* data, s64 length)
{
  return m_impl.read(data, length);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_FILE_H