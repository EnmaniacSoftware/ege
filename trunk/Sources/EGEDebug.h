#ifndef EGE_DEBUG_H
#define EGE_DEBUG_H

#include "Core/Platform.h"
#include "Core/Debug/Console.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Available debug message types. */
enum DebugMsgType
{
  DMT_NORMAL,               /*!< Normal debug message. Usually, informative. */
  DMT_WARNING,              /*!< Warning debug message. Usually, for recoverable issues. */
  DMT_CRITICAL              /*!< Critical debug message. Usually, for unrecoverable issues. */
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#ifdef EGE_FEATURE_DEBUG

#define EGE_ASSERT(cond) ((!(cond)) ? Debug::Assert(#cond, __FILE__, __LINE__) : ege_noop())
#define EGE_LOG(text, ...) ege_noop()

#else

#define EGE_ASSERT(cond) ege_noop()
#define EGE_LOG(text, ...) ege_noop()
#define CHECK_GL_ERROR() ege_noop()

#endif // EGE_FEATURE_DEBUG

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Core/Debug/Debug.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_DEBUG_H