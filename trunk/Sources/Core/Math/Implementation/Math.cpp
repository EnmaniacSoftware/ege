#include "Core/Math/Interface/Math.h"
#include "EGEMatrix.h"
#include "EGEVector4.h"
#include "EGEQuaternion.h"
#include "EGEAngle.h"
#include "EGEComplex.h"
#include "EGEColor.h"
#include "EGERect.h"
#include "EGEDebug.h"
#include <limits>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const float32 Math::EPSILON         = std::numeric_limits<float32>::epsilon();
const float32 Math::EPSILON_SQUARED = std::numeric_limits<float32>::epsilon() * std::numeric_limits<float32>::epsilon();
const float32 Math::PI              = 3.14159265358979323846f;
const float32 Math::TWO_PI          = 6.28318530717958647692f;
const float32 Math::PI_HALF         = 1.57079632679489661923f;
const float32 Math::PI_REP          = 0.31830988618379067153f;
const s32     Math::MAX_U32         = std::numeric_limits<u32>::max();
const s32     Math::MIN_U32         = std::numeric_limits<u32>::min();
const s32     Math::MAX_S32         = std::numeric_limits<s32>::max();
const s32     Math::MIN_S32         = std::numeric_limits<s32>::min();
const s32     Math::MAX_S16         = std::numeric_limits<s16>::max();
const s32     Math::MIN_S16         = std::numeric_limits<s16>::min();
const s32     Math::MAX_U16         = std::numeric_limits<u16>::max();
const s32     Math::MIN_U16         = std::numeric_limits<u16>::min();

static RandomGenerator l_random;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Math::Math()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Math::~Math()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Math::Convert(Matrix4f& matrix, const Quaternionf& quaternion)
{
  // 1st column
	matrix.data[0] = 1.0f - 2.0f * (quaternion.y * quaternion.y + quaternion.z * quaternion.z); 
	matrix.data[1] = 2.0f * (quaternion.x * quaternion.y - quaternion.z * quaternion.w);
	matrix.data[2] = 2.0f * (quaternion.x * quaternion.z + quaternion.y * quaternion.w);
	matrix.data[3] = 0;

	// 2nd column
	matrix.data[4] = 2.0f * (quaternion.x * quaternion.y + quaternion.z * quaternion.w);  
	matrix.data[5] = 1.0f - 2.0f * (quaternion.x * quaternion.x + quaternion.z * quaternion.z); 
	matrix.data[6] = 2.0f * (quaternion.z * quaternion.y - quaternion.x * quaternion.w);  
	matrix.data[7] = 0;

	// 3rd column
	matrix.data[8]  = 2.0f * (quaternion.x * quaternion.z - quaternion.y * quaternion.w);
	matrix.data[9]  = 2.0f * (quaternion.y * quaternion.z + quaternion.x * quaternion.w);
	matrix.data[10] = 1.0f - 2.0f *(quaternion.x * quaternion.x + quaternion.y * quaternion.y);  
	matrix.data[11] = 0;

	// 4th column
	matrix.data[12] = 0;
	matrix.data[13] = 0;
	matrix.data[14] = 0;  
	matrix.data[15] = 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector4f Math::Transform(const Vector4f& vector, const Matrix4f& matrix)
{
  Vector4f result;

  result.x = matrix.data[0] * vector.x + matrix.data[4] * vector.y + matrix.data[8]  * vector.z + matrix.data[12] * vector.w;
  result.y = matrix.data[1] * vector.x + matrix.data[5] * vector.y + matrix.data[9]  * vector.z + matrix.data[13] * vector.w;
  result.z = matrix.data[2] * vector.x + matrix.data[6] * vector.y + matrix.data[10] * vector.z + matrix.data[14] * vector.w;
  result.w = matrix.data[3] * vector.x + matrix.data[7] * vector.y + matrix.data[11] * vector.z + matrix.data[15] * vector.w;

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Matrix4f Math::CreateMatrix(const Vector4f& translation, const Vector4f& scale, const Quaternionf& orientation)
{
  // Ordering:
  //    1. Scale
  //    2. Rotate
  //    3. Translate

  const Matrix4f scaleMatrix(scale.x, 0, 0, 0, 0, scale.y, 0, 0, 0, 0, scale.z, 0, 0, 0, 0, scale.w);

  // convert quaternion into rotation matrix
  Matrix4f rotationMatrix;
  Math::Convert(rotationMatrix, orientation);

  // combine 1. and 2.
  Matrix4f matrix = rotationMatrix.multiply(scaleMatrix);

  // apply 3.
  matrix.setTranslation(translation.x, translation.y, translation.z);
  matrix.data[15] = 1.0f;

  return matrix;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 Math::Lerp(float32 from, float32 to, float32 time)
{
  return (1.0f - time) * from + time * to;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Math::LineLineIntersectPoint(Vector2f* out, const Vector2f* line1PointA, const Vector2f* line1PointB, const Vector2f* line2PointA, 
                                  const Vector2f* line2PointB)
{
  EGE_ASSERT(out);
  EGE_ASSERT(line1PointA);
  EGE_ASSERT(line1PointB);
  EGE_ASSERT(line2PointA);
  EGE_ASSERT(line2PointB);

  //// check if points forming the lines are unique
  //if ((Math::EPSILON > Math::Abs(line1PointB->x - line1PointA->x)) || (Math::EPSILON > Math::Abs(line2PointB->x - line2PointA->x)))
  //{
  //  // invalid
  //  return false;
  //}

  //float32 a1 = (line1PointB->y - line1PointA->y) / (line1PointB->x - line1PointA->x);
  //float32 a2 = (line2PointB->y - line2PointA->y) / (line2PointB->x - line2PointA->x);
  //float32 b1 = line1PointA->y - a1 * line1PointA->x;
  //float32 b2 = line2PointA->y - a2 * line2PointA->x;

  //// check if lines are parallel
  //if (Math::EPSILON > Math::Abs(a1 - a2))
  //{
  //  // parallel
  //  return false;
  //}

  //// calculate final point
  //out->x = (b2 - b1) / (a1 - a2);
  //out->y = a1 * out->x + b1;

  // calculate denotmiator
  const float32 denominator = (line1PointA->x - line1PointB->x) * (line2PointA->y - line2PointB->y) - 
                              (line1PointA->y - line1PointB->y) * (line2PointA->x - line2PointB->x);
  if (Math::EPSILON > Math::Abs(denominator))
  {
    // parallel
    return false;
  }

  // calculate some coefficient for optimizaton purposes
  const float32 nominatorCoe1 = (line1PointA->x * line1PointB->y - line1PointA->y * line1PointB->x);
  const float32 nominatorCoe2 = (line2PointA->x * line2PointB->y - line2PointA->y * line2PointB->x);

  out->x = (nominatorCoe1 * (line2PointA->x - line2PointB->x) - (line1PointA->x - line1PointB->x) * nominatorCoe2) / denominator;
  out->y = (nominatorCoe1 * (line2PointA->y - line2PointB->y) - (line1PointA->y - line1PointB->y) * nominatorCoe2) / denominator;

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Math::Align(Vector2f* point, const Vector2f* size, Alignment currentAlignment, Alignment newAlignment)
{
  EGE_ASSERT(point);
  EGE_ASSERT(size);

  // horizontal alignment
  if (currentAlignment & ALIGN_LEFT)
  {
    if (newAlignment & ALIGN_HCENTER)
    {
      point->x -= size->x * 0.5f;
    }
    else if (newAlignment & ALIGN_RIGHT)
    {
      point->x -= size->x;
    }
  }
  else if (currentAlignment & ALIGN_HCENTER)
  {
    if (newAlignment & ALIGN_LEFT)
    {
      point->x += size->x * 0.5f;
    }
    else if (newAlignment & ALIGN_RIGHT)
    {
      point->x -= size->x * 0.5f;
    }
  }
  else if (currentAlignment & ALIGN_RIGHT)
  {
    if (newAlignment & ALIGN_LEFT)
    {
      point->x += size->x;
    }
    else if (newAlignment & ALIGN_HCENTER)
    {
      point->x += size->x * 0.5f;
    }
  }

  // vertical alignment
  if (currentAlignment & ALIGN_TOP)
  {
    if (newAlignment & ALIGN_VCENTER)
    {
      point->y -= size->y * 0.5f;
    }
    else if (newAlignment & ALIGN_BOTTOM)
    {
      point->y -= size->y;
    }
  }
  else if (currentAlignment & ALIGN_VCENTER)
  {
    if (newAlignment & ALIGN_TOP)
    {
      point->y += size->y * 0.5f;
    }
    else if (newAlignment & ALIGN_BOTTOM)
    {
      point->y -= size->y * 0.5f;
    }
  }
  else if (currentAlignment & ALIGN_BOTTOM)
  {
    if (newAlignment & ALIGN_TOP)
    {
      point->y += size->y;
    }
    else if (newAlignment & ALIGN_VCENTER)
    {
      point->y += size->y * 0.5f;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Math::Align(Rectf* rect, Alignment currentAlignment, Alignment newAlignment)
{
  EGE_ASSERT(rect);

  Vector2f point(rect->x, rect->y);
  Vector2f size(rect->width, rect->height);

  Math::Align(&point, &size, currentAlignment, newAlignment);

  rect->x = point.x;
  rect->y = point.y;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Math::Align(Vector4f* point, const Vector2f* size, Alignment currentAlignment, Alignment newAlignment)
{
  EGE_ASSERT(point);
  EGE_ASSERT(size);

  Vector2f newPoint(point->x, point->y);
  Math::Align(&newPoint, size, currentAlignment, newAlignment);

  point->x = newPoint.x;
  point->y = newPoint.y;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Math::Align(Vector4f* point, const Vector4f* size, Alignment currentAlignment, Alignment newAlignment)
{
  EGE_ASSERT(point);
  EGE_ASSERT(size);

  Vector2f newPoint(point->x, point->y);
  Vector2f newSize(size->x, size->y);
  Math::Align(&newPoint, &newSize, currentAlignment, newAlignment);

  point->x = newPoint.x;
  point->y = newPoint.y;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Math::Align(Rectf* rect, const Rectf* otherRect, Alignment currentAlignment, Alignment newAlignment)
{
  EGE_ASSERT(rect);
  EGE_ASSERT(otherRect);

  // align back to TOP-LEFT
  if (currentAlignment & ALIGN_RIGHT)
  {
    rect->x -= (otherRect->width - rect->width);
  }
  else if (currentAlignment & ALIGN_HCENTER)
  {
    rect->x -= (otherRect->width - rect->width) * 0.5f;
  }

  if (currentAlignment & ALIGN_BOTTOM)
  {
    rect->y -= (otherRect->height - rect->height);
  }
  else if (currentAlignment & ALIGN_VCENTER)
  {
    rect->y -= (otherRect->height - rect->height) * 0.5f;
  }

  // do new alignment
  if (newAlignment & ALIGN_RIGHT)
  {
    rect->x += (otherRect->width - rect->width);
  }
  else if (newAlignment & ALIGN_HCENTER)
  {
    rect->x += (otherRect->width - rect->width) * 0.5f;
  }

  if (newAlignment & ALIGN_BOTTOM)
  {
    rect->y += (otherRect->height - rect->height);
  }
  else if (newAlignment & ALIGN_VCENTER)
  {
    rect->y += (otherRect->height - rect->height) * 0.5f;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector3f Math::RandomDeviant(const Angle* angle, const Vector3f* vector, const Vector3f* up)
{
  EGE_ASSERT(angle);
  EGE_ASSERT(vector);

  Vector3f newUp;

  if (NULL == up)
  {
    // Generate an up vector
    newUp = vector->perpendicular();
    newUp.normalize();
  }
  else
  {
    newUp = *up;
  }

  // rotate up vector by random amount around this
  Quaternionf q;
  q.create(*vector, Angle(Math::TWO_PI * l_random(0.0f, 1.0f)));
  newUp = q * newUp;

  // finally rotate this by given angle around randomised up
  q.create(newUp, *angle);
  return q * (*vector);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector2f Math::RandomDeviant(const Angle* angle, const Vector2f* vector)
{
  EGE_ASSERT(angle);
  EGE_ASSERT(vector);

  float32 randomization = l_random(-0.5f, 0.5f);

  float32 cos = Math::Cos(angle->radians() * randomization);
  float32 sin = Math::Sin(angle->radians() * randomization);

  return Vector2f(vector->x * cos - vector->y * sin, vector->x * sin + vector->y * cos);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 Math::GreatestCommonDivisor(s32 a, s32 b)
{
  while (true)
  {
    a = a % b;
    if (0 == a)
    {
      return b;
    }

    b = b % a;
    
    if (0 == b)
    {
      return a;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 Math::ZeroRoundOff(float32 value)
{
  return (Math::EPSILON > Math::Abs(value)) ? 0.0f : value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RandomGenerator& Math::Random()
{
  return l_random;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 Math::RadiansToDegrees(float32 radians) 
{ 
  return radians * 57.29577951f; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 Math::DegreesToRadians(float32 degrees) 
{ 
  return degrees * 0.017453292f; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END