#ifndef EGE_CORE_DEBUG_H
#define EGE_CORE_DEBUG_H

/** This object represents debug root for framework. Most debugging tools and functionalities have root in this class. 
 */

#include <EGE.h>
#include <EGEString.h>
#include "Core/Debug/DebugFont.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Logger;
class CubicSpline;
class RenderComponent;
class Console;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Debug : public Object
{
  public:

    Debug(Application* app);
    virtual ~Debug();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /* Asserts with given description. */
    static void Assert(const char* description, const char* fileName, s32 lineNumber);
    /* Logs given text to file. */
    static void Log(const String& text);

    /* Prints given c-string to output. */
    static void PrintRaw(const char* text);

    /* Prints given string to output including function name. */
    static void PrintWithFunction(const char* functionName, const String& text);
    /* Prints given text to output as a warning including function name. */
    static void WarningWithFunction(const char* functionName, const String& text);

  private:

    /*! Main log. */
    static Logger* m_log;

  private:

    /*! Debug console. */
    Console* m_console;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DEBUG_H