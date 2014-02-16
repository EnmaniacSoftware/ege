#include "Core/Math/Tests/Unittest/Helpers/MathHelper.h"
#include <gtest/gtest.h>
#include <limits>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<float32> MathHelper::MultiplyVector(const std::vector<float32>& matrix, const std::vector<float32>& vector)
{
  EXPECT_EQ(4, vector.size());
  EXPECT_EQ(16, matrix.size());

  std::vector<float32> out;

  out.push_back(matrix[0] * vector[0] + matrix[4] * vector[1] + matrix[8]  * vector[2] + matrix[12] * vector[3]);
  out.push_back(matrix[1] * vector[0] + matrix[5] * vector[1] + matrix[9]  * vector[2] + matrix[13] * vector[3]);
  out.push_back(matrix[2] * vector[0] + matrix[6] * vector[1] + matrix[10] * vector[2] + matrix[14] * vector[3]);
  out.push_back(matrix[3] * vector[0] + matrix[7] * vector[1] + matrix[11] * vector[2] + matrix[15] * vector[3]);

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------