#include "Core/Debug/Debug.h"
#include <stdio.h>
#include <s3e.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Asserts with given description. */
void Debug::Assert(const char* description, const char* fileName, s32 lineNumber)
{
  EGE_UNUSED(fileName);
  EGE_UNUSED(lineNumber);

  s3eDebugAssertShow(S3E_MESSAGE_CONTINUE_STOP_IGNORE, description);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Prints given c-string to output. */
void Debug::PrintRaw(const char* text)
{
  s3eDebugOutputString(text);
  s3eDebugOutputString("\n");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END