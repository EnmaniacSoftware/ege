#include "Core/Debug/Interface/Assert.h"
#include <assert.h>
#import <Foundation/Foundation.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Assert::DefaultHandleAssert(const char* fileName, s32 lineNo)
{
  // log
  Debug::PrintWithArgs("Assertion in file: %s @ line %d", fileName, lineNo);

  // assert
  assert(false);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Assert::DefaultHandleAssertX(const char* reason, const char* fileName, s32 lineNo)
{
  // log
  Debug::PrintWithArgs("Assertion: %s, in file: %s @ line %d", reason, fileName, lineNo);

  // assert
  assert(false);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END