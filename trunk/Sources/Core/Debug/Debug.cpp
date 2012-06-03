#include "Core/Debug/Debug.h"
#include "Core/Debug/Logger.h"
#include "Core/Debug/Console.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Debug)
EGE_DEFINE_DELETE_OPERATORS(Debug)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Logger* Debug::m_log = NULL;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Debug::Debug(Application* app) : Object(app)
{
  m_console = ege_new Console(app);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Debug::~Debug()
{
  EGE_DELETE(m_console);
  EGE_DELETE(m_log);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Logs given text to file. */
void Debug::Log(const String& text)
{
  // check if no logger created yet
  if (NULL == Debug::m_log)
  {
    // create logger
    m_log = ege_new Logger("engine.log", true);
  }

  if (m_log)
  {
    m_log->write(text);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Prints given string to output including function name. */
void Debug::PrintWithFunction(const char* functionName, const String& text)
{
  String finalText = functionName;
  finalText += " ";
  finalText += text;

  PrintRaw(finalText.toAscii());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Prints given text to output as a warning including function name. */
void Debug::WarningWithFunction(const char* functionName, const String& text)
{
  String finalText = functionName;
  finalText += " WARNING: ";
  finalText += text;

  PrintRaw(finalText.toAscii());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END