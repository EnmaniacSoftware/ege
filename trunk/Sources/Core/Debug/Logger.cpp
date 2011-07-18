#include "Core/Debug/Logger.h"
#include "Core/Data/DataBuffer.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(Logger)
EGE_DEFINE_DELETE_OPERATORS(Logger)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger::Logger(const String& filePath, bool timeStampEnabled) : m_file(filePath), m_timeStampEnabled(timeStampEnabled)
{
  //m_file = NULL;//ege_new File(filePath);

  //// open file for writting
  //if ( this->createLoggerFile( pszFilePath ) == true )
  //{
  //  // initiate Logger entries
  //  this->write( "LoggerGER INITIALIZED!\n\n" );
  //}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger::~Logger()
{
  m_file.close();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool Logger::isValid() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Opens Logger. */
EGEResult Logger::open()
{
  if (isValid())
  {
    return m_file.open(EGEFile::MODE_APPEND);
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Closes Logger. */
void Logger::close()
{
  if (isValid())
  {
    m_file.close();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Writes text. */
EGEResult Logger::write(const String& text)
{
  s64 written = 0;

  if (isValid())
  {
    if (EGE_SUCCESS == open())
    {
      DataBuffer buf((void*) text.toAscii(), text.length() + 1);
      *reinterpret_cast<s8*>(buf.data(text.length())) = 0xA;
      written = m_file.write(buf, buf.size());
      
      close();
    }
  }

  return (written >= (s64) text.length()) ? EGE_SUCCESS : EGE_ERROR;
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
