#ifndef EGE_WIN32_FILE_PRIVATE_H
#define EGE_WIN32_FILE_PRIVATE_H

#include "EGE.h"
#include "EGEDataBuffer.h"
#include "Core/File/Interface/File.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class FilePrivate
{
  public:

    FilePrivate(const String& filePath);
   ~FilePrivate();

    /*! @see File::open. */
    EGEResult open(FileMode mode);
    /*! @see File::close. */
    void close();
    /*! @see File::read. */
    s64 read(const PDataBuffer& dst, s64 size);
    /*! @see File::write. */
    s64 write(const PDataBuffer& src, s64 size);
    /*! @see File::seek. */
    s64 seek(s64 offset, FileSeek mode);
    /*! @see File::tell. */
    s64 tell();
    /*! @see File::isOpen. */
    bool isOpen() const;
    /*! @see File::filePath. */
    const String& filePath() const;

  private:

    /*! File handle. */
    FILE* m_file;
    /*! File path. */
    String m_filePath;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_WIN32_FILE_PRIVATE_H