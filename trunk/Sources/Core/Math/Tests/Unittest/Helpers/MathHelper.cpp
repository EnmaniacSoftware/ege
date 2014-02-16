#include "Core/Math/Tests/Unittest/Helpers/MathHelper.h"
#include <limits>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MathHelper::MultiplyVector(const float32 matrix[16], const float32 vector[4], float32 vectorOut[4])
{
  vectorOut[0] = matrix[0] * vector[0] + matrix[4] * vector[1] + matrix[8]  * vector[2] + matrix[12] * vector[3];
  vectorOut[1] = matrix[1] * vector[0] + matrix[5] * vector[1] + matrix[9]  * vector[2] + matrix[13] * vector[3];
  vectorOut[2] = matrix[2] * vector[0] + matrix[6] * vector[1] + matrix[10] * vector[2] + matrix[14] * vector[3];
  vectorOut[3] = matrix[3] * vector[0] + matrix[7] * vector[1] + matrix[11] * vector[2] + matrix[15] * vector[3];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------