#include "Win32/File/Interface/FileWin32.h"
#include "EGEFileUtils.h"
#include "EGEDataBuffer.h"
#include "EGEMath.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
FilePrivate::FilePrivate(const String& filePath) 
: m_file(NULL)
, m_filePath(filePath)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
FilePrivate::~FilePrivate()
{
  close();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult FilePrivate::open(FileMode mode)
{
  close();

  // map mode
  String modeInternal;
  switch (mode)
  {
    case EFileModeReadOnly:     modeInternal = "rb"; break;
    case EFileModeWriteOnly:    modeInternal = "wb"; break;
    case EFileModeWriteAppend:  modeInternal = "ab"; break;

    default:

      return EGE_ERROR_BAD_PARAM;
  }

  // open file
  if (0 != fopen_s(&m_file, filePath().toAscii(), modeInternal.toAscii()))
  {
    // error!
    return EGE_ERROR_IO;
  }

  // check if append mode requested
  if (EFileModeWriteAppend == mode)
  {
    // move file pointer to the end
    // NOTE: 'ab' mode does not move file pointer before first I\O operation happens
    if (0 != seek(0, EFileSeekEnd))
    {
      // error!
      close();
      return EGE_ERROR_IO;
    }
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void FilePrivate::close()
{
  if (NULL != m_file)
  {
    fclose(m_file);
    m_file = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 FilePrivate::read(const PDataBuffer& dst, s64 size)
{
  EGE_ASSERT(NULL != dst);

  // check if entire file should be read
  if (0 > size)
  {
    size = FileUtils::Size(filePath());
  }

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
  if ((readCount = fread(dst->data(writeOffset), 1, (size_t) size, m_file)) < size)
  {
    // check if EOF found
    if (feof(m_file))
    {
      // this is not error, however, we need to reflect real number of bytes read in buffer itself
      // NOTE: call below should never fail as we are effectively shirnking the data size
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
  EGE_ASSERT(NULL != src);

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
  const s64 readOffset = src->readOffset();

  // dont allow to read beyond the size boundary of buffer
  size = Math::Min(size, src->size() - src->readOffset());

  // write bytes
  const size_t bytesWritten = fwrite(src->data(readOffset), 1, static_cast<size_t>(size), m_file);

  // manually update read offset in the buffer
  if (readOffset != src->setReadOffset(readOffset + bytesWritten))
  {
    // error!
    return 0;
  }

  return static_cast<s64>(bytesWritten);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 FilePrivate::seek(s64 offset, FileSeek mode) 
{
  if ( ! isOpen())
  {
    // error!
    return -1;
  }

  // map mode
  int modeInternal;
  switch (mode)
  {
    case EFileSeekBegin:   modeInternal = SEEK_SET; break;
    case EFileSeekCurrent: modeInternal = SEEK_CUR; break;
    case EFileSeekEnd:     modeInternal = SEEK_END; break;

    default:

      return -1;
  }

  // store current position
  const s64 curPos = tell();

  // try to change position
  // NOTE: seeking beyond the the file content is NOT an error!
  //       However, seeking to locations 'before' the content IS!
  if ((-1 == curPos) || (0 != _fseeki64(m_file, offset, modeInternal)))
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

  return _ftelli64(m_file);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FilePrivate::isOpen() const
{
  return (NULL != m_file);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& FilePrivate::filePath() const
{
  return m_filePath;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END
