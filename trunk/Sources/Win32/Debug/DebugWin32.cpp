#include "Core/Debug/Debug.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Debug::Assert(const char* description, const char* fileName, s32 lineNumber)
{
  _CrtDbgReport(_CRT_ASSERT, fileName, lineNumber, NULL, description);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Debug::Print(const String& text)
{
  OutputDebugStringA(text.toAscii());
  OutputDebugStringA("\n");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
