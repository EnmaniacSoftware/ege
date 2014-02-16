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