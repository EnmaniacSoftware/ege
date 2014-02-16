#include "Core/Math/Tests/Unittest/Helpers/MatrixHelper.h"
#include "Core/Math/Tests/Unittest/Helpers/GeneratorHelper.h"
#include <limits>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<float32> MatrixHelper::RandomMatrix4()
{
  return GeneratorHelper::RandomData(16);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool MatrixHelper::AreEqual(const std::vector<float32>& data1, const std::vector<float32>& data2)
{
  bool result = GeneratorHelper::AreEqual(data1, data2);

  if (result)
  {
    if ((16 != data1.size()) || (16 != data2.size()))
    {
      result = false;
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool MatrixHelper::AreEqual(const std::vector<float32>& data1, const float32 data2[16])
{
  std::vector<float32> data2vec;

  for (s32 i = 0; i < 16; ++i)
  {
    data2vec.push_back(data2[i]);
  }

  return AreEqual(data1, data2vec);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool MatrixHelper::AreEqual(const float32 data1[16], const float32 data2[16])
{
  std::vector<float32> data1vec;
  std::vector<float32> data2vec;

  for (s32 i = 0; i < 16; ++i)
  {
    data1vec.push_back(data1[i]);
    data2vec.push_back(data2[i]);
  }

  return AreEqual(data1vec, data2vec);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<float32> MatrixHelper::Multiply(const std::vector<float32>& data1, const std::vector<float32>& data2)
{
  std::vector<float32> out(16, 0);

  // 1st row
  out[0]  = data1[0] * data2[0]  + data1[4] * data2[1]  + data1[8] * data2[2]  + data1[12] * data2[3];
  out[4]  = data1[0] * data2[4]  + data1[4] * data2[5]  + data1[8] * data2[6]  + data1[12] * data2[7];
  out[8]  = data1[0] * data2[8]  + data1[4] * data2[9]  + data1[8] * data2[10] + data1[12] * data2[11];
  out[12] = data1[0] * data2[12] + data1[4] * data2[13] + data1[8] * data2[14] + data1[12] * data2[15];
  
  // 2nd row
  out[1]  = data1[1] * data2[0]  + data1[5] * data2[1]  + data1[9] * data2[2]  + data1[13] * data2[3];
  out[5]  = data1[1] * data2[4]  + data1[5] * data2[5]  + data1[9] * data2[6]  + data1[13] * data2[7];
  out[9]  = data1[1] * data2[8]  + data1[5] * data2[9]  + data1[9] * data2[10] + data1[13] * data2[11];
  out[13] = data1[1] * data2[12] + data1[5] * data2[13] + data1[9] * data2[14] + data1[13] * data2[15];

  // 3rd row
  out[2]  = data1[2] * data2[0]  + data1[6] * data2[1]  + data1[10] * data2[2]  + data1[14] * data2[3];
  out[6]  = data1[2] * data2[4]  + data1[6] * data2[5]  + data1[10] * data2[6]  + data1[14] * data2[7];
  out[10] = data1[2] * data2[8]  + data1[6] * data2[9]  + data1[10] * data2[10] + data1[14] * data2[11];
  out[14] = data1[2] * data2[12] + data1[6] * data2[13] + data1[10] * data2[14] + data1[14] * data2[15];

  // 4th row
  out[3]  = data1[3] * data2[0]  + data1[7] * data2[1]  + data1[11] * data2[2]  + data1[15] * data2[3];
  out[7]  = data1[3] * data2[4]  + data1[7] * data2[5]  + data1[11] * data2[6]  + data1[15] * data2[7];
  out[11] = data1[3] * data2[8]  + data1[7] * data2[9]  + data1[11] * data2[10] + data1[15] * data2[11];
  out[15] = data1[3] * data2[12] + data1[7] * data2[13] + data1[11] * data2[14] + data1[15] * data2[15];

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<float32> MatrixHelper::Identity()
{
  std::vector<float32> out(16, 0);

  out[0]  = 1;
  out[5]  = 1;
  out[10] = 1;
  out[15] = 1;

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
