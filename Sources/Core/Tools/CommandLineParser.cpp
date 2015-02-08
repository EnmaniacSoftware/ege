#include "Core/Tools/CommandLineParser.h"
#include "EGEDebug.h"

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
CommandLineParser::CommandLineParser(const StringList& list)
{
  String string;
  for (StringList::const_iterator it = list.begin(); it != list.end(); ++it)
  {
    const String& value = *it;
    
    string += value;
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
  s32 pos = findNextSwitchToken(commandLine, 0);
  while ((0 <= pos) && (pos < commandLine.length()))
  {
    // skip switch token
    pos += strlen(SWITCH_TOKEN);

    // retrieve name
    name = retrieveName(commandLine, pos);

    // update position
    pos += name.length();

    // check if value name is present
    value.clear();
    if ('=' == commandLine.at(pos))
    {
      // skip value token
      ++pos;

      value = retrieveName(commandLine, pos);

      // update position
      pos += value.length();
    }

    // add to dictionary
    m_values.insert(name, value);

    // find next switch token
    pos = findNextSwitchToken(commandLine, pos);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 CommandLineParser::findNextSwitchToken(const String& commandLine, s32 pos)
{
  return commandLine.indexOf(SWITCH_TOKEN, pos);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String CommandLineParser::retrieveName(const String& commandLine, s32 pos)
{
  String name;

  while (pos < commandLine.length())
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