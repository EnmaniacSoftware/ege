#ifndef EGE_DEBUG_H
#define EGE_DEBUG_H

#include "Core/Debug/Interface/Debug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef EGE_FEATURE_DEBUG
  #define EGE_ASSERT(cond) ((!(cond)) ? Debug::Assert(NULL, __FILE__, __LINE__) : ege_noop())
  #define EGE_ASSERT_X(cond, reason) ((!(cond)) ? Debug::Assert(#reason, __FILE__, __LINE__) : ege_noop())
#else
  #define EGE_ASSERT(cond) ege_noop()
  #define EGE_ASSERT_X(cond, reason) ege_noop()
#endif // EGE_FEATURE_DEBUG
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_DEBUG_H