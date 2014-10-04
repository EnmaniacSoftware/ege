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
  EFileModeReadOnly = 0,            /*!< Read only. File needs to exist. */
  EFileModeWriteOnly,               /*!< Write only. File will be created if does not exist. Otherwise, content is reset. */
  EFileModeWriteAppend              /*!< Write only. File will be created if does not exist. File pointer moved to an end of content. */
};

/*! Available file seek modes. */
enum FileSeek
{
  EFileSeekBegin = 0,
  EFileSeekCurrent,
  EFileSeekEnd
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(File, PFile)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class File : public Object
{
  public:

    File(const String& filePath);
   ~File();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Returns TRUE if file at given path exists. */
    static bool Exists(const String& filePath);
    /*! Removes file at given path is possible. */
    static bool Remove(const String& filePath);

  public:

    /*! Returns TRUE if object is valid. */
    bool isValid() const;
    /*! Opens the given. 
     *  @param  mode  File open mode.
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
    /*! Returns file size.
     *  @return Size of the file in bytes. In case of an error, negative value is returned. 
     *  @note File needs to be open before call to this method.
     */
    s64 size();

    // TAGE - to be removed. Only statics to be left.
    /*! Returns TRUE if file exists. */
    bool exists() const;
    /*! Removes file if possible. */
    bool remove();

    // TAGE - remove as well ??
    File& operator << (u8 value);
    File& operator << (s8 value);
    File& operator << (u16 value);
    File& operator << (s16 value);
    File& operator << (u32 value);
    File& operator << (s32 value);
    File& operator << (u64 value);
    File& operator << (s64 value);
    File& operator << (bool value);
    File& operator << (float32 value);
    File& operator << (float64 value);
    File& operator >> (u8& value);
    File& operator >> (s8& value);
    File& operator >> (u16& value);
    File& operator >> (s16& value);
    File& operator >> (u32& value);
    File& operator >> (s32& value);
    File& operator >> (u64& value);
    File& operator >> (s64& value);
    File& operator >> (bool& value);
    File& operator >> (float32& value);
    File& operator >> (float64& fValue);

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(File);

    /*! Full file path. */
    String m_filePath;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_FILE_H