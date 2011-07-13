#ifndef EGE_CORE_DEBUG_H
#define EGE_CORE_DEBUG_H

#include <EGEString.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Logger;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Debug
{
  public:

    static void Deinit();
    static void Assert(const char* description);
    static void Log(const EGEString& text);
    static void Print(const EGEString& text);

  private:

    /*! Main log. */
    static Logger* m_log;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DEBUG_H