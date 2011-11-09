#include "Core/Debug/Debug.h"
#include <stdio.h>
#include <s3e.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Asserts with given description. */
void Debug::Assert(const char* description, const char* fileName, s32 lineNumber)
{
  EGE_UNUSED(fileName);
  EGE_UNUSED(lineNumber);

  s3eDebugAssertShow(S3E_MESSAGE_CONTINUE_STOP_IGNORE, description);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Prints given text to output. */
void Debug::Print(const String& text)
{
  char buffer[2048];
  sprintf(buffer, "%s\n", text.toAscii());
  s3eDebugOutputString(buffer);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
