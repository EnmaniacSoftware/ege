#include "EGE.h"
#include "Core/Math/Matrix4.h"
#include "Core/Math/Vector4.h"
#include "Core/Math/Quaternion.h"
#include "Core/Math/Math.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

const float32 Math::DELTA = std::numeric_limits<float32>::epsilon();

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

Math::Math()
{
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

Math::~Math()
{
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

Matrix4f& Math::Convert(Matrix4f& cMatrix, const Quaternionf& cQuaternion)
{
  // NOTE: this converts quternion to rotation matrix

  // 1st column
	cMatrix.data[0] = 1.0f - 2.0f * (cQuaternion.y * cQuaternion.y + cQuaternion.z * cQuaternion.z); 
	cMatrix.data[1] = 2.0f * (cQuaternion.x * cQuaternion.y - cQuaternion.z * cQuaternion.w);
	cMatrix.data[2] = 2.0f * (cQuaternion.x * cQuaternion.z + cQuaternion.y * cQuaternion.w);
	cMatrix.data[3] = 0;

	// 2nd column
	cMatrix.data[4] = 2.0f * (cQuaternion.x * cQuaternion.y + cQuaternion.z * cQuaternion.w);  
	cMatrix.data[5] = 1.0f - 2.0f * (cQuaternion.x * cQuaternion.x + cQuaternion.z * cQuaternion.z); 
	cMatrix.data[6] = 2.0f * (cQuaternion.z * cQuaternion.y - cQuaternion.x * cQuaternion.w);  
	cMatrix.data[7] = 0;

	// 3rd column
	cMatrix.data[8]  = 2.0f * (cQuaternion.x * cQuaternion.z - cQuaternion.y * cQuaternion.w);
	cMatrix.data[9]  = 2.0f * (cQuaternion.y * cQuaternion.z + cQuaternion.x * cQuaternion.w);
	cMatrix.data[10] = 1.0f - 2.0f *(cQuaternion.x * cQuaternion.x + cQuaternion.y * cQuaternion.y);  
	cMatrix.data[11] = 0;

	// 4th column
	cMatrix.data[12] = 0;
	cMatrix.data[13] = 0;
	cMatrix.data[14] = 0;  
	cMatrix.data[15] = 1;

  return cMatrix;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

Vector4f& Math::Transform(Vector4f& cVector, const Matrix4f& cMatrix)
{
  Vector4f cOriginal(cVector);

  // NOTE: this pre-multiples vector and matrix
  cVector.x = cMatrix.data[0] * cOriginal.x + cMatrix.data[4] * cOriginal.y + cMatrix.data[8] * cOriginal.z + 
              cMatrix.data[12] * cOriginal.w;
  cVector.y = cMatrix.data[1] * cOriginal.x + cMatrix.data[5] * cOriginal.y + cMatrix.data[9] * cOriginal.z + 
              cMatrix.data[13] * cOriginal.w;
  cVector.z = cMatrix.data[2] * cOriginal.x + cMatrix.data[6] * cOriginal.y + cMatrix.data[10] * cOriginal.z + 
              cMatrix.data[14] * cOriginal.w;
  cVector.w = cMatrix.data[3] * cOriginal.x + cMatrix.data[7] * cOriginal.y + cMatrix.data[11] * cOriginal.z + 
              cMatrix.data[15] * cOriginal.w;

  return cVector;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

Matrix4f& Math::CreateMatrix(Matrix4f& cMatrix, const Vector4f& cTranslation, const Vector4f& cScaling, 
                               const Quaternionf& cOrientation)
{
  // Ordering:
  //    1. Scale
  //    2. Rotate
  //    3. Translate

  Matrix4f cRotationMatrix;
  Matrix4f cScaleMatrix(cScaling.x, 0, 0, 0, 0, cScaling.y, 0, 0, 0, 0, cScaling.z, 0, 0, 0, 0, cScaling.w);

  // convert quaternion into rotation matrix
  Math::Convert(cRotationMatrix, cOrientation);

  // combine 1. and 2.
  cMatrix = cRotationMatrix.multiply(cScaleMatrix);

  // apply 3.
  cMatrix(3, 0) = cTranslation.x;
  cMatrix(3, 1) = cTranslation.y;
  cMatrix(3, 2) = cTranslation.z;
  cMatrix(3, 3) = cTranslation.w;

  // reset projection term
  //m_fData[ 0 ][ 3 ] = 0; 
  //m_fData[ 1 ][ 3 ] = 0; 
  //m_fData[ 2 ][ 3 ] = 0; 
  //m_fData[ 3 ][ 3 ] = 1;

  return cMatrix;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

float32 Math::DotProduct(const Vector4f& cVector1, const Vector4f& cVector2)
{
  return cVector1.x * cVector2.x + cVector1.y * cVector2.y + cVector1.z * cVector2.z;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
