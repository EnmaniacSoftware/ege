#include "Core/Math/Tests/Unittest/Helpers/GeneratorHelper.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<float32> GeneratorHelper::RandomData(s32 elements)
{
  std::vector<float32> out;

  for (s32 i = 0; i < elements; ++i)
  {
    out.push_back((rand() / static_cast<float32>(RAND_MAX) - 0.5f) * 2.0f);
  }

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool GeneratorHelper::AreEqual(const std::vector<float32>& data1, const std::vector<float32>& data2)
{
  bool result = true;

  if (data1.size() != data2.size())
  {
    result = false;
  }
  else
  {
    for (size_t i = 0; i < data1.size(); ++i)
    {
      if (std::numeric_limits<float32>::epsilon() < fabs(data1[i] - data2[i]))
      {
        result = false;
      }
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------