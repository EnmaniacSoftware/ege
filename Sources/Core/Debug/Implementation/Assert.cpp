#include "Core/Debug/Interface/Assert.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static egeAssertFunc  l_customAssertFunc = NULL;
static egeAssertXFunc l_customAssertXFunc = NULL;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Assert::InstallHandlers(egeAssertFunc assertFunc, egeAssertXFunc assertXFunc)
{
  l_customAssertFunc  = assertFunc;
  l_customAssertXFunc = assertXFunc;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Assert::HandleAssert(const char* fileName, s32 lineNo)
{
  if (NULL != l_customAssertFunc)
  {
    l_customAssertFunc(fileName, lineNo);
  }
  else
  {
    Assert::DefaultHandleAssert(fileName, lineNo);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Assert::HandleAssertX(const char* reason, const char* fileName, s32 lineNo)
{
  if (NULL != l_customAssertXFunc)
  {
    l_customAssertXFunc(reason, fileName, lineNo);
  }
  else
  {
    Assert::DefaultHandleAssertX(reason, fileName, lineNo);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END