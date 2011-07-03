#include "Core/Math/Math.h"
#include <EGEMatrix.h>
#include <EGEVector.h>
#include <EGEQuaternion.h>
#include <EGEAngle.h>
#include <limits>
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const float32 Math::EPSILON = std::numeric_limits<float32>::epsilon();
const float32 Math::PI      = 3.14159265f;
const float32 Math::TWO_PI  = 6.28318531f;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Math::Math()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Math::~Math()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts quaternion to matrix representation. */
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
/*! Transforms (pre-multiples) vector by matrix. */
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
/*! Creates matrix from translation, scale vectors and quaternion. */
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
/*! Returns angle between positive Y axis and given point around origin. */
void Math::GetAngle(Angle* angle, const Vector2f* origin, const Vector2f* point)
{
  EGE_ASSERT(angle);
  EGE_ASSERT(origin);
  EGE_ASSERT(point);

  if (point->x >= origin->x)
  {
    angle->fromRadians(Math::ACos((origin->y - point->y) / origin->distanceTo(*point)));
  }
  else
  {
    angle->fromRadians(Math::ACos(-(origin->y - point->y) / origin->distanceTo(*point)) + Math::PI);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*  Performs spherical linear interpolation between given quaternions. 
  *  @param  out           Resulting quaternion.
  *  @param  from          First (start) quaternion.
  *  @param  to            Second (end) quaternion.
  *  @param  time          Scalar in range [0..1] describing relative distance between quaternion for which interpolation is to be calculated.
  *  @param  shortestPath  TRUE if shortest path (if possible) is to be used for interpolation.
  */
void Math::Slerp(Quaternionf* out, Quaternionf* from, Quaternionf* to, float32 time, bool shortestPath)
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
