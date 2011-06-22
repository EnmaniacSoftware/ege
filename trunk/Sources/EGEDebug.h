#ifndef EGE_DEBUG_H
#define EGE_DEBUG_H

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "Core/Debug/Debug.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#ifdef EGE_FEATURE_DEBUG

#define EGE_ASSERT(cond) ((!(cond)) ? Debug::Assert(#cond) : ege_noop())
#define EGE_LOG(text) Debug::LogText(text)

#else

#define EGE_ASSERT(cond) ege_noop()
#define EGE_LOG(text) ege_noop()

#endif // EGE_FEATURE_DEBUG

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_DEBUG_H