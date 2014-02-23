#include "Airplay/File/FileAirplay_p.h"
#include "Core/Math/Math.h"
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(FilePrivate)
EGE_DEFINE_DELETE_OPERATORS(FilePrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
FilePrivate::FilePrivate(File* base) : m_d(base), 
                                       m_file(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
FilePrivate::~FilePrivate()
{
  close();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FilePrivate::isValid() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult FilePrivate::open(EGEFile::EMode mode)
{
  close();

  // map mode
  String modeInternal;
  switch (mode)
  {
    case EGEFile::MODE_READ_ONLY:  modeInternal = "rb"; break;
    case EGEFile::MODE_WRITE_ONLY: modeInternal = "wb"; break;
    case EGEFile::MODE_APPEND:     modeInternal = "a+"; break;

    default:

      return EGE_ERROR_BAD_PARAM;
  }

  // open file
  if (NULL == (m_file = s3eFileOpen(d_func()->filePath().c_str(), modeInternal.c_str())))
  {
    // error!
    return EGE_ERROR_IO;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void FilePrivate::close()
{
  if (NULL != m_file)
  {
    s3eFileClose(m_file);
    m_file = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 FilePrivate::read(const PDataBuffer& dst, s64 size)
{
  EGE_ASSERT(dst && (0 <= size));

  // store current write offset in data buffer
  s64 writeOffset = dst->writeOffset();

  if ( ! isOpen())
  {
    // error!
    return 0;
  }

  // make sure buffer is big enough
  if (EGE_SUCCESS != dst->setSize(writeOffset + size))
  {
    // error!
    return 0;
  }

  // read data into buffer
  size_t readCount;
  if ((readCount = s3eFileRead(dst->data(writeOffset), 1, (size_t) size, m_file)) < size)
  {
    // check if EOF found
    if (s3eFileEOF(m_file))
    {
      // this is not error, however, we need to reflect real number of bytes read in buffer itself
      // NOTE: call below should never fail as we r effectively shirnking the data size
      EGEResult result = dst->setSize(writeOffset + readCount);
      EGE_ASSERT(EGE_SUCCESS == result);
    }
    else
    {
      // error!
      return 0;
    }
  }

  // manually update write offset in buffer
  if (writeOffset != dst->setWriteOffset(writeOffset + readCount))
  {
    // error!
    return 0;
  }

  return static_cast<s64>(readCount);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 FilePrivate::write(const PDataBuffer& src, s64 size)
{
  EGE_ASSERT(src);

  if (0 > size)
  {
    size = src->size();
  }

  EGE_ASSERT(0 <= size);

  if ( ! isOpen())
  {
    // error!
    return 0;
  }

  // store current read offset from data buffer
  s64 readOffset = src->readOffset();

  // dont allow to read beyond the size boundary of buffer
  size = Math::Min(size, src->size() - src->readOffset());

  return static_cast<s64>(s3eFileWrite(src->data(readOffset), 1, (size_t) size, m_file));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 FilePrivate::seek(s64 offset, EGEFile::ESeekMode mode) 
{
  if ( ! isOpen())
  {
    // error!
    return -1;
  }

  // map mode
  s3eFileSeekOrigin modeInternal;
  switch (mode)
  {
    case EGEFile::SEEK_MODE_BEGIN:   modeInternal = S3E_FILESEEK_SET; break;
    case EGEFile::SEEK_MODE_CURRENT: modeInternal = S3E_FILESEEK_CUR; break;
    case EGEFile::SEEK_MODE_END:     modeInternal = S3E_FILESEEK_END; break;

    default:

      return -1;
  }

  // store current position
  s64 curPos = tell();

  // try to change position
  if ((-1 == curPos) || (S3E_RESULT_SUCCESS != s3eFileSeek(m_file, (int32) offset, modeInternal)))
  {
    // error!
    return -1;
  }

  return curPos;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 FilePrivate::tell()
{
  if ( ! isOpen())
  {
    // error!
    return -1;
  }

  return s3eFileTell(m_file);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FilePrivate::isOpen() const
{
  return (NULL != m_file);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 FilePrivate::size()
{
  if ( ! isOpen())
  {
    // error!
    return -1;
  }

  // store current file position
  s64 curPos = tell();
  
  // try to skip to end of the file
  if ((-1 != curPos) && (-1 != seek(0, EGEFile::SEEK_MODE_END)))
  {
    // store position
    s64 endPos = tell();

    // return to previous position
    if (-1 != seek(curPos, EGEFile::SEEK_MODE_BEGIN))
    {
      return endPos;
    }
  }

  return -1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FilePrivate::exists() const
{
  return S3E_TRUE == s3eFileCheckExists(d_func()->filePath().c_str());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FilePrivate::remove()
{
  return (S3E_RESULT_SUCCESS == s3eFileDelete(d_func()->filePath().c_str()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END