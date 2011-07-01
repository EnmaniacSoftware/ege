#ifndef EGE_CORE_MATH_H
#define EGE_CORE_MATH_H

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include <EGETypes.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T> class TMatrix4;
typedef TMatrix4<float32> Matrix4f;
template <typename T> class TVector2;
typedef TVector2<float32> Vector2f;
template <typename T> class TVector4;
typedef TVector4<float32> Vector4f;
template <typename T> class TQuaternion;
typedef TQuaternion<float32> Quaternionf;
class Angle;

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

    template <typename T>
    inline static T Bound(T value, T min, T max) { return (value > max) ? max : ((value < min) ? min : value); }

    inline static float32 Floor(float32 value) { return floorf(value); }

    inline static float32 RadiansToDegrees(float32 radians) { return radians * 57.29577951f; }
    inline static float32 DegreesToRadians(float32 degrees) { return degrees * 0.017453292f; }

    inline static float32 Sqrt(float32 number) { return sqrtf(number); }

    inline static float32 Sin(float32 radians) { return sinf(radians); }
    inline static float32 Cos(float32 radians) { return cosf(radians); }
    inline static float32 Tan(float32 radians) { return tanf(radians); }
    inline static float32 Ctg(float32 radians) { return 1.0f / Tan(radians); }
    inline static float32 ACos(float32 radians) { return acosf(radians); }
    inline static float32 Abs(float32 value) { return fabsf(value); }

    /* Converts quaternion to matrix representation. */
    static void Convert(Matrix4f* matrix, const Quaternionf* quaternion);
    /* Transforms (pre-multiples) vector by matrix. */
    static void Transform(Vector4f* vector, const Matrix4f* matrix);
    /* Creates matrix from translation, scale vectors and quaternion. */
    static void CreateMatrix(Matrix4f* matrix, const Vector4f* translation, const Vector4f* scale, const Quaternionf* orientation);
    /* Returns angle between positive Y axis and given point around origin. */
    static void GetAngle(Angle* angle, const Vector2f* origin, const Vector2f* point);

  public:

    static const float32 DELTA;
    static const float32 PI;
    static const float32 TWO_PI;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_H
