#include "Core/Debug/Debug.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Asserts with given description. */
void Debug::Assert(const char* description, const char* fileName, s32 lineNumber)
{
#ifdef EGE_FEATURE_DEBUG
  _CrtDbgReport(_CRT_ASSERT, fileName, lineNumber, NULL, description);
  _CrtDbgBreak();
#endif // EGE_FEATURE_DEBUG
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Prints given c-string to output. */
void Debug::PrintRaw(const char* text)
{
  OutputDebugStringA(text);
  OutputDebugStringA("\n");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
