#include "Core/Debug/Debug.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Asserts with given description. */
void Debug::Assert(const char* description, const char* fileName, s32 lineNumber)
{
  _CrtDbgReport(_CRT_ASSERT, fileName, lineNumber, NULL, description);
  _CrtDbgBreak();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Prints given c-string to output. */
void Debug::PrintRaw(const char* text)
{
  OutputDebugStringA(text);
  OutputDebugStringA("\n");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
