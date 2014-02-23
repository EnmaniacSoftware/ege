#ifndef EGE_CORE_FILE_H
#define EGE_CORE_FILE_H

#include "EGE.h"
#include "EGEFile.h"
#include "EGEString.h"
#include "EGEDataBuffer.h"

EGE_NAMESPACE_BEGIN

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
    /*! Opens the given file with requested mode. */
    EGEResult open(EGEFile::EMode mode);
    /*! Closes file. */
    void close();
    /*! Reads given amount of data into destination buffer.
     *  @return Returns number of bytes read.
     */
    s64 read(const PDataBuffer& dst, s64 size);
    /*! Writes given amount of data from destination buffer.
     *  @return Returns number of bytes written.
     *  @note If size is negative all data from source buffer is written.
     */
    s64 write(const PDataBuffer& src, s64 size = -1);
    /*! Sets new position within file. Returns old position or -1 if error occured. */
    s64 seek(s64 offset, EGEFile::ESeekMode mode);
    /*! Returns current position in file. Returns -1 if error occured. */
    s64 tell();
    /*! Returns TRUE if file is opened. */
    bool isOpen() const;
    /*! Returns file path. */
    const String& filePath() const;
    /*! Returns file size. Returns -1 if error occured. */
    s64 size();
    /*! Returns TRUE if file exists. */
    bool exists() const;
    /*! Removes file if possible. */
    bool remove();

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