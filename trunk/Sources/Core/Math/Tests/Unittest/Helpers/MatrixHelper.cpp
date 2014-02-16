#include "Core/Math/Tests/Unittest/Helpers/MatrixHelper.h"
#include <limits>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MatrixHelper::RandomData(float32 data[16])
{
  for (int i = 0; i < 16; ++i)
  {
    data[i] = (rand() / static_cast<float32>(RAND_MAX) - 0.5f) * 2.0f;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool MatrixHelper::AreEqual(const float32 data1[16], const float32 data2[16])
{
  bool result = true;

  for (int i = 0; i < 16; ++i)
  {
    if (std::numeric_limits<float32>::epsilon() < fabs(data1[i] - data2[i]))
    {
      result = false;
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MatrixHelper::Multiply(float32 dataOut[16], const float32 data1[16], const float32 data2[16])
{
  // 1st row
  dataOut[0]  = data1[0] * data2[0]  + data1[4] * data2[1]  + data1[8] * data2[2]  + data1[12] * data2[3];
  dataOut[4]  = data1[0] * data2[4]  + data1[4] * data2[5]  + data1[8] * data2[6]  + data1[12] * data2[7];
  dataOut[8]  = data1[0] * data2[8]  + data1[4] * data2[9]  + data1[8] * data2[10] + data1[12] * data2[11];
  dataOut[12] = data1[0] * data2[12] + data1[4] * data2[13] + data1[8] * data2[14] + data1[12] * data2[15];
  
  // 2nd row
  dataOut[1]  = data1[1] * data2[0]  + data1[5] * data2[1]  + data1[9] * data2[2]  + data1[13] * data2[3];
  dataOut[5]  = data1[1] * data2[4]  + data1[5] * data2[5]  + data1[9] * data2[6]  + data1[13] * data2[7];
  dataOut[9]  = data1[1] * data2[8]  + data1[5] * data2[9]  + data1[9] * data2[10] + data1[13] * data2[11];
  dataOut[13] = data1[1] * data2[12] + data1[5] * data2[13] + data1[9] * data2[14] + data1[13] * data2[15];

  // 3rd row
  dataOut[2]  = data1[2] * data2[0]  + data1[6] * data2[1]  + data1[10] * data2[2]  + data1[14] * data2[3];
  dataOut[6]  = data1[2] * data2[4]  + data1[6] * data2[5]  + data1[10] * data2[6]  + data1[14] * data2[7];
  dataOut[10] = data1[2] * data2[8]  + data1[6] * data2[9]  + data1[10] * data2[10] + data1[14] * data2[11];
  dataOut[14] = data1[2] * data2[12] + data1[6] * data2[13] + data1[10] * data2[14] + data1[14] * data2[15];

  // 4th row
  dataOut[3]  = data1[3] * data2[0]  + data1[7] * data2[1]  + data1[11] * data2[2]  + data1[15] * data2[3];
  dataOut[7]  = data1[3] * data2[4]  + data1[7] * data2[5]  + data1[11] * data2[6]  + data1[15] * data2[7];
  dataOut[11] = data1[3] * data2[8]  + data1[7] * data2[9]  + data1[11] * data2[10] + data1[15] * data2[11];
  dataOut[15] = data1[3] * data2[12] + data1[7] * data2[13] + data1[11] * data2[14] + data1[15] * data2[15];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
