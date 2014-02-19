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