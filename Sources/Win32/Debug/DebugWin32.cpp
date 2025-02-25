#include "Core/Debug/Interface/Debug.h"
#include <windows.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Debug::Assert(const char* assertion, const char* file, int line)
{
#ifdef EGE_FEATURE_DEBUG
  _CrtDbgReport(_CRT_ASSERT, file, line, NULL, assertion);
  _CrtDbgBreak();
#endif // EGE_FEATURE_DEBUG
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Debug::Print(const char* text)
{
  OutputDebugStringA(text);
  OutputDebugStringA("\n");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END