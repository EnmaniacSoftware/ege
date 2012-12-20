#include "Core/Debug/Logger.h"
#include <EGEFile.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger::Logger(const String& logFileName, bool timeStamp) : m_referenceCounter(1), m_spaceSeperated(true), m_timeStampEnabled(timeStamp)
{
  m_fileName = logFileName;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger::Logger(const Logger& other) : m_spaceSeperated(other.m_spaceSeperated), m_timeStampEnabled(other.m_timeStampEnabled)
{
  m_fileName          = other.m_fileName;
  m_buffer            = other.m_buffer;
  m_referenceCounter  = other.m_referenceCounter + 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger::~Logger()
{
  if (0 == (--m_referenceCounter))
  {
    // write log
    write();
  }
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
    m_buffer += " ";
  }
  
  return *this; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger& Logger::operator << (bool t)
{ 
  m_buffer += String::Format("%s", t ? "true" : "false"); 
  return maybeSpace(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger& Logger::operator << (char t) 
{ 
  m_buffer += String::Format("%c", t); 
  return maybeSpace(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger& Logger::operator << (Char t) 
{ 
  m_buffer += String::Format("%C", t); 
  return maybeSpace(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger& Logger::operator << (s16 t) 
{ 
  m_buffer += String::Format("%d", t); 
  return maybeSpace(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger& Logger::operator << (u16 t) 
{ 
  m_buffer += String::Format("%d", t); 
  return maybeSpace(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger& Logger::operator << (s32 t) 
{ 
  m_buffer += String::Format("%d", t); 
  return maybeSpace(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger& Logger::operator << (u32 t) 
{ 
  m_buffer += String::Format("%d", t); 
  return maybeSpace(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger& Logger::operator << (s64 t) 
{ 
  m_buffer += String::Format("%d", t); 
  return maybeSpace(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger& Logger::operator << (u64 t) 
{ 
  m_buffer += String::Format("%d", t); 
  return maybeSpace(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger& Logger::operator << (float32 t) 
{ 
  m_buffer += String::Format("%f", t); 
  return maybeSpace(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger& Logger::operator << (const char* t) 
{ 
  m_buffer += t; 
  return maybeSpace(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger& Logger::operator << (const String& t) 
{ 
  m_buffer += t; 
  return maybeSpace(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger& Logger::operator << (const void* t) 
{ 
  m_buffer += String::Format("%p", t); 
  return maybeSpace(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Logger::write()
{
  File file(m_fileName);
  
  // open file
  if (EGE_SUCCESS == file.open(EGEFile::MODE_APPEND))
  {
    // store data  
    DataBuffer buf((void*) m_buffer.toAscii(), m_buffer.length());    
    file.write(buf, buf.size());   
  }
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