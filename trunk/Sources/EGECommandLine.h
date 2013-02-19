#ifndef EGE_COMMANDLINE_H
#define EGE_COMMANDLINE_H

#include "Core/Platform.h"

#if EGE_PLATFORM_WIN32
#include "Win32/Tools/CommandLineParserWin32.h"
#else
#include "Core/Tools/CommandLineParser.h"
#endif // EGE_PLATFORM_WIN32

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_COMMANDLINE_H