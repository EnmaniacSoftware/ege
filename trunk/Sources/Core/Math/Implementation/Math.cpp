#include "Core/Math/Interface/Math.h"
#include "EGEMatrix.h"
#include "EGEVector.h"
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
void Math::Convert(Matrix4f* matrix, const Quaternionf* quaternion)
{
  EGE_ASSERT(matrix);
  EGE_ASSERT(quaternion);

  // 1st column
	matrix->data[0] = 1.0f - 2.0f * (quaternion->y * quaternion->y + quaternion->z * quaternion->z); 
	matrix->data[1] = 2.0f * (quaternion->x * quaternion->y - quaternion->z * quaternion->w);
	matrix->data[2] = 2.0f * (quaternion->x * quaternion->z + quaternion->y * quaternion->w);
	matrix->data[3] = 0;

	// 2nd column
	matrix->data[4] = 2.0f * (quaternion->x * quaternion->y + quaternion->z * quaternion->w);  
	matrix->data[5] = 1.0f - 2.0f * (quaternion->x * quaternion->x + quaternion->z * quaternion->z); 
	matrix->data[6] = 2.0f * (quaternion->z * quaternion->y - quaternion->x * quaternion->w);  
	matrix->data[7] = 0;

	// 3rd column
	matrix->data[8]  = 2.0f * (quaternion->x * quaternion->z - quaternion->y * quaternion->w);
	matrix->data[9]  = 2.0f * (quaternion->y * quaternion->z + quaternion->x * quaternion->w);
	matrix->data[10] = 1.0f - 2.0f *(quaternion->x * quaternion->x + quaternion->y * quaternion->y);  
	matrix->data[11] = 0;

	// 4th column
	matrix->data[12] = 0;
	matrix->data[13] = 0;
	matrix->data[14] = 0;  
	matrix->data[15] = 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Math::Transform(Vector4f* vector, const Matrix4f* matrix)
{
  EGE_ASSERT(matrix);
  EGE_ASSERT(vector);

  Vector4f result;

  result.x = matrix->data[0] * vector->x + matrix->data[4] * vector->y + matrix->data[8]  * vector->z + matrix->data[12] * vector->w;
  result.y = matrix->data[1] * vector->x + matrix->data[5] * vector->y + matrix->data[9]  * vector->z + matrix->data[13] * vector->w;
  result.z = matrix->data[2] * vector->x + matrix->data[6] * vector->y + matrix->data[10] * vector->z + matrix->data[14] * vector->w;
  result.w = matrix->data[3] * vector->x + matrix->data[7] * vector->y + matrix->data[11] * vector->z + matrix->data[15] * vector->w;

  *vector = result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Math::CreateMatrix(Matrix4f* matrix, const Vector4f* translation, const Vector4f* scale, const Quaternionf* orientation)
{
  EGE_ASSERT(matrix);
  EGE_ASSERT(translation);
  EGE_ASSERT(scale);
  EGE_ASSERT(orientation);

  // Ordering:
  //    1. Scale
  //    2. Rotate
  //    3. Translate

  Matrix4f rotationMatrix;
  Matrix4f scaleMatrix(scale->x, 0, 0, 0, 0, scale->y, 0, 0, 0, 0, scale->z, 0, 0, 0, 0, scale->w);

  // convert quaternion into rotation matrix
  Math::Convert(&rotationMatrix, orientation);

  // combine 1. and 2.
  *matrix = rotationMatrix.multiply(scaleMatrix);

  // apply 3.
  matrix->setTranslation(translation->x, translation->y, translation->z);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Math::GetAngle(Angle* angle, const Vector2f* origin, const Vector2f* point)
{
  EGE_ASSERT(angle);
  EGE_ASSERT(origin);
  EGE_ASSERT(point);
  EGE_ASSERT(!(origin->x == point->x && origin->y == point->y));

  // NOTE: arguments are in (Y, X) order.
  //       we assume X axis grows from LEFT to RIGHT
  //       we assume Y axis grows from BOTTOM to TOP
  angle->fromRadians(Math::ATan2(origin->y - point->y, point->x - origin->x));

  //if (point->x >= origin->x)
  //{
  //  angle->fromRadians(Math::ACos((origin->y - point->y) / origin->distanceTo(*point)));
  //}
  //else
  //{
  //  angle->fromRadians(Math::ACos(-(origin->y - point->y) / origin->distanceTo(*point)) + EGEMath::PI);
  //}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Math::Slerp(Quaternionf* out, const Quaternionf* from, const Quaternionf* to, float32 time, bool shortestPath)
{
  EGE_ASSERT(out);
  EGE_ASSERT(from);
  EGE_ASSERT(to);
  
  Quaternionf tmp;

  //
  // p     - source quaternion
  // q     - destination quaternion
  // t     - time
  // omega - angle between p and q
  //
  //                    p*sin( ( 1-t )*omega )+q*sin( t*omega )
  // slerp( p, q, t ) = ---------------------------------------
  //                                 sin( omega )
  //

  // calculate cosine omega (dot product of 2 quaternions)
  float32 cosOmega = from->dotProduct(*to);

  // adjust the signs
  if ((0.0f > cosOmega) && shortestPath)
  {
    cosOmega = -cosOmega;

    // NOTE: q and -q rotates from the same start point and to the same end point but thru opposite direction
    tmp = -(*to);
  }
  else
  {
    tmp = *to;
  }

  // calculate coefficients
  if ((1.0f - Math::EPSILON) > Math::Abs(cosOmega))
  {
    // standard case (slerp)
    float32 sinOmega = Math::Sqrt(1 - cosOmega * cosOmega);
    float32 invSinOmega = 1.0f / sinOmega;

    float32 angle = Math::ATan2(sinOmega, cosOmega);

    float32 coeff0 = Math::Sin((1.0f - time) * angle) * invSinOmega;
    float32 coeff1 = Math::Sin(time * angle) * invSinOmega;

    *out = coeff0 * (*from) + coeff1 * tmp;
  }
  else
  {
    // There are two situations:
    // 1. "from" and "to" are very close (cos ~= +1), so we can do a linear interpolation safely.
    // 2. "from" and "to" are almost inverse of each other (cos ~= -1), there are an infinite number of possibilities interpolation. 
    //    Do linear interpolation here as well as no other way to fix it yet.
    *out = (1.0f - time) * (*from) + time * tmp;

    // taking the complement requires renormalisation
    out->normalize();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Math::Slerp(Complexf* out, const Complexf* from, const Complexf* to, float32 time)
{
  EGE_ASSERT(out);
  EGE_ASSERT(from);
  EGE_ASSERT(to);

  Complexf tmp;

  float32 fSinOmega;
  float32 fCosOmega;
  float32 fOmega;
  float32 fScale0;
  float32 fScale1;

  // calculate cosine omega (dot product of 2 quaternions)
  fCosOmega = from->dotProduct(*to);

  // adjust the signs
 // if ( fCosOmega < 0 )
  {
    tmp = *to;
  }
 // else
  {
 //   cTemp.m_fReal = cDestination.m_fReal;
 //   cTemp.m_fImaginary = cDestination.m_fImaginary;
  }

  // calculate coefficients
  if ( 1.0f-fCosOmega > ( float ) 1e-6 )
  {
    // standard case
    fOmega    = Math::ACos( fCosOmega );
    fSinOmega = Math::Sin( fOmega );

    fScale0 = Math::Sin( ( 1.0f-time )*fOmega )/fSinOmega;
    fScale1 = Math::Sin( time*fOmega )/fSinOmega;
  }
  else
  {
    // source and destination quaternions are very close so we perform linear interpolation
    fScale0 = 1.0f-time;
    fScale1 = time;
  }

	// calculate final values
	out->x = fScale0*from->x+fScale1*tmp.x;
	out->y = fScale0*from->y+fScale1*tmp.y;

  out->normalize();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Math::Lerp(Vector2f* out, const Vector2f* from, const Vector2f* to, float32 time)
{
  EGE_ASSERT(out);
  EGE_ASSERT(from);
  EGE_ASSERT(to);

  *out = (1.0f - time) * (*from) + time * (*to);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Math::Lerp(Vector3f* out, const Vector3f* from, const Vector3f* to, float32 time)
{
  EGE_ASSERT(out);
  EGE_ASSERT(from);
  EGE_ASSERT(to);

  *out = (1.0f - time) * (*from) + time * (*to);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Math::Lerp(Vector4f* out, const Vector4f* from, const Vector4f* to, float32 time)
{
  EGE_ASSERT(out);
  EGE_ASSERT(from);
  EGE_ASSERT(to);

  *out = (1.0f - time) * (*from) + time * (*to);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Math::Lerp(Matrix4f* out, const Matrix4f* from, const Matrix4f* to, float32 time)
{
  EGE_ASSERT(out);
  EGE_ASSERT(from);
  EGE_ASSERT(to);

  const float32 oneMinusTime = (1.0f - time);

  out->data[0]  = oneMinusTime * from->data[0]  + time * to->data[0];
  out->data[1]  = oneMinusTime * from->data[1]  + time * to->data[1];
  out->data[2]  = oneMinusTime * from->data[2]  + time * to->data[2];
  out->data[3]  = oneMinusTime * from->data[3]  + time * to->data[3];
  out->data[4]  = oneMinusTime * from->data[4]  + time * to->data[4];
  out->data[5]  = oneMinusTime * from->data[5]  + time * to->data[5];
  out->data[6]  = oneMinusTime * from->data[6]  + time * to->data[6];
  out->data[7]  = oneMinusTime * from->data[7]  + time * to->data[7];
  out->data[8]  = oneMinusTime * from->data[8]  + time * to->data[8];
  out->data[9]  = oneMinusTime * from->data[9]  + time * to->data[9];
  out->data[10] = oneMinusTime * from->data[10] + time * to->data[10];
  out->data[11] = oneMinusTime * from->data[11] + time * to->data[11];
  out->data[12] = oneMinusTime * from->data[12] + time * to->data[12];
  out->data[13] = oneMinusTime * from->data[13] + time * to->data[13];
  out->data[14] = oneMinusTime * from->data[14] + time * to->data[14];
  out->data[15] = oneMinusTime * from->data[15] + time * to->data[15];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 Math::Lerp(float32 from, float32 to, float32 time)
{
  return (1.0f - time) * from + time * to;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Math::Lerp(Color* out, const Color* from, const Color* to, float32 time)
{
  EGE_ASSERT(out);
  EGE_ASSERT(from);
  EGE_ASSERT(to);

  float32 oneMinusTime = 1.0f - time;

  out->red    = oneMinusTime * from->red + time * to->red;
  out->green  = oneMinusTime * from->green + time * to->green;
  out->blue   = oneMinusTime * from->blue + time * to->blue;
  out->alpha  = oneMinusTime * from->alpha + time * to->alpha;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Math::ClosestSegmentPoint(Vector3f* out, const Vector3f* linePointA, const Vector3f* linePointB, const Vector3f* point)
{
  EGE_ASSERT(out);
  EGE_ASSERT(linePointA);
  EGE_ASSERT(linePointB);
  EGE_ASSERT(point);

  Vector3f lineDir = (*linePointB) - (*linePointA);

  float32 lineLength = lineDir.length();

  lineDir.normalize();

  Vector3f dist = (*point) - (*linePointA);

  float32 t = lineDir.dotProduct(dist);

  if (0 > t)
  {
    *out = *linePointA;
  }
  else if (t >= lineLength)
  {
    *out = *linePointB;
  }
  else
  {
    *out = *linePointA + lineDir * t;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Math::ClosestSegmentPoint(Vector2f* out, const Vector2f* linePointA, const Vector2f* linePointB, const Vector2f* point)
{
  EGE_ASSERT(out);
  EGE_ASSERT(linePointA);
  EGE_ASSERT(linePointB);
  EGE_ASSERT(point);

  Vector2f lineDir = (*linePointB) - (*linePointA);

  float32 lineLength = lineDir.length();

  lineDir.normalize();

  Vector2f dist = (*point) - (*linePointA);

  float32 t = lineDir.dotProduct(dist);

  if (0 > t)
  {
    *out = *linePointA;
  }
  else if (t >= lineLength)
  {
    *out = *linePointB;
  }
  else
  {
    *out = *linePointA + lineDir * t;
  }
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
float32 Math::ACos(float32 radians) 
{
  if (-1.0f < radians)
  {
    return (1.0f > radians) ? acosf(radians) : 0;
  }

  return EGEMath::PI; 
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
void Math::GetDirection(Vector2f* vector, const Angle* angle)
{
  EGE_ASSERT(vector);
  EGE_ASSERT(angle);

  vector->x = Math::Cos(angle->radians());
  vector->y = Math::Sin(angle->radians());
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
  q.create(*vector, Angle(EGEMath::TWO_PI * l_random(0.0f, 1.0f)));
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

EGE_NAMESPACE_END