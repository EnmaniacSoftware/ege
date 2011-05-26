#include "EGE.h"
#include "Core/Math/Matrix4.h"
#include "Core/Math/Vector4.h"
#include "Core/Math/Vector2.h"
#include "Core/Math/Quaternion.h"
#include "Core/Math/Math.h"
#include "Core/Math/Angle.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const float32 Math::DELTA   = std::numeric_limits<float32>::epsilon();
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
/*! Coverts quaternion to matrix representation. */
Matrix4f& Math::Convert(Matrix4f& matrix, const Quaternionf& quaternion)
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

  return matrix;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Transforms (pre-multiples) vector by matrix. */
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
/*! Creates matrix from translation, scale vectors and quaternion. */
Matrix4f& Math::CreateMatrix(Matrix4f& matrix, const Vector4f& translation, const Vector4f& scale, const Quaternionf& orientation)
{
  // Ordering:
  //    1. Scale
  //    2. Rotate
  //    3. Translate

  Matrix4f rotationMatrix;
  Matrix4f scaleMatrix(scale.x, 0, 0, 0, 0, scale.y, 0, 0, 0, 0, scale.z, 0, 0, 0, 0, scale.w);

  // convert quaternion into rotation matrix
  Math::Convert(rotationMatrix, orientation);

  // combine 1. and 2.
  matrix = rotationMatrix.multiply(scaleMatrix);

  // apply 3.
  matrix(3, 0) = translation.x;
  matrix(3, 1) = translation.y;
  matrix(3, 2) = translation.z;
  matrix(3, 3) = translation.w;

  // reset projection term
  //m_fData[ 0 ][ 3 ] = 0; 
  //m_fData[ 1 ][ 3 ] = 0; 
  //m_fData[ 2 ][ 3 ] = 0; 
  //m_fData[ 3 ][ 3 ] = 1;

  return matrix;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns Dot-Product of given vectors. */
float32 Math::DotProduct(const Vector4f& vector1, const Vector4f& vector2)
{
  return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns angle between positive Y axis and given point around origin. */
Angle Math::GetAngle(const Vector2f& origin, const Vector2f& point)
{
  Angle angle;
  
  if (point.x >= origin.x)
  {
    angle.fromRadians(Math::ACos((origin.y - point.y) / origin.distanceTo(point)));
  }
  else
  {
    angle.fromRadians(Math::ACos(-(origin.y - point.y) / origin.distanceTo(point)) + Math::PI);
  }

  return angle;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
