#ifndef EGE_DEBUG_H
#define EGE_DEBUG_H

#include "Core/Platform.h"
#include "Core/Debug/Debug.h"
#include "Core/Debug/Console.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#ifdef EGE_FEATURE_DEBUG

#define EGE_ASSERT(cond) ((!(cond)) ? Debug::Assert(#cond, __FILE__, __LINE__) : ege_noop())
#define EGE_PRINT(text, ...) Debug::PrintWithFunction(__FUNCTION__, String::Format(#text, ##__VA_ARGS__))
#define EGE_WARNING(text, ...) Debug::WarningWithFunction(__FUNCTION__, String::Format(#text, ##__VA_ARGS__))
#define EGE_LOG(text, ...) Debug::Log(String::Format(#text, ##__VA_ARGS__))

#define CHECK_GL_ERROR() if (GL_NO_ERROR == glGetError()) { EGE_PRINT("%s @ line %d - OpenGL error!", __FUNCTION__, __LINE__); }

#else

#define EGE_ASSERT(cond) ege_noop()
#define EGE_PRINT(text, ...) ege_noop()
#define EGE_WARNING(text, ...) ege_noop()
#define EGE_LOG(text, ...) ege_noop()
#define CHECK_GL_ERROR() ege_noop()

#endif // EGE_FEATURE_DEBUG

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_DEBUG_H