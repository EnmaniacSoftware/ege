#ifndef EGE_CORE_DEBUG_H
#define EGE_CORE_DEBUG_H

#include <EGEString.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Logger;
class CubicSpline;
class RenderComponent;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Debug
{
  public:

    static void Deinit();
    static void Assert(const char* description);
    static void Log(const String& text);
    static void Print(const String& text);

    /* Creates renderable for given object. */
    static RenderComponent* renderable(const CubicSpline* spline);

  private:

    /*! Main log. */
    static Logger* m_log;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DEBUG_H