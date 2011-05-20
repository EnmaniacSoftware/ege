//#include "Core/EGEngine.h"
#include "Core/Debug/Log.h"
#include "Core/Data/DataBuffer.h"
#include "EGEFile.h"

//
//#include "CEnmLogger.h"
//
//#include <stdio.h>
//#include <stdarg.h>
//#include <time.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Log::Log(const EGEString& filePath, bool timeStampEnabled) : m_file(filePath), m_timeStampEnabled(timeStampEnabled)
{
  //m_file = NULL;//ege_new File(filePath);

  //// open file for writting
  //if ( this->createLogFile( pszFilePath ) == true )
  //{
  //  // initiate log entries
  //  this->write( "LOGGER INITIALIZED!\n\n" );
  //}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Log::~Log()
{
  m_file.close();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool Log::isValid() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Opens log. */
EGEResult Log::open()
{
  if (isValid())
  {
    return m_file.open(EGEFile::MODE_APPEND);
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Closes log. */
void Log::close()
{
  if (isValid())
  {
    m_file.close();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Writes text. */
EGEResult Log::write(const EGEString& text)
{
  s64 written = 0;

  if (isValid())
  {
    if (EGE_SUCCESS == open())
    {
      DataBuffer buf((void*) text.toAscii(), text.length());
      written = m_file.write(&buf, buf.size());
      close();
    }
  }

  return (written >= (s64) text.length()) ? EGE_SUCCESS : EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Writes text with EOL. */
EGEResult Log::writeln(const EGEString& text)
{
  s64 written = 0;

  if (isValid())
  {
    if (EGE_SUCCESS == open())
    {
      DataBuffer buf((void*) text.toAscii(), text.length() + 1);
      *reinterpret_cast<s8*>(buf.data(text.length())) = 0xA;
      written = m_file.write(&buf, buf.size());
      close();
    }
  }

  return (written >= (s64) text.length()) ? EGE_SUCCESS : EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

/////////////////////////////////////////////////////////////
// PUBLICS

//bool CEnmLogger::createLogFile( char* pszFilePath )
//{
//  // close opened file if any
//  this->closeLogFile();
//
//  // open new file
//  if ( ( this->pLogFile = fopen( pszFilePath, "wt" ) ) == NULL )
//  {
//    // error!
//    return false;
//  }
//
//  return true;
//}
//
//void CEnmLogger::closeLogFile( void )
//{
//  // close log file if any
//  if ( this->pLogFile != NULL )
//  {
//    // write close message
//    this->write( "CLOSING LOGGER." );
//
//    // close file
//    fclose( this->pLogFile );
//    this->pLogFile = NULL;
//  }
//}
//
//bool CEnmLogger::write( char* pszText, ... )
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
//  if ( this->bTimeStampsEnabled == true && this->isLogFileOpened() == true )
//  {
//    // get time as strinng   
//    _strtime( this->szBuffer );
//    strcat( this->szBuffer, " " );
//
//    // write time into file
//    fprintf( this->pLogFile, this->szBuffer );
//  }
//
//	va_start( argumentPtr, pszText );
//	vsprintf( this->szBuffer, pszText, argumentPtr );
//	va_end( argumentPtr );	
//
//  // check if log opened
//  if ( this->isLogFileOpened() == true )
//  {
//    // write message into file
//    fprintf( this->pLogFile, this->szBuffer );
//  }
//
//  return true;
//}
//
//void CEnmLogger::newLine( void )
//{
//  // check if file opened
//  if ( this->isLogFileOpened() == true )
//  {
//    // write new line into file
//    fprintf( this->pLogFile, "\n" );
//  }
//}
//
//bool CEnmLogger::isLogFileOpened( void )
//{
//  return ( this->pLogFile != NULL ) ? true : false;
//}
//
//void CEnmLogger::enableTimeStamps( bool bEnable )
//{
//  this->bTimeStampsEnabled = bEnable;
//}

/////////////////////////////////////////////////////////////
// PRIVATES
