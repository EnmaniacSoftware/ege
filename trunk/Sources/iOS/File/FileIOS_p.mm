#include "iOS/File/FileIOS_p.h"
#include "Core/Math/Math.h"
#include "EGEDebug.h"
#import <Foundation/NSFileHandle.h>
#import <Foundation/NSFileManager.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(FilePrivate)
EGE_DEFINE_DELETE_OPERATORS(FilePrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
FilePrivate::FilePrivate(File* base) : m_d(base),
                                       m_file(nil)
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

  // convert file path
  NSString* filePath = [NSString stringWithCString: d_func()->filePath().c_str() encoding: NSASCIIStringEncoding];
  
  // open file
  switch (mode)
  {
    case EGEFile::MODE_READ_ONLY:
      
      m_file = [NSFileHandle fileHandleForReadingAtPath: filePath];
      break;
      
    case EGEFile::MODE_WRITE_ONLY:
    
      m_file = [NSFileHandle fileHandleForWritingAtPath: filePath];
      break;
    
    case EGEFile::MODE_APPEND:
      
      m_file = [NSFileHandle fileHandleForUpdatingAtPath: filePath];
      break;

    default:

      return EGE_ERROR_BAD_PARAM;
  }

  return (nil != m_file) ? EGE_SUCCESS : EGE_ERROR_IO;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void FilePrivate::close()
{
  [(id) m_file closeFile];
  m_file = nil;
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
  NSData* data = [(id) m_file readDataOfLength: size];
  
  // retrieve number of bytes read
  s64 readCount = [data length];
  
  // check if EOF reached
  if (readCount < size)
  {
    // this is not error, however, we need to reflect real number of bytes read in buffer itself
    // NOTE: call below should never fail as we are effectively shirnking the data size
    EGEResult result = dst->setSize(writeOffset + readCount);
    EGE_ASSERT(EGE_SUCCESS == result);
  }
  
  // copy data into destination buffer
  EGE_MEMCPY(dst->data(writeOffset), [data bytes], readCount);

  // manually update write offset in buffer
  if (writeOffset != dst->setWriteOffset(writeOffset + readCount))
  {
    // error!
    return 0;
  }

  return readCount;
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

  // convert data
  NSData* data = [NSData dataWithBytes: src->data(readOffset) length: size];
  
  // get current file position
  s64 curPos = tell();
  
  // store to file
  @try
  {
    [(id) m_file writeData: data];
  }
  @catch (NSException *exception)
  {
    return 0;
  }
  
  return tell() - curPos;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 FilePrivate::seek(s64 offset, EGEFile::ESeekMode mode) 
{
  if ( ! isOpen())
  {
    // error!
    return -1;
  }

  // move position
  @try
  {
    switch (mode)
    {
      case EGEFile::SEEK_MODE_BEGIN:
        
        [(id) m_file seekToFileOffset: offset];
        break;
        
      case EGEFile::SEEK_MODE_CURRENT:
        
        [(id) m_file seekToFileOffset: tell() + offset];
        break;
        
      case EGEFile::SEEK_MODE_END:
        
        [(id) m_file seekToFileOffset: size() + offset];
        break;
        
      default:
        
        return -1;
    }
    
  }
  @catch (NSException *exception)
  {
    return -1;
  }

  return tell();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 FilePrivate::tell()
{
  if ( ! isOpen())
  {
    // error!
    return -1;
  }

  return [(id) m_file offsetInFile];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FilePrivate::isOpen() const
{
  return (nil != m_file);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 FilePrivate::size()
{
  if ( ! isOpen())
  {
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
  // convert file path
  NSString* filePath = [NSString stringWithCString: d_func()->filePath().c_str() encoding: NSASCIIStringEncoding];
  
  // check if exists
  return (YES == [[NSFileManager defaultManager] fileExistsAtPath: filePath]);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FilePrivate::remove()
{
  // convert file path
  NSString* filePath = [NSString stringWithCString: d_func()->filePath().c_str() encoding: NSASCIIStringEncoding];
  
  // remove file
  return (YES == [[NSFileManager defaultManager] removeItemAtPath: filePath error: nil]);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END