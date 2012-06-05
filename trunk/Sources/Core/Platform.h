#ifndef EGE_CORE_PLATFORM_H
#define EGE_CORE_PLATFORM_H

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline void ege_noop() {}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

// Available platform defines
// EGE_PLATFORM_WIN32
// EGE_PLATFORM_AIRPLAY

// Available rendering APIs
// EGE_RENDERING_OPENGL_2
// EGE_RENDERING_OPENGLES_1

// Available physics APIs
// EGE_PHYSICS_BOX2D
// EGE_PHYSICS_NULL

// Availaable threading APIs
// EGE_THREAD_PTHREAD

#if WIN32
#define EGE_PLATFORM_WIN32 1
#endif // WIN32

#if AIRPLAY_BUILD
#define EGE_PLATFORM_AIRPLAY 1
#endif // AIRPLAY_BUILD

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// Common defines
#define EGE_DECLARE_SMART_CLASS(className, smartName) class className; typedef EGE::SmartPointer<className> smartName;
//#define DECLARE_TEMPLATED_SMART_CLASS(className, smartName) template <class T> class className; typedef Pointer<className> smartName;

#define EGE_NAMESPACE_BEGIN namespace EGE {
#define EGE_NAMESPACE_END }
#define EGE_NAMESPACE using namespace EGE;

#define EGE_DECLARE_PRIVATE_IMPLEMENTATION(className) class className##Private* m_p; friend class className##Private; \
                                                      inline className##Private* p_func() { return m_p; } \
                                                      inline const className##Private* p_func() const { return m_p; }

#define EGE_DECLARE_PUBLIC_IMPLEMENTATION(className) class className* m_d; friend class className; \
                                                     inline className* d_func() { return m_d; } \
                                                     inline const className* d_func() const { return m_d; }

#define EGE_UNUSED(x) (void) x;

#define signals public
#define emit
#define slots
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// OS includes
#if EGE_PLATFORM_WIN32
#define NOMINMAX 1
#include <windows.h>
#include <math.h>
#include <assert.h>
#endif // EGE_PLATFORM_WIN32

#if EGE_PLATFORM_AIRPLAY
#include <math.h>
#include <assert.h>
#endif // EGE_PLATFORM_AIRPLAY

// Compiler specific
#ifdef __GNUC__
#define override
#endif // __GNUC__
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_CORE_PLATFORM_H