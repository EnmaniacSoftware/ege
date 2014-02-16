#include "Core/Math/Tests/Unittest/Helpers/QuaternionHelper.h"
#include <limits>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void QuaternionHelper::RandomData(float32 data[4])
{
  for (int i = 0; i < 4; ++i)
  {
    data[i] = (rand() / static_cast<float32>(RAND_MAX) - 0.5f) * 2.0f;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool QuaternionHelper::AreEqual(const float32 data1[4], const float32 data2[4])
{
  bool result = true;

  for (int i = 0; i < 4; ++i)
  {
    if (std::numeric_limits<float32>::epsilon() < fabs(data1[i] - data2[i]))
    {
      result = false;
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------