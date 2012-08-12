#include "Core/Debug/Debug.h"
#include <stdio.h>
#include <s3e.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Performs assertion. */
void Debug::Assert(const char* assertion, const char* file, int line)
{
  EGE_UNUSED(file);
  EGE_UNUSED(line);

  s3eDebugAssertShow(S3E_MESSAGE_CONTINUE_STOP_IGNORE, assertion);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Prints given c-string to output. */
void Debug::Print(const char* text)
{
  s3eDebugOutputString(text);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END