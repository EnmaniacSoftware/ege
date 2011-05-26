#ifndef EGE_CORE_MATH_H
#define EGE_CORE_MATH_H

#include "Core/Math/Angle.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Math
{
  public:

    Math();
   ~Math();

    template <typename T>
    inline static T Min(T a, T b) { return (a > b) ? b : a; }

    template <typename T>
    inline static T Max(T a, T b) { return (a > b) ? a : b; }

    inline static float32 Floor(float32 value) { return floorf(value); }

    inline static float32 RadiansToDegrees(float32 radians) { return radians * 57.29577951f; }
    inline static float32 DegreesToRadians(float32 degrees) { return degrees * 0.017453292f; }

    inline static float32 Sqrt(float32 number) { return sqrtf(number); }

    inline static float32 Sin(float32 radians) { return sinf(radians); }
    inline static float32 Cos(float32 radians) { return cosf(radians); }
    inline static float32 Tan(float32 radians) { return tanf(radians); }
    inline static float32 Ctg(float32 radians) { return 1.0f / Tan(radians); }
    inline static float32 ACos(float32 radians) { return acosf(radians); }

    /* Coverts quaternion to matrix representation. */
    static Matrix4f& Convert(Matrix4f& matrix, const Quaternionf& quaternion);
    /* Transforms (pre-multiples) vector by matrix. */
    static Vector4f Transform(const Vector4f& vector, const Matrix4f& matrix);
    /* Creates matrix from translation, scale vectors and quaternion. */
    static Matrix4f& CreateMatrix(Matrix4f& matrix, const Vector4f& translation, const Vector4f& scale, const Quaternionf& orientation);
    /* Returns Dot-Product of given vectors. */
    static float32 DotProduct(const Vector4f& vector1, const Vector4f& vector2);
    /* Returns angle between positive Y axis and given point around origin. */
    static Angle GetAngle(const Vector2f& origin, const Vector2f& point);

  public:

    static const float32 DELTA;
    static const float32 PI;
    static const float32 TWO_PI;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_H
