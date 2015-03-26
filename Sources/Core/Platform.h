#ifndef EGE_CORE_PLATFORM_H
#define EGE_CORE_PLATFORM_H

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline void ege_noop() {}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

// Available platform defines
// EGE_PLATFORM_WIN32
// EGE_PLATFORM_AIRPLAY
// EGE_PLATFORM_IOS

// Available rendering APIs
// EGE_RENDERING_OPENGL_FIXED
// EGE_RENDERING_OPENGL_3

// Available physics APIs
// EGE_PHYSICS_BOX2D
// EGE_PHYSICS_NULL

// Availaable threading APIs
// EGE_THREAD_PTHREAD

// Available social platforms
// EGE_SOCIAL_SERVICES_NULL
// EGE_SOCIAL_SERVICES_GAMECENTER

// Available purchase platforms
// EGE_PURCHASE_SERVICES_NULL
// EGE_PURCHASE_SERVICES_APPSTORE

#if WIN32
  #define EGE_PLATFORM_WIN32 1
  #define PLATFORM_CLASSNAME(classname) classname##Win32
#endif // WIN32

#if AIRPLAY_BUILD
  #define EGE_PLATFORM_AIRPLAY 1
#endif // AIRPLAY_BUILD

#ifdef __APPLE__
  #include "TargetConditionals.h"

  #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
    #define EGE_PLATFORM_IOS 1
    #define PLATFORM_CLASSNAME(classname) classname##IOS
  #endif // TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#endif // __APPLE__

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// Common defines
#define EGE_DECLARE_SMART_CLASS(className, smartName) class className; typedef EGE::SmartPointer<className> smartName;

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

#define operators public
#define signals
#define emit
#define slots
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// OS includes
#if EGE_PLATFORM_WIN32
#include <cmath>
#include <assert.h>
#endif // EGE_PLATFORM_WIN32

#if EGE_PLATFORM_AIRPLAY
#include <cmath>
#include <assert.h>
#endif // EGE_PLATFORM_AIRPLAY

#if EGE_PLATFORM_IOS
#include <cmath>
#endif // EGE_PLATFORM_IOS

// Compiler specific
#ifdef __GNUC__
#define override
#define EGE_FUNC_INFO __PRETTY_FUNCTION__
#endif // __GNUC__

#ifdef _MSC_VER
#define EGE_FUNC_INFO __FUNCSIG__
#endif // _MSC_VER
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_CORE_PLATFORM_H