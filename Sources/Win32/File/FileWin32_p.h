#ifndef EGE_WIN32_FILE_PRIVATE_H
#define EGE_WIN32_FILE_PRIVATE_H

#include <EGE.h>
#include "Core/File/File.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class DataBuffer;
EGE_DECLARE_SMART_CLASS(File, PFile)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class FilePrivate
{
  public:

    FilePrivate(File* base);
   ~FilePrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(File)

    /*! @see File::isValid. */
    bool isValid() const;
    /*! @see File::open. */
    EGEResult open(EGEFile::EMode mode);
    /*! @see File::close. */
    void close();
    /*! @see File::read. */
    s64 read(const PDataBuffer& dst, s64 size);
    /*! @see File::write. */
    s64 write(const PDataBuffer& src, s64 size);
    /*! @see File::seek. */
    s64 seek(s64 offset, EGEFile::ESeekMode mode);
    /*! @see File::tell. */
    s64 tell();
    /*! @see File::isOpen. */
    bool isOpen() const;
    /*! @see File::size. */
    s64 size();
    /*! @see File::exists. */
    bool exists() const;
    /*! @see File::remove. */
    bool remove();

  private:

    /*! File handle. */
    FILE* m_file;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_WIN32_FILE_PRIVATE_H