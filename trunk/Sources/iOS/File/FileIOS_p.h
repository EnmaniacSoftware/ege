#ifndef EGE_IOS_FILE_PRIVATE_H
#define EGE_IOS_FILE_PRIVATE_H

#include <EGE.h>
#include <EGEFile.h>
#include <EGEDataBuffer.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(File, PFile)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class FilePrivate
{
  public:

    FilePrivate(File* base);
   ~FilePrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

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
     */
    s64 write(const PDataBuffer& src, s64 size);
    /*! Sets new position within file.  Returns old position or -1 if error occured. */
    s64 seek(s64 offset, EGEFile::ESeekMode mode);
    /*! Returns current position in file. Returns -1 if error occured. */
    s64 tell();
    /*! Returns TRUE if file is opened. */
    bool isOpen() const;
    /*! Returns file size. Returns -1 if error occured. */
    s64 size();
    /*! Returns TRUE if file exists. */
    bool exists() const;
    /*! Removes file if possible. */
    bool remove();

  private:

    /*! Pointer to base. */
    File* m_base;
    /*! File handle. */
   // s3eFile* m_file;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_IOS_FILE_PRIVATE_H