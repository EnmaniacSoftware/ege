#include "Core/Tools/CommandLineParser.h"
#include <EGEDebug.h>

#ifdef EGE_PLATFORM_WIN32
#include "Win32/Tools/CommandLineParserWin32_p.h"
#elif EGE_PLATFORM_AIRPLAY
#include "Core/Tools/CommandLineParserGeneric.h"
#endif

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CommandLineParser::CommandLineParser() : m_p(NULL)
{
  m_p = ege_new CommandLineParserPrivate(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CommandLineParser::~CommandLineParser()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CommandLineParser::isValid() const
{
  return (NULL != m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END