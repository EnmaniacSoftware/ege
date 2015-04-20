#include "iOS/File/Interface/FileIOS.h"
#include "iOS/String/Interface/StringHelper.h"
#include "EGEFileUtils.h"
#include "EGEMath.h"
#include "EGEDebug.h"
#import <Foundation/NSFileHandle.h>
#import <Foundation/NSFileManager.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
FilePrivate::FilePrivate(const String& filePath) 
: m_file(nil)
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
  
  // open file
  switch (mode)
  {
    case EFileModeReadOnly:
      
      // first try to open file in Documents folder
      m_file = [NSFileHandle fileHandleForReadingAtPath: StringHelper::Convert(filePath())];
      break;
      
    case EFileModeWriteOnly:
    
      // check if file exists
      if ( ! FileUtils::Exists(filePath()))
      {
        // create new one
        [[NSFileManager defaultManager] createFileAtPath: StringHelper::Convert(filePath()) contents: nil attributes: nil];
      }

      // open
      m_file = [NSFileHandle fileHandleForWritingAtPath: StringHelper::Convert(filePath())];
      
      // truncate to make sure no garbage gets through
      [(id) m_file truncateFileAtOffset: 0];
      break;
    
    case EFileModeWriteAppend:
      
      // check if file exists
      if ( ! FileUtils::Exists(filePath()))
      {
        // create new one
        [[NSFileManager defaultManager] createFileAtPath: StringHelper::Convert(filePath()) contents: nil attributes: nil];
      }

      // open
      m_file = [NSFileHandle fileHandleForWritingAtPath: StringHelper::Convert(filePath())];

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
s64 FilePrivate::read(void* data, s64 length)
//s64 FilePrivate::read(const PDataBuffer& dst, s64 size)
{
  if ( ! isOpen())
  {
    // error!
    return -1;
  }

  // read data into buffer
  NSData* dataBuffer = [(id) m_file readDataOfLength: length];
  
  // retrieve number of bytes read
  s64 readCount = [dataBuffer length];
  
  // check if EOF reached
  if (readCount < length)
  {
    // TAGE - apparently it is not easy to do so with NSFileHandle...
  }
  
  // copy data into destination buffer
  EGE_MEMCPY(data, [dataBuffer bytes], readCount);

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
  
  // convert data
  NSData* dataBuffer = [NSData dataWithBytes: data length: length];
  
  // store to file
  @try
  {
    [(id) m_file writeData: dataBuffer];
  }
  @catch (NSException* exception)
  {
    return -1;
  }
  
  // NOTE: at this point it is assumed 'all' requested data was managed to be written

  return length;
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
        
        [(id) m_file seekToFileOffset: FileUtils::Size(filePath()) + offset];
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
const String& FilePrivate::filePath() const
{
  return m_filePath;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END