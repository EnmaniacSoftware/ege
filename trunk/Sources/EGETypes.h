#ifndef EGE_TYPES_H
#define EGE_TYPES_H

#include <EGE.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class TRect;

template <typename T>
class TVector2;

template <typename T>
class TVector3;

template <typename T>
class TVector4;

template <typename T>
class TQuaternion;

template <typename T>
class TMatrix4;

template <typename T>
class TPlane;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

enum EGEResult
{
  EGE_SUCCESS = 0,        // all is ok
  EGE_ERROR,              // general failure
  EGE_ERROR_NO_MEMORY,
  EGE_ERROR_BAD_PARAM,
  EGE_ERROR_NOT_SUPPORTED,
  EGE_ERROR_IO,
  EGE_ERROR_ALREADY_EXISTS,
  EGE_ERROR_NOT_FOUND
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#if EGE_PLATFORM_WIN32
#include "Win32/TypesWin32.h"
#elif EGE_PLATFORM_AIRPLAY
#include "Airplay/TypesAirplay.h"
#endif // EGE_PLATFORM_WIN32

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef wchar_t EGEChar;

#define EGETEXT(txt) L##txt

typedef TRect<s32>            Recti;
typedef TRect<float32>        Rectf;
typedef TVector2<s32>         Vector2i;
typedef TVector2<float32>     Vector2f;
typedef TVector3<s32>         Vector3i;
typedef TVector3<float32>     Vector3f;
typedef TVector4<s32>         Vector4i;
typedef TVector4<float32>     Vector4f;
typedef TQuaternion<float32>  Quaternionf;
typedef TMatrix4<float32>     Matrix4f;
typedef TPlane<float32>       Planef;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_TYPES_H