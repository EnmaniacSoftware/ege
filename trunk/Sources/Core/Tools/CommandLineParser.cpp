#include "Core/Tools/CommandLineParser.h"
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define SWITCH_TOKEN "--"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CommandLineParser::CommandLineParser(char* commandLine)
{
  String string = commandLine;
  parse(string);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CommandLineParser::CommandLineParser(int argc, char** argv)
{
  String string;
  for (int i = 0; i < argc; ++i)
  {
    string += argv[i];
    string += " ";
  }

  parse(string);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CommandLineParser::~CommandLineParser()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const Dictionary& CommandLineParser::dictionary() const
{
  return m_values;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void CommandLineParser::parse(const String& commandLine)
{
  String name;
  String value;

  // find first switch
  size_t pos = findNextSwitchToken(commandLine, 0);
  while ((String::npos != pos) && (pos < commandLine.size()))
  {
    // skip switch token
    pos += strlen(SWITCH_TOKEN);

    // retrieve name
    name = retrieveName(commandLine, pos);

    // update position
    pos += name.size();

    // check if value name is present
    value.clear();
    if ('=' == commandLine.at(pos))
    {
      // skip value token
      ++pos;

      value = retrieveName(commandLine, pos);

      // update position
      pos += value.size();
    }

    // add to dictionary
    m_values.insert(name, value);

    // find next switch token
    pos = findNextSwitchToken(commandLine, pos);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
size_t CommandLineParser::findNextSwitchToken(const String& commandLine, size_t pos)
{
  return commandLine.find(SWITCH_TOKEN, pos);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String CommandLineParser::retrieveName(const String& commandLine, size_t pos)
{
  String name;

  while (pos < commandLine.size())
  {
    char c = commandLine.at(pos);
    
    // check if termination character is present
    if ((' ' == c) || ('=' == c))
    {
      // done
      break;
    }

    // append to name
    name += c;

    // go to next char
    ++pos;
  }

  return name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END