#include "Core/Math/Math.h"
#include <EGEMatrix.h>
#include <EGEVector.h>
#include <EGEQuaternion.h>
#include <EGEAngle.h>
#include <EGEComplex.h>
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
/*! Returns angle between positive X axis and given point around origin. */
void Math::GetAngle(Angle* angle, const Vector2f* origin, const Vector2f* point)
{
  EGE_ASSERT(angle);
  EGE_ASSERT(origin);
  EGE_ASSERT(point);
  EGE_ASSERT(!(origin->x == point->x && origin->y == point->y));

  // NOTE: arguments are in (Y, X) order.
  //       we assume X axis grows from LEFT to RIGHT
  //       we assume T axis grows from BOTTOM to TOP
  angle->fromRadians(Math::ATan2(origin->y - point->y, point->x - origin->x));

  //if (point->x >= origin->x)
  //{
  //  angle->fromRadians(Math::ACos((origin->y - point->y) / origin->distanceTo(*point)));
  //}
  //else
  //{
  //  angle->fromRadians(Math::ACos(-(origin->y - point->y) / origin->distanceTo(*point)) + Math::PI);
  //}
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
/*  Performs spherical linear interpolation between given complex numbers. 
  *  @param  out           Resulting complex number.
  *  @param  from          First (start) complex number.
  *  @param  to            Second (end) complex number.
  *  @param  time          Scalar in range [0..1] describing relative distance between numbers for which interpolation is to be calculated.
  */
void Math::Slerp(Complexf* out, Complexf* from, Complexf* to, float32 time)
{
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
