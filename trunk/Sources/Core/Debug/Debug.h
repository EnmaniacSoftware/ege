#ifndef EGE_CORE_DEBUG_H
#define EGE_CORE_DEBUG_H

#include "EGE.h"
#include "Core/Debug/Log.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Debug
{
  public:

    static void Deinit();
    static void Assert(const char* description);
    static void LogText(const EGEString& text);

  private:

    /*! Main log. */
    static Log* m_log;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DEBUG_H