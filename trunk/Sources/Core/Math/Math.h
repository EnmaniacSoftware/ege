#ifndef EGE_CORE_MATH_H
#define EGE_CORE_MATH_H

//#include "EGE.h"

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

    inline static float32 RadiansToDegrees(float32 radians) { return radians * 57.29577951f; }
    inline static float32 DegreesToRadians(float32 degrees) { return degrees * 0.017453292f; }

    inline static float32 Sqrt(float32 number) { return sqrtf(number); }

    inline static float32 Sin(float32 radians) { return sinf(radians); }
    inline static float32 Cos(float32 radians) { return cosf(radians); }
    inline static float32 Tan(float32 radians) { return tanf(radians); }
    inline static float32 Ctg(float32 radians) { return 1.0f / Tan(radians); }

    static TMatrix4f& Convert(TMatrix4f& matrix, const TQuaternionf& quaternion);

    static TVector4f& Transform(TVector4f& vector, const TMatrix4f& matrix);

    static TMatrix4f& CreateMatrix(TMatrix4f& matrix, const TVector4f& translation, const TVector4f& scale, const TQuaternionf& orientation);

    static float32 DotProduct(const TVector4f& vector1, const TVector4f& vector2);

    //inline static float32 ASin(float32 fRadians);
    //inline static float32 ACos(float32 fRadians);
    //inline static float32 ATan(float32 fRadians);

    static const float32 DELTA;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_H
