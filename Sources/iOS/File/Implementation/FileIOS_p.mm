#include "iOS/File/Interface/FileIOS_p.h"
#include "iOS/String/Interface/StringHelper.h"
#include "EGEFileUtils.h"
#include "EGEMath.h"
#include "EGEDebug.h"
#import <Foundation/NSFileHandle.h>
#import <Foundation/NSFileManager.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(FilePrivate)
EGE_DEFINE_DELETE_OPERATORS(FilePrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
FilePrivate::FilePrivate(File* base) 
: m_d(base)
, m_file(nil)
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
  
  // open file
  switch (mode)
  {
    case EFileModeReadOnly:
      
      // first try to open file in Documents folder
      m_file = [NSFileHandle fileHandleForReadingAtPath: StringHelper::Convert(d_func()->filePath())];
      break;
      
    case EFileModeWriteOnly:
    
      // check if file exists
      if ( ! FileUtils::Exists(d_func()->filePath()))
      {
        // create new one
        [[NSFileManager defaultManager] createFileAtPath: StringHelper::Convert(d_func()->filePath()) contents: nil attributes: nil];
      }

      // open
      m_file = [NSFileHandle fileHandleForWritingAtPath: StringHelper::Convert(d_func()->filePath())];
      
      // truncate to make sure no garbage gets through
      [(id) m_file truncateFileAtOffset: 0];
      break;
    
    case EFileModeWriteAppend:
      
      // check if file exists
      if ( ! FileUtils::Exists(d_func()->filePath()))
      {
        // create new one
        [[NSFileManager defaultManager] createFileAtPath: StringHelper::Convert(d_func()->filePath()) contents: nil attributes: nil];
      }

      // open
      m_file = [NSFileHandle fileHandleForWritingAtPath: StringHelper::Convert(d_func()->filePath())];

      // move file pointer to the end of the file
      [(id) m_file seekToEndOfFile];
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
  EGE_ASSERT(NULL != dst);

  // check if entire file should be read
  if (0 > size)
  {
    size = FileUtils::Size(d_func()->filePath());
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
  NSData* data = [(id) m_file readDataOfLength: size];
  
  // retrieve number of bytes read
  s64 readCount = [data length];
  
  // check if EOF reached
  if (readCount < size)
  {
    // this is not error, however, we need to reflect real number of bytes read in buffer itself
    // NOTE: call below should never fail as we are effectively shirnking the data size
    EGEResult result = dst->setSize(writeOffset + readCount);
    if (EGE_SUCCESS != result)
    {
      // error!
      EGE_ASSERT(EGE_SUCCESS == result);
      return 0;
    }
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
  EGE_ASSERT(NULL != src);

  // check if entire buffer should be written
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

  // convert data
  NSData* data = [NSData dataWithBytes: src->data(readOffset) length: size];
  
  // store to file
  @try
  {
    [(id) m_file writeData: data];
  }
  @catch (NSException* exception)
  {
    return 0;
  }
  
  // NOTE: at this point it is assumed 'all' requested data was managed to be written

  // manually update read offset in the buffer
  if (readOffset != src->setReadOffset(readOffset + size))
  {
    // error!
    return 0;
  }

  return size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 FilePrivate::seek(s64 offset, FileSeek mode) 
{
  if ( ! isOpen())
  {
    // error!
    return -1;
  }

  // get current file position
  s64 oldPosition = tell();
  
  // move position
  @try
  {
    switch (mode)
    {
      case EFileSeekBegin:
        
        [(id) m_file seekToFileOffset: offset];
        break;
        
      case EFileSeekCurrent:
        
        [(id) m_file seekToFileOffset: tell() + offset];
        break;
        
      case EFileSeekEnd:
        
        [(id) m_file seekToFileOffset: FileUtils::Size(d_func()->filePath()) + offset];
        break;
        
      default:
        
        return -1;
    }
  }
  @catch (NSException *exception)
  {
    return -1;
  }

  return oldPosition;
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

EGE_NAMESPACE_END