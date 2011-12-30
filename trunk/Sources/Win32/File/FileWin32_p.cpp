#include "Win32/File/FileWin32_p.h"
#include "Core/Data/DataBuffer.h"
#include "Core/Math/Math.h"
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(FilePrivate)
EGE_DEFINE_DELETE_OPERATORS(FilePrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
FilePrivate::FilePrivate(File* base) : m_base(base), 
                                       m_file(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
FilePrivate::~FilePrivate()
{
  close();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool FilePrivate::isValid() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Opens the given file with requested mode. */
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
  if (0 != fopen_s(&m_file, m_base->filePath().c_str(), modeInternal.c_str()))
  {
    // error!
    return EGE_ERROR_IO;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Closes file. */
void FilePrivate::close()
{
  if (NULL != m_file)
  {
    fclose(m_file);
    m_file = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Reads given amount of data into destination buffer. */
/// @return Returns number of bytes read.
s64 FilePrivate::read(const PDataBuffer& dst, s64 size)
{
  EGE_ASSERT(dst && (0 <= size));

  // store current write offset in data buffer
  s64 writeOffset = dst->writeOffset();

  if (!isOpen())
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
      // NOTE: call below should never fail as we r effectively shirnking the data size
      EGE_ASSERT(EGE_SUCCESS == dst->setSize((s64) readCount));
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

  return (s64) readCount;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Writes given amount of data from destination buffer. */
/// @return Returns number of bytes written.
s64 FilePrivate::write(const PDataBuffer& src, s64 size)
{
  EGE_ASSERT(src);

  if (-1 == size)
  {
    size = src->size();
  }

  EGE_ASSERT(0 <= size);

  if (!isOpen())
  {
    // error!
    return 0;
  }

  // store current read offset from data buffer
  s64 readOffset = src->readOffset();

  // dont allow to read beyond the size boundary of buffer
  size = Math::Min(size, src->size() - src->readOffset());

  return (s64) fwrite(src->data(readOffset), 1, (size_t) size, m_file);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets new position within file. Returns old position or -1 if error occured. */
s64 FilePrivate::seek(s64 offset, EGEFile::ESeekMode mode) 
{
  if (!isOpen())
  {
    // error!
    return -1;
  }

  // map mode
  int modeInternal;
  switch (mode)
  {
    case EGEFile::SEEK_MODE_BEGIN:   modeInternal = SEEK_SET; break;
    case EGEFile::SEEK_MODE_CURRENT: modeInternal = SEEK_CUR; break;
    case EGEFile::SEEK_MODE_END:     modeInternal = SEEK_END; break;

    default:

      return -1;
  }

  // store current position
  s64 curPos = tell();

  // try to change position
  if ((-1 == curPos) || (0 != _fseeki64(m_file, offset, modeInternal)))
  {
    // error!
    return -1;
  }

  return curPos;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns current position in file. Returns -1 if error occured. */
s64 FilePrivate::tell()
{
  if (!isOpen())
  {
    // error!
    return -1;
  }

  return _ftelli64(m_file);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if file is opened. */
bool FilePrivate::isOpen() const
{
  return NULL != m_file;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns file size. Returns -1 if error occured. */
s64 FilePrivate::size()
{
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
/*! Returns TRUE if file exists. */
bool FilePrivate::exists() const
{
  bool exists = false;

  // try to open file
  FILE* file;
  if (0 == fopen_s(&file, m_base->filePath().c_str(), "r"))
  {
    // exists
    exists = true;

    // close file
    fclose(file);
  }
  
  return exists;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
