#ifndef EGE_CORE_MATH_H
#define EGE_CORE_MATH_H

#include "EGETypes.h"
#include "EGEAlignment.h"
#include "EGERandom.h"
#include "EGEDebug.h"
#include "Core/Math/Implementation/MatrixTypes.h"
#include "Core/Math/Implementation/QuaternionTypes.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T> class TVector2;
typedef TVector2<float32> Vector2f;
template <typename T> class TVector3;
typedef TVector3<float32> Vector3f;
template <typename T> class TVector4;
typedef TVector4<float32> Vector4f;
template <typename T> class TComplex;
typedef TComplex<float32> Complexf;
template <typename T> class TRect;
typedef TRect<float32> Rectf;
class Angle;
class Color;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Math
{
  public:

    Math();
   ~Math();

    /*! Returns smaller value of the given values.
     *  @param  value1  Value 1.
     *  @param  value2  Value 2.
     *  @return Returns value which is smaller of given two.
     */
    template <typename T>
    static const T& Min(const T& value1, const T& value2);
    /*! Returns greater value of the given values.
     *  @param  value1  Value 1.
     *  @param  value2  Value 2.
     *  @return Returns value which is greater of given two.
     */
    template <typename T>
    static const T& Max(const T& value1, const T& value2);
    /*! Clamps given value to specified interval.
     *  @param  value Value to be evaluated.
     *  @param  min   Minimum allowed value.
     *  @param  max   Maximum allowed value.
     *  @return Clamped value in [min, max] range.
     *  @note Min must be less or equal to max. Otherwise, results are unspecified.
     */
    template <typename T>
    static const T& Clamp(const T& value, const T& min, const T& max);
    /*! Returns square root of a given value.
     *  @param  value Value for which square root is to be calculated.
     *  @return Calculated square root value.
     *  @note Value needs to be non-negative.
     */
    template <typename T>
    static T Sqrt(const T& value);
    /*! Calculates value raised to a given power.
     *  @param  value Base value.
     *  @param  power Power the base value should be raised to.
     *  @return Returns calculated value.
     *  @note Power needs to be non-negative. If value is negative, power should be integer. Otherwise, results are unspecified.
     */
    template <typename T>
    static T Pow(const T& value, const T& power);
    /*! Returns closest smaller integer value to given one.
     *  @param  value Value for which closest integer is to be found.
     *  @return Calculated value.
     */
    template <typename T>
    static s32 Floor(const T& value);
    /*! Returns closest greater integer value to given one.
     *  @param  value Value for which closest integer is to be found.
     *  @return Calculated value.
     */
    template <typename T>
    static s32 Ceil(const T& value);
    /*! Returns absolute value of a given one.
     *  @param  value Value for which absolute value should be calculated.
     *  @return Calculated absolute value.
     */
    template <typename T>
    static T Abs(const T& value);

    /*! Convert radians to degrees.
     *  @param  radians Radians to be converted.
     *  @return Calculated value in degrees.
     */
    static float32 RadiansToDegrees(float32 radians);
    /*! Convert degrees to radians.
     *  @param  degrees Degrees to be converted.
     *  @return Calculated value in radian.
     */
    static float32 DegreesToRadians(float32 degrees);
    /*! Calculates sine of a given angle.
     *  @param  radians Angle in radians.
     *  @return Calculated value.
     */
    template <typename T>
    static T Sin(const T& radians);
    /*! Calculates cosine of a given angle.
     *  @param  radians Angle in radians.
     *  @return Calculated value.
     */
    template <typename T>
    static T Cos(const T& radians);
    /*! Calculates arccosine of a given value.
     *  @param  value Value for which calculations should be done in [-1, 1] range.
     *  @return Calculated angle in range [0, PI].
     */
    template <typename T>
    static T ACos(const T& value);
    /*! Calculates tangent of a given angle.
     *  @param  radians Angle in radians
     *  @return Calculated value.
     */
    template <typename T>
    static T Tan(const T& radians);
    /*! Calculates cotangent of a given angle.
     *  @param  radians Angle in radians.
     *  @return Calculated value.
     */
    template <typename T>
    static T Ctg(const T& radians);
    /*! Calculates arctangent of a y/x quotient.
     *  @param  y Nominator of the quotient value.
     *  @param  x Denominator of the quotient value.
     *  @return Calculated value.
     *  @note Values of X and Y MUST NOT be equal to 0 at the same time.
     */
    template <typename T>
    static T ATan2(const T& y, const T& x);

    /*! Converts rotation desctibed by given quaternion to matrix representation. 
     *  @param  matrix      Matrix alike rotation representation.
     *  @param  quaternion  Rotation represented by quaternion.
     */
    static void Convert(Matrix4f& matrix, const Quaternionf& quaternion);

    /*! Transforms vector by matrix. 
     *  @param  vector  Vector to be tranformed.
     *  @param  matrix  Transformation matrix.
     *  @return Transformed vector.
     */
    static Vector4f Transform(const Vector4f& vector, const Matrix4f& matrix);

    /*! Creates matrix from translation, scale vectors and rotation quaternion. 
     *  @param  translation Translation vector.
     *  @param  scale       Scale vector.
     *  @param  orientation Rotation quaternion.
     *  @return Resulting matrix.
     */
    static Matrix4f CreateMatrix(const Vector4f& translation, const Vector4f& scale, const Quaternionf& orientation);

    /*! Calculates angle between positive X axis and given point around origin. */
    static void GetAngle(Angle* angle, const Vector2f* origin, const Vector2f* point);
    /*! Calculates unit direction vector from given angle. This is relative to positive X axis. */
    static void GetDirection(Vector2f* vector, const Angle* angle);

    /*! Performs spherical linear interpolation between given quaternions. 
     *  @param  out           Resulting quaternion.
     *  @param  from          First (start) quaternion.
     *  @param  to            Second (end) quaternion.
     *  @param  time          Scalar in range [0..1] describing relative distance between quaternion for which interpolation is to be calculated.
     *  @param  shortestPath  TRUE if shortest path (if possible) is to be used for interpolation.
     */
    static void Slerp(Quaternionf* out, const Quaternionf* from, const Quaternionf* to, float32 time, bool shortestPath = false);

    /*! Performs spherical linear interpolation between given complex numbers. 
     *  @param  out  Resulting complex number.
     *  @param  from First (start) complex number.
     *  @param  to   Second (end) complex number.
     *  @param  time Scalar in range [0..1] describing relative distance between numbers for which interpolation is to be calculated.
     */
    static void Slerp(Complexf* out, const Complexf* from, const Complexf* to, float32 time);

    /*! Performs linear interpolation between given vectors. 
     *  @param  out  Resulting vector.
     *  @param  from First (start) vector.
     *  @param  to   Second (end) vector.
     *  @param  time Scalar in range [0..1] describing relative distance between vectors for which interpolation is to be calculated.
     */
    static void Lerp(Vector2f* out, const Vector2f* from, const Vector2f* to, float32 time);
    static void Lerp(Vector3f* out, const Vector3f* from, const Vector3f* to, float32 time);
    static void Lerp(Vector4f* out, const Vector4f* from, const Vector4f* to, float32 time);
    static void Lerp(Matrix4f* out, const Matrix4f* from, const Matrix4f* to, float32 time);

    /*! Performs linear interpolation between given scalars. 
     *  @param  from First (start) scalar.
     *  @param  to   Second (end) scalar.
     *  @param  time Scalar in range [0..1] describing relative distance between input scalar for which interpolation is to be calculated.
     *  @return Resulting scalar.
     */
    static float32 Lerp(float32 from, float32 to, float32 time);

    /*! Performs linear interpolation between given colors. 
     *  @param  out  Resulting color.
     *  @param  from First (start) color.
     *  @param  to   Second (end) color.
     *  @param  time Scalar in range [0..1] describing relative distance between input colors for which interpolation is to be calculated.
     */
    static void Lerp(Color* out, const Color* from, const Color* to, float32 time);

    /*! Calculates point on the segment lying closest to given point.
     *  @param  out         Calculated point on the segment lying closest to given point.
     *  @param  linePointA  First point defining line segment.
     *  @param  linePointB  Second point defining line segment.
     *  @param  point       Point for which minimum distance between it and line is to be found.
     *  @note Resulting point always lies on the line segment defined by linePointA and linePointB.
     */
    static void ClosestSegmentPoint(Vector3f* out, const Vector3f* linePointA, const Vector3f* linePointB, const Vector3f* point);
    static void ClosestSegmentPoint(Vector2f* out, const Vector2f* linePointA, const Vector2f* linePointB, const Vector2f* point);

    /*! Calculates point on intersection of two lines.
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

    /*! Aligns anchor point with respect to given virtual size. 
     *  @param point            Virtual object anchor point. This point is realigned.
     *  @param size             Virtual obeject size.
     *  @param currentAlignment Current point alignment within given area.
     *  @param newAlignment     New point alignment within given area.
     */
    static void Align(Vector2f* point, const Vector2f* size, Alignment currentAlignment, Alignment newAlignment);
    /*! Aligns rectangle. 
     *  @param rect             Rectangle to align.
     *  @param currentAlignment Current rectangle alignment.
     *  @param newAlignment     New rectangle alignment.
     */
    static void Align(Rectf* rect, Alignment currentAlignment, Alignment newAlignment);
    /*! Aligns anchor point with respect to given virtual size. 
     *  @param point            Virtual object anchor point. This point is realigned.
     *  @param size             Virtual obeject size.
     *  @param currentAlignment Current point alignment within given area.
     *  @param newAlignment     New point alignment within given area.
     */
    static void Align(Vector4f* point, const Vector2f* size, Alignment currentAlignment, Alignment newAlignment);
    /*! Aligns anchor point with respect to given virtual size. 
     *  @param point            Virtual object anchor point. This point is realigned.
     *  @param size             Virtual obeject size.
     *  @param currentAlignment Current point alignment within given area.
     *  @param newAlignment     New point alignment within given area.
     */
    static void Align(Vector4f* point, const Vector4f* size, Alignment currentAlignment, Alignment newAlignment);
    /*! Aligns rectangle with respect to another rectangle. 
     *  @param rect             Rectangle to align.
     *  @param otherRect        Another rectangle with respect to which first rectangle is to be aligned.
     *  @param currentAlignment Current rectangle alignment.
     *  @param newAlignment     New rectangle alignment.
     */
    static void Align(Rectf* rect, const Rectf* otherRect, Alignment currentAlignment, Alignment newAlignment);
    /*! Generates a new random vector which deviates from given vector by a given angle in a random direction.
     *  @param  angle   The angle at which to deviate.
     *  @param  vector  Vector from which deviation should be generated.
     *  @param  up      Any vector perpendicular to this one. If not given the function will derive one. 
     *  @returns  A random vector which deviates from this vector by angle. This vector will not be normalized.
     */
    static Vector3f RandomDeviant(const Angle* angle, const Vector3f* vector, const Vector3f* up = NULL);
    /*! Generates a new random vector which deviates from given vector by a given angle in a random direction.
     *  @param  angle   The angle at which to deviate.
     *  @param  vector  Vector from which deviation should be generated.
     *  @returns  A random vector which deviates from this vector by angle. This vector will not be normalized.
     */
    static Vector2f RandomDeviant(const Angle* angle, const Vector2f* vector);
    /*! Calculates greatest common divisor. */
    static s32 GreatestCommonDivisor(s32 a, s32 b);

    /*! Rounds to zero given value if less than default epsilon. */
    static float32 ZeroRoundOff(float32 value);

    /*! Returns random generator. */
    static RandomGenerator& Random();

  public:

    /*! Inaccuracy cut-off value. */
    static const float32 EPSILON;
    /*! Squared inaccuracy cut-off value. */
    static const float32 EPSILON_SQUARED;
    /*! PI number. */
    static const float32 PI;
    /*! Two times PI number. */
    static const float32 TWO_PI;
    /*! PI half number. */
    static const float32 PI_HALF;
    /*! Reciprocal of PI number. */
    static const float32 PI_REP;
    /*! Max 32-bit unsigned integer value. */
    static const s32 MAX_U32;
    /*! Min 32-bit unsigned integer value. */
    static const s32 MIN_U32;
    /*! Max 32-bit signed integer value. */
    static const s32 MAX_S32;
    /*! Min 32-bit signed integer value. */
    static const s32 MIN_S32;
    /*! Max 16-bit signed integer value. */
    static const s32 MAX_S16;
    /*! Min 16-bit signed integer value. */
    static const s32 MIN_S16;
    /*! Max 16-bit unsigned integer value. */
    static const s32 MAX_U16;
    /*! Min 16-bit unsigned integer value. */
    static const s32 MIN_U16;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
const T& Math::Min(const T& value1, const T& value2) 
{ 
  return (value1 > value2) ? value2 : value1; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
const T& Math::Max(const T& value1, const T& value2) 
{ 
  return (value1 > value2) ? value1 : value2; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
const T& Math::Clamp(const T& value, const T& min, const T& max) 
{ 
  EGE_ASSERT(min <= max);

  return (value > max) ? max : ((value < min) ? min : value); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T Math::Sqrt(const T& value)
{
  EGE_ASSERT(0 <= value);

  return sqrt(value);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T Math::Pow(const T& value, const T& power) 
{ 
  EGE_ASSERT(0 <= power);
  EGE_ASSERT((0 <= value) || ((0 > value) && (power == Math::Floor(power))));

  return pow(value, power); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
s32 Math::Floor(const T& value) 
{ 
  return static_cast<s32>(floor(value)); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
s32 Math::Ceil(const T& value) 
{ 
  return static_cast<s32>(ceil(value)); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T Math::Abs(const T& value) 
{ 
  return abs(value); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T Math::Sin(const T& radians)
{
  return sin(radians);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T Math::Cos(const T& radians)
{
  return cos(radians);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T Math::Tan(const T& radians)
{
  return tan(radians);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T Math::Ctg(const T& radians)
{
  const T tangent = tan(radians);

  EGE_ASSERT(0 != tangent);

  return static_cast<T>(1.0) / tangent;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T Math::ATan2(const T& y, const T& x)
{ 
  EGE_ASSERT((x != y) || ((x == y) && (0 != x)));

  return atan2(y, x); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T Math::ACos(const T& value)
{
  EGE_ASSERT(-1 <= value); 
  EGE_ASSERT(1 >= value); 

  return acos(value); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector4<T> operator * (const TMatrix4<T>& matrix, const TVector4<T>& vector)
{
  return Math::Transform(vector, matrix);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_H


  // the function Random() returns a float in the range [0,1]
  //float2 RandomDirection2D()
  //{  
  //  float azimuth = Random() * 2 * pi;  
  //  return float2(cos(azimuth), sin(azimuth);
  //}
  //
  //float3 RandomDirection3D()
  //{  
  //  float z = (2*Random()) - 1; // z is in the range [-1,1]  
  //  float2 planar = RandomDirection2D() * sqrt(1-z*z);  
  //  return float3(planar.x, planar.y, z);
  //}
