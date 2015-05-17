#include "Core/Debug/Interface/Assert.h"
#include <windows.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Assert::DefaultHandleAssert(const char* fileName, s32 lineNo)
{
  // NOTE: these symbols are not defined in Release builds
#ifdef EGE_FEATURE_DEBUG
  _CrtDbgReport(_CRT_ASSERT, fileName, lineNo, NULL, NULL);
  _CrtDbgBreak();
#endif // EGE_FEATURE_DEBUG
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Assert::DefaultHandleAssertX(const char* reason, const char* fileName, s32 lineNo)
{
  // NOTE: these symbols are not defined in Release builds
#ifdef EGE_FEATURE_DEBUG
  _CrtDbgReport(_CRT_ASSERT, fileName, lineNo, NULL, reason);
  _CrtDbgBreak();
#endif // EGE_FEATURE_DEBUG
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END