#ifndef EGE_CORE_MATH_H
#define EGE_CORE_MATH_H

#include <EGETypes.h>
#include <EGEAlignment.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T> class TMatrix4;
typedef TMatrix4<float32> Matrix4f;
template <typename T> class TVector2;
typedef TVector2<float32> Vector2f;
template <typename T> class TVector3;
typedef TVector3<float32> Vector3f;
template <typename T> class TVector4;
typedef TVector4<float32> Vector4f;
template <typename T> class TQuaternion;
typedef TQuaternion<float32> Quaternionf;
template <typename T> class TComplex;
typedef TComplex<float32> Complexf;
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
    inline static float32 Ceil(float32 value) { return ceilf(value); }

    inline static float32 RadiansToDegrees(float32 radians) { return radians * 57.29577951f; }
    inline static float32 DegreesToRadians(float32 degrees) { return degrees * 0.017453292f; }

    inline static float32 Sqrt(float32 number) { return sqrtf(number); }

    inline static float32 Sin(float32 radians) { return sinf(radians); }
    inline static float32 Cos(float32 radians) { return cosf(radians); }
    inline static float32 Tan(float32 radians) { return tanf(radians); }
    inline static float32 Ctg(float32 radians) { return 1.0f / Tan(radians); }
    inline static float32 ATan2(float32 y, float32 x) { return atan2f(y, x); }
    inline static float32 Abs(float32 value) { return fabsf(value); }
    static float32 ACos(float32 radians);

    /* Converts quaternion to matrix representation. */
    static void Convert(Matrix4f* matrix, const Quaternionf* quaternion);
    /* Transforms (pre-multiples) vector by matrix. */
    static void Transform(Vector4f* vector, const Matrix4f* matrix);
    /* Creates matrix from translation, scale vectors and quaternion. */
    static void CreateMatrix(Matrix4f* matrix, const Vector4f* translation, const Vector4f* scale, const Quaternionf* orientation);
    /* Returns angle between positive X axis and given point around origin. */
    static void GetAngle(Angle* angle, const Vector2f* origin, const Vector2f* point);

    /*  Performs spherical linear interpolation between given quaternions. 
     *  @param  out           Resulting quaternion.
     *  @param  from          First (start) quaternion.
     *  @param  to            Second (end) quaternion.
     *  @param  time          Scalar in range [0..1] describing relative distance between quaternion for which interpolation is to be calculated.
     *  @param  shortestPath  TRUE if shortest path (if possible) is to be used for interpolation.
     */
    static void Slerp(Quaternionf* out, Quaternionf* from, Quaternionf* to, float32 time, bool shortestPath = false);

    /*  Performs spherical linear interpolation between given complex numbers. 
     *  @param  out  Resulting complex number.
     *  @param  from First (start) complex number.
     *  @param  to   Second (end) complex number.
     *  @param  time Scalar in range [0..1] describing relative distance between numbers for which interpolation is to be calculated.
     */
    static void Slerp(Complexf* out, Complexf* from, Complexf* to, float32 time);

    /*  Performs linear interpolation between given vectors. 
     *  @param  out  Resulting vector.
     *  @param  from First (start) vector.
     *  @param  to   Second (end) vector.
     *  @param  time Scalar in range [0..1] describing relative distance between vectors for which interpolation is to be calculated.
     */
    static void Lerp(Vector2f* out, Vector2f* from, Vector2f* to, float32 time);

    /*  Performs linear interpolation between given scalars. 
     *  @param  from First (start) scalar.
     *  @param  to   Second (end) scalar.
     *  @param  time Scalar in range [0..1] describing relative distance between input scalar for which interpolation is to be calculated.
     *  @return Resulting scalar.
     */
    static float32 Lerp(float32 from, float32 to, float32 time);

    /*  Calculates point on the segment lying closest to given point.
     *  @param  out         Calculated point on the segment lying closest to given point.
     *  @param  linePointA  First point defining line segment.
     *  @param  linePointB  Second point defining line segment.
     *  @param  point       Point for which minimum distance between it and line is to be found.
     *  @note Resulting point always lies on the line segment defined by linePointA and linePointB.
     */
    static void ClosestSegmentPoint(Vector3f* out, const Vector3f* linePointA, const Vector3f* linePointB, const Vector3f* point);
    static void ClosestSegmentPoint(Vector2f* out, const Vector2f* linePointA, const Vector2f* linePointB, const Vector2f* point);

    /*  Calculates point on intersection of two lines.
     *  @param  out           Calculated point of intersection.
     *  @param  line1PointA   First point defining line 1.
     *  @param  line1PointB   Second point defining line 1.
     *  @param  line2PointA   First point defining line 2.
     *  @param  line2PointB   Second point defining line 2.
     *  @note   If return value is FALSE, out value is undefined.
     *  @return Returns TRUE if point of intersection was found. Otherwise FALSE.
     */
    static bool LineLineIntersectPoint(Vector2f* out, const Vector2f* line1PointA, const Vector2f* line1PointB, 
                                       const Vector2f* line2PointA, const Vector2f* line2PointB);

    /* Aligns point. 
     * @param point            Point to align. This point is realigned.
     * @param size             Area within each point is aligned.
     * @param currentAlignment Current point alignment within given area.
     * @param newAlignment     New point alignment within given area.
     */
    static void Align(Vector2f* point, Vector2f* size, Alignment currentAlignment, Alignment newAlignment);
    /* Aligns point along XY plane. 
     * @param point            Point to align. This point is realigned.
     * @param size             Area within each point is aligned.
     * @param currentAlignment Current point alignment within given area.
     * @param newAlignment     New point alignment within given area.
     */
    static void AlignXY(Vector4f* point, Vector2f* size, Alignment currentAlignment, Alignment newAlignment);

  public:

    /*! Inaccuracy cut-off value. */
    static const float32 EPSILON;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_H
