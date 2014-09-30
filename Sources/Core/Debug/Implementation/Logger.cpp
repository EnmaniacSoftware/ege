#include "Core/Debug/Interface/Logger.h"
#include "EGEFile.h"

EGE_NAMESPACE_BEGIN
  
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger::Logger(const String& logFileName, bool timeStamp) : m_fileName(logFileName)
                                                          , m_spaceSeperated(true)
                                                          , m_timeStampEnabled(timeStamp)
{
  // allocate buffer
  m_buffer = ege_new StringBuffer();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger::Logger(const Logger& other)
{
  *this = other;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger::~Logger()
{
  if ((NULL != m_buffer) && (1 == m_buffer->referenceCount()))
  {
    // write log
    write();
  }

  // clean up
  m_buffer = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger& Logger::space() 
{ 
  m_spaceSeperated = true; 
  return maybeSpace(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger& Logger::nospace() 
{ 
  m_spaceSeperated = false; 
  return *this; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger& Logger::maybeSpace() 
{ 
  if (m_spaceSeperated)
  {
    *m_buffer << " ";
  }
  
  return *this; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger& Logger::operator << (bool t)
{ 
  *m_buffer << t; 
  return maybeSpace(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger& Logger::operator << (s16 t) 
{ 
  *m_buffer << t; 
  return maybeSpace(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger& Logger::operator << (u16 t) 
{ 
  *m_buffer << t; 
  return maybeSpace(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger& Logger::operator << (s32 t) 
{ 
  *m_buffer << t; 
  return maybeSpace(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger& Logger::operator << (u32 t) 
{ 
  *m_buffer << t; 
  return maybeSpace(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//Logger& Logger::operator << (size_t t)
//{
//  m_buffer += String::Format("%lu", t);
//  return maybeSpace();
//}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger& Logger::operator << (s64 t) 
{ 
  *m_buffer << t; 
  return maybeSpace(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger& Logger::operator << (u64 t) 
{ 
  *m_buffer << t; 
  return maybeSpace(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger& Logger::operator << (float32 t) 
{ 
  *m_buffer << t; 
  return maybeSpace(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger& Logger::operator << (const char* t) 
{ 
  *m_buffer << t; 
  return maybeSpace(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger& Logger::operator << (const String& t) 
{ 
  *m_buffer << t; 
  return maybeSpace(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger& Logger::operator << (const void* t) 
{ 
  *m_buffer << String::Format("%p", t); 
  return maybeSpace(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger& Logger::operator = (const Logger& other)
{
  if (this != &other)
  {
    m_buffer            = other.m_buffer;
    m_spaceSeperated    = other.m_spaceSeperated;
    m_fileName          = other.m_fileName;
    m_timeStampEnabled  = other.m_timeStampEnabled;
  }
  
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Logger::write()
{
  File file(m_fileName);
  
  // open file
  if (EGE_SUCCESS == file.open(EFileModeWriteAppend))
  {
    // add new line character
    *m_buffer << "\n";

    // store data
    DataBuffer buf(const_cast<void*>(reinterpret_cast<const void*>(m_buffer->string().toAscii())), m_buffer->string().length());    
    file.write(buf, buf.size());   
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const PStringBuffer& Logger::buffer() const
{
  return m_buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

/////////////////////////////////////////////////////////////
// PUBLICS

//bool CEnmLoggerger::createLoggerFile( char* pszFilePath )
//{
//  // close opened file if any
//  this->closeLoggerFile();
//
//  // open new file
//  if ( ( this->pLoggerFile = fopen( pszFilePath, "wt" ) ) == NULL )
//  {
//    // error!
//    return false;
//  }
//
//  return true;
//}
//
//void CEnmLoggerger::closeLoggerFile( void )
//{
//  // close Logger file if any
//  if ( this->pLoggerFile != NULL )
//  {
//    // write close message
//    this->write( "CLOSING LoggerGER." );
//
//    // close file
//    fclose( this->pLoggerFile );
//    this->pLoggerFile = NULL;
//  }
//}
//
//bool CEnmLoggerger::write( char* pszText, ... )
//{
//	va_list argumentPtr;
//
//  // check if any text passed
//	if ( pszText == NULL )
//  {
//    // do nothing
//		return true;
//  }
//
//  // check if timestamps enabled
//  if ( this->bTimeStampsEnabled == true && this->isLoggerFileOpened() == true )
//  {
//    // get time as strinng   
//    _strtime( this->szBuffer );
//    strcat( this->szBuffer, " " );
//
//    // write time into file
//    fprintf( this->pLoggerFile, this->szBuffer );
//  }
//
//	va_start( argumentPtr, pszText );
//	vsprintf( this->szBuffer, pszText, argumentPtr );
//	va_end( argumentPtr );	
//
//  // check if Logger opened
//  if ( this->isLoggerFileOpened() == true )
//  {
//    // write message into file
//    fprintf( this->pLoggerFile, this->szBuffer );
//  }
//
//  return true;
//}
//
//void CEnmLoggerger::newLine( void )
//{
//  // check if file opened
//  if ( this->isLoggerFileOpened() == true )
//  {
//    // write new line into file
//    fprintf( this->pLoggerFile, "\n" );
//  }
//}
//
//bool CEnmLoggerger::isLoggerFileOpened( void )
//{
//  return ( this->pLoggerFile != NULL ) ? true : false;
//}
//
//void CEnmLoggerger::enableTimeStamps( bool bEnable )
//{
//  this->bTimeStampsEnabled = bEnable;
//}

/////////////////////////////////////////////////////////////
// PRIVATES

EGE_NAMESPACE_END