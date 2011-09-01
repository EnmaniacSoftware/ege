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

    static void Deinit();
    static void Assert(const char* description, const char* fileName, s32 lineNumber);
    static void Log(const String& text);
    static void Print(const String& text);

    /* Creates renderable for given object. */
    static RenderComponent* Renderable(const CubicSpline* spline);

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