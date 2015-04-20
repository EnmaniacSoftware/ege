#include "Win32/File/Interface/FileWin32.h"
#include "EGEDataBuffer.h"
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
s64 FilePrivate::read(void* data, s64 length)
{
  if ( ! isOpen())
  {
    // error!
    return -1;
  }

  // read data
  const size_t readCount = fread(data, 1, static_cast<size_t>(length), m_file);
  if (readCount < length)
  {
    // check if EOF found
    if ( ! feof(m_file))
    {
      // error!
      return -1;
    }
  }

  return readCount;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 FilePrivate::write(const void* data, s64 length)
{
  if ( ! isOpen())
  {
    // error!
    return -1;
  }

  // write bytes
  const size_t bytesWritten = fwrite(data, 1, static_cast<size_t>(length), m_file);
  if (bytesWritten < length)
  {
    // error!
    return -1;
  }

  return bytesWritten;
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
