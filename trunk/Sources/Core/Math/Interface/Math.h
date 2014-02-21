#ifndef EGE_CORE_MATH_H
#define EGE_CORE_MATH_H

#include "EGETypes.h"
#include "EGEAlignment.h"
#include "EGERandom.h"
#include "EGEAngle.h"
#include "EGEDebug.h"
#include "Core/Math/Implementation/MatrixTypes.h"
#include "Core/Math/Implementation/QuaternionTypes.h"
#include "Core/Math/Implementation/Vector2Types.h"
#include "Core/Math/Implementation/Vector3Types.h"
#include "Core/Math/Implementation/Vector4Types.h"
#include "Core/Math/Implementation/Line2Types.h"
#include "Core/Math/Implementation/RectTypes.h"

EGE_NAMESPACE_BEGIN

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
    template <typename T>
    static void Convert(TMatrix4<T>& matrix, const TQuaternion<T>& quaternion);

    /*! Transforms vector by matrix. 
     *  @param  vector  Vector to be tranformed.
     *  @param  matrix  Transformation matrix.
     *  @return Transformed vector.
     */
    template <typename T>
    static TVector4<T> Transform(const TVector4<T>& vector, const TMatrix4<T>& matrix);

    /*! Creates matrix from translation, scale vectors and rotation quaternion. 
     *  @param  translation Translation vector.
     *  @param  scale       Scale vector.
     *  @param  orientation Rotation quaternion.
     *  @return Resulting matrix.
     */
    template <typename T>
    static TMatrix4<T> CreateMatrix(const TVector4<T>& translation, const TVector4<T>& scale, const TQuaternion<T>& orientation);

    /*! Performs linear interpolation between given scalars. 
     *  @param  from First (start) scalar.
     *  @param  to   Second (end) scalar.
     *  @param  time Scalar in range [0..1] describing relative distance between input scalar for which interpolation is to be calculated.
     *  @return Resulting scalar.
     */
    template <typename T>
    static T Lerp(const T& from, const T& to, float32 time);

    /*! Aligns anchor point with respect to given 'virtual frame' size. 
     *  @param point            Point to which 'virtual frame' is attached.
     *  @param frameSize        'Virtual frame' size.
     *  @param pointAlignment   Alignment of the point with respect to 'virtual frame'.
     *  @param newAlignment     Alignment for which position of the point is being calculated.
     *  @return Returns realigned point.
     *  @note This method defines 'virtual frame' relative to a given point and its aligment within the 'frame' i.e. TOP_LEFT alignment of the point
     *        indicates the that Top-Left corner of the frame lies at point position. Next, method calculates position of another point in such a way
     *        that if the same 'frame' (size and alignment) is applied to it, original point alignment with respect to new 'frame' becomes new alignment.
     */
    template <typename T>
    static TVector2<T> Align(const TVector2<T>& point, const TVector2<T>& frameSize, Alignment pointAlignment, Alignment newAlignment);
    /*! Aligns rectangle with respect to another rectangle. 
     *  @param rect             Rectangle to align.
     *  @param otherRect        Another rectangle with respect to which first rectangle is to be aligned.
     *  @param currentAlignment Current rectangle alignment.
     *  @param newAlignment     New rectangle alignment.
     *  @return Returns realigned rectangle.
     */
    template <typename T>
    static TRect<T> Align(const TRect<T>& rect, const TRect<T>& otherRect, Alignment currentAlignment, Alignment newAlignment);
    
    /*! Generates a new random vector which deviates from given vector by a given angle in a random direction.
     *  @param  angle   The angle at which to deviate.
     *  @param  vector  Normalized vector from which deviation should be generated.
     *  @param  up      Any normalized vector perpendicular to this one. If not given the function will derive one.
     *  @returns  A random vector which deviates from this vector by angle. This vector will not be normalized.
     *  @note New vector deviates from original one in [-angle,+angle] range.
     */
    template <typename T>
    static TVector3<T> RandomDeviant(const Angle& angle, const TVector3<T>& vector, const TVector3<T>* up = NULL); // TAGE - VS 2010 seems to crash when optimizing this method while up vector
                                                                                                                   //        is changed to reference...so keeping it as a pointer type
    /*! Generates a new random vector which deviates from given vector by a given angle in a random direction.
     *  @param  angle   The angle at which to deviate.
     *  @param  vector  Normalized vector from which deviation should be generated.
     *  @returns  A random vector which deviates from this vector by angle. This vector will not be normalized.
     *  @note New vector deviates from original one in [-angle,+angle] range.
     */
    template <typename T>
    static TVector2<T> RandomDeviant(const Angle& angle, const TVector2<T>& vector);
    
    /*! Rounds to zero given value if less than default epsilon. 
     *  @param  value Value to be zeroed if close enough to zero.
     *  @return Returned value. Can be either zero or given value.
     */
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
void Math::Convert(TMatrix4<T>& matrix, const TQuaternion<T>& quaternion)
{
  // 1st column
	matrix.data[0] = 1 - 2 * (quaternion.y * quaternion.y + quaternion.z * quaternion.z); 
	matrix.data[1] = 2 * (quaternion.x * quaternion.y - quaternion.z * quaternion.w);
	matrix.data[2] = 2 * (quaternion.x * quaternion.z + quaternion.y * quaternion.w);
	matrix.data[3] = 0;

	// 2nd column
	matrix.data[4] = 2 * (quaternion.x * quaternion.y + quaternion.z * quaternion.w);  
	matrix.data[5] = 1 - 2 * (quaternion.x * quaternion.x + quaternion.z * quaternion.z); 
	matrix.data[6] = 2 * (quaternion.z * quaternion.y - quaternion.x * quaternion.w);  
	matrix.data[7] = 0;

	// 3rd column
	matrix.data[8]  = 2 * (quaternion.x * quaternion.z - quaternion.y * quaternion.w);
	matrix.data[9]  = 2 * (quaternion.y * quaternion.z + quaternion.x * quaternion.w);
	matrix.data[10] = 1 - 2 *(quaternion.x * quaternion.x + quaternion.y * quaternion.y);  
	matrix.data[11] = 0;

	// 4th column
	matrix.data[12] = 0;
	matrix.data[13] = 0;
	matrix.data[14] = 0;  
	matrix.data[15] = 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector4<T> Math::Transform(const TVector4<T>& vector, const TMatrix4<T>& matrix)
{
  TVector4<T> result;

  result.x = matrix.data[0] * vector.x + matrix.data[4] * vector.y + matrix.data[8]  * vector.z + matrix.data[12] * vector.w;
  result.y = matrix.data[1] * vector.x + matrix.data[5] * vector.y + matrix.data[9]  * vector.z + matrix.data[13] * vector.w;
  result.z = matrix.data[2] * vector.x + matrix.data[6] * vector.y + matrix.data[10] * vector.z + matrix.data[14] * vector.w;
  result.w = matrix.data[3] * vector.x + matrix.data[7] * vector.y + matrix.data[11] * vector.z + matrix.data[15] * vector.w;

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TMatrix4<T> Math::CreateMatrix(const TVector4<T>& translation, const TVector4<T>& scale, const TQuaternion<T>& orientation)
{
  // Ordering:
  //    1. Scale
  //    2. Rotate
  //    3. Translate

  const TMatrix4<T> scaleMatrix(scale.x, 0, 0, 0, 0, scale.y, 0, 0, 0, 0, scale.z, 0, 0, 0, 0, scale.w);

  // convert quaternion into rotation matrix
  TMatrix4<T> rotationMatrix;
  Math::Convert(rotationMatrix, orientation);

  // combine 1. and 2.
  TMatrix4<T> matrix = rotationMatrix.multiply(scaleMatrix);

  // apply 3.
  matrix.setTranslation(translation.x, translation.y, translation.z);
  matrix.data[15] = 1;

  return matrix;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T Math::Lerp(const T& from, const T& to, float32 time)
{
  return (1 - time) * from + time * to;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector2<T> Math::Align(const TVector2<T>& point, const TVector2<T>& frameSize, Alignment pointAlignment, Alignment newAlignment)
{
  EGE_ASSERT(0 <= frameSize.x);
  EGE_ASSERT(0 <= frameSize.y);

  TVector2<T> out = point;

  // NOTE: Idea here is, that if we apply original alignment to a newly calculated point, original point gains the required aligment relative to new point and 
  //       'virtual frame' applied to it.
  //
  // Example:
  //
  // Original point P(x,y) with alignment LEFT_CENTER relative to 'virtual frame':
  //
  //       *---------*
  //       |         |
  //       @ P(x,y)  |
  //       |         |
  //       *---------*
  //
  // It is requsted to re-align it to TOP_RIGHT. This means to find the point P' (and 'frame') with original alignment such as the original point will have 
  // requested alignment relative to 'new frame'.
  //
  //      
  //            *---------*
  //            |         |
  //   +--------@ P       |      P has orignal aligment relative to 'original frame'
  //   |        |         |      P' has original alignment relative to 'new frame'
  //   @ p'     *---------*      P has new alignment relative to 'new frame'
  //   |        |
  //   +--------+
  //

  // recalculate horizontal position of new point depending on requirements

  if (pointAlignment & ALIGN_LEFT)
  {
    if (newAlignment & ALIGN_HCENTER)
    {
      out.x -= frameSize.x * 0.5f;
    }
    else if (newAlignment & ALIGN_RIGHT)
    {
      out.x -= frameSize.x;
    }
  }
  else if (pointAlignment & ALIGN_HCENTER)
  {
    if (newAlignment & ALIGN_LEFT)
    {
      out.x += frameSize.x * 0.5f;
    }
    else if (newAlignment & ALIGN_RIGHT)
    {
      out.x -= frameSize.x * 0.5f;
    }
  }
  else if (pointAlignment & ALIGN_RIGHT)
  {
    if (newAlignment & ALIGN_LEFT)
    {
      out.x += frameSize.x;
    }
    else if (newAlignment & ALIGN_HCENTER)
    {
      out.x += frameSize.x * 0.5f;
    }
  }

  // recalculate vertical position of new point depending on requirements

  if (pointAlignment & ALIGN_TOP)
  {
    if (newAlignment & ALIGN_VCENTER)
    {
      out.y -= frameSize.y * 0.5f;
    }
    else if (newAlignment & ALIGN_BOTTOM)
    {
      out.y -= frameSize.y;
    }
  }
  else if (pointAlignment & ALIGN_VCENTER)
  {
    if (newAlignment & ALIGN_TOP)
    {
      out.y += frameSize.y * 0.5f;
    }
    else if (newAlignment & ALIGN_BOTTOM)
    {
      out.y -= frameSize.y * 0.5f;
    }
  }
  else if (pointAlignment & ALIGN_BOTTOM)
  {
    if (newAlignment & ALIGN_TOP)
    {
      out.y += frameSize.y;
    }
    else if (newAlignment & ALIGN_VCENTER)
    {
      out.y += frameSize.y * 0.5f;
    }
  }

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TRect<T> Math::Align(const TRect<T>& rect, const TRect<T>& otherRect, Alignment currentAlignment, Alignment newAlignment)
{
  EGE_ASSERT( ! rect.isNull());
  EGE_ASSERT( ! otherRect.isNull());

  TRect<T> out = rect;

  // align back to TOP-LEFT
  if (currentAlignment & ALIGN_RIGHT)
  {
    out.x -= (otherRect.width - rect.width);
  }
  else if (currentAlignment & ALIGN_HCENTER)
  {
    out.x -= (otherRect.width - rect.width) * 0.5f;
  }

  if (currentAlignment & ALIGN_BOTTOM)
  {
    out.y -= (otherRect.height - rect.height);
  }
  else if (currentAlignment & ALIGN_VCENTER)
  {
    out.y -= (otherRect.height - rect.height) * 0.5f;
  }

  // do new alignment
  if (newAlignment & ALIGN_RIGHT)
  {
    out.x += (otherRect.width - rect.width);
  }
  else if (newAlignment & ALIGN_HCENTER)
  {
    out.x += (otherRect.width - rect.width) * 0.5f;
  }

  if (newAlignment & ALIGN_BOTTOM)
  {
    out.y += (otherRect.height - rect.height);
  }
  else if (newAlignment & ALIGN_VCENTER)
  {
    out.y += (otherRect.height - rect.height) * 0.5f;
  }

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector3<T> Math::RandomDeviant(const Angle& angle, const TVector3<T>& vector, const TVector3<T>* up)
{
  EGE_ASSERT(Math::EPSILON > (1 - vector.lengthSquared()));
  EGE_ASSERT((NULL == up) || ((NULL != up ) && (Math::EPSILON > (1 - up->lengthSquared()))));

  TVector3<T> newUp;

  if (NULL == up)
  {
    // generate an up vector
    newUp = vector.perpendicular();
    newUp.normalize();
  }
  else
  {
    newUp = *up;
  }

  // rotate up vector by random amount around this
  TQuaternion<T> q;
  q.create(vector, Angle(Math::TWO_PI * Random()(-1.0f, 1.0f)));
  newUp = q * newUp;

  // finally rotate this by given angle around randomised up
  q.create(newUp, angle);
  return q * vector;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector2<T> Math::RandomDeviant(const Angle& angle, const TVector2<T>& vector)
{
  const float32 randomization = Random()(-1.0f, 1.0f);

  float32 cos = Math::Cos(angle.radians() * randomization);
  float32 sin = Math::Sin(angle.radians() * randomization);

  return TVector2<T>(vector.x * cos - vector.y * sin, vector.x * sin + vector.y * cos);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector4<T> operator * (const TMatrix4<T>& matrix, const TVector4<T>& vector)
{
  return Math::Transform(vector, matrix);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TVector3<T> operator * (const TQuaternion<T>& quaternion, const TVector3<T>& vector)
{
  // nVidia SDK implementation
  TVector3<T> uv;
  TVector3<T> uuv;
  TVector3<T> qvec(quaternion.x, quaternion.y, quaternion.z);

  uv  = qvec.crossProduct(vector);
  uuv = qvec.crossProduct(uv);
  uv  *= (2 * quaternion.w);
  uuv *= 2;

  return vector + uv + uuv;
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
