#include "EGEDebug.h"
#include "EGEString.h"
#include <assert.h>
#import <Foundation/Foundation.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Debug::Assert(const char* assertion, const char* file, int line)
{
  // log
  Debug::PrintWithArgs("Assertion: %s, in file: %s @ line %d", assertion, file, line);

  // assert
  assert(false);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Debug::Print(const char* text)
{
  NSLog(@"%s", text);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
