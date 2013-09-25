#include "iOS/File/FileIOS_p.h"
#include "Core/Math/Math.h"
#include "EGEDebug.h"
#import <Foundation/NSFileHandle.h>
#import <Foundation/NSFileManager.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Path to application folder. */
static String l_applicationFolderPath;
/*! Path to documents folder. */
static String l_documentsFolderPath;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(FilePrivate)
EGE_DEFINE_DELETE_OPERATORS(FilePrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function for converting file path from EGE to iOS format. */
NSString* FilePathToNative(const String& path, const String& root)
{
  // compose full path
  String fullPath = root + "/" + path;
  
  // convert file path
  NSString* filePath = [NSString stringWithCString: fullPath.c_str() encoding: NSASCIIStringEncoding];

  return filePath;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
FilePrivate::FilePrivate(File* base) : m_d(base),
                                       m_file(nil)
{
  if (l_documentsFolderPath.empty())
  {
    // get the documents directory
    NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString* path = [paths objectAtIndex: 0];
  
    l_documentsFolderPath = [path cStringUsingEncoding: NSASCIIStringEncoding];
  }
  
  if (l_applicationFolderPath.empty())
  {
    // get application directory
    NSString* path = [[NSBundle mainBundle] bundlePath];
  
    l_applicationFolderPath = [path cStringUsingEncoding: NSASCIIStringEncoding];
  }
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
  NSString* documentsFilePath   = FilePathToNative(d_func()->filePath(), l_documentsFolderPath);
  NSString* applicationFilePath = FilePathToNative(d_func()->filePath(), l_applicationFolderPath);
  
  // open file
  switch (mode)
  {
    case EGEFile::MODE_READ_ONLY:
      
      // first try to open file in Documents folder
      m_file = [NSFileHandle fileHandleForReadingAtPath: documentsFilePath];
      
      // check if failed
      if (nil == m_file)
      {
        // try to open file in application folder
        m_file = [NSFileHandle fileHandleForReadingAtPath: applicationFilePath];        
      }
      break;
      
    case EGEFile::MODE_WRITE_ONLY:
    
      // check if file exists
      if ( ! exists())
      {
        // create new one
        [[NSFileManager defaultManager] createFileAtPath: documentsFilePath contents: nil attributes: nil];
      }

      // open
      m_file = [NSFileHandle fileHandleForWritingAtPath: documentsFilePath];
      break;
    
    case EGEFile::MODE_APPEND:
      
      m_file = [NSFileHandle fileHandleForUpdatingAtPath: documentsFilePath];
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
  
  // convert file path
  NSString* documentsFilePath   = FilePathToNative(d_func()->filePath(), l_documentsFolderPath);
  NSString* applicationFilePath = FilePathToNative(d_func()->filePath(), l_applicationFolderPath);
    
  // first try to retrieve file info from documents folder
  NSDictionary* dict = [[NSFileManager defaultManager] attributesOfItemAtPath: documentsFilePath error: nil];
  
  // check if failed
  if (nil == dict)
  {
    // try to retrieve file info from application folder
    dict = [[NSFileManager defaultManager] attributesOfItemAtPath: applicationFilePath error: nil];
  }
  
  // get file size attribute
  s64 size = [dict fileSize];
  
  // check if not found
  if (0 == size)
  {
    // error!
    return -1;
  }
  
  return size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FilePrivate::exists() const
{
  // convert file path
  NSString* documentsFilePath   = FilePathToNative(d_func()->filePath(), l_documentsFolderPath);
  NSString* applicationFilePath = FilePathToNative(d_func()->filePath(), l_applicationFolderPath);
  
  // check if exists
  return (YES == [[NSFileManager defaultManager] fileExistsAtPath: documentsFilePath]) ||
         (YES == [[NSFileManager defaultManager] fileExistsAtPath: applicationFilePath]);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FilePrivate::remove()
{
  // convert file path
  // NOTE: removing can be done in documents folder
  NSString* documentsFilePath = FilePathToNative(d_func()->filePath(), l_documentsFolderPath);
  
  // remove file
  return (YES == [[NSFileManager defaultManager] removeItemAtPath: documentsFilePath error: nil]);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END