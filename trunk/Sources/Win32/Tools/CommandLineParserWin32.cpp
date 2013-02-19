#include "Win32/Tools/CommandLineParserWin32.h"
#include <windows.h>
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CommandLineParser::CommandLineParser()
{
  parse(GetCommandLine());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CommandLineParser::~CommandLineParser()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void CommandLineParser::parse(const String& commandLine)
{
  bool keyStarted = false;
  bool valueStarted = false;

  // go thru whole command line
  for (s32 i = 0; i < static_cast<s32>(commandLine.length()); ++i)
  {
    char ch = commandLine.at(i);

  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const Dictionary& CommandLineParser::dictionary() const
{
  return m_values;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END