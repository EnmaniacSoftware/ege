#include "Core/Math/Tests/Unittest/Helpers/QuaternionHelper.h"
#include "Core/Math/Tests/Unittest/Helpers/GeneratorHelper.h"
#include <limits>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<float32> QuaternionHelper::RandomData()
{
  return GeneratorHelper::RandomData(4);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool QuaternionHelper::AreEqual(const std::vector<float32>& data1, const std::vector<float32>& data2)
{
  bool result = GeneratorHelper::AreEqual(data1, data2);

  if (result)
  {
    if ((4 != data1.size()) || (4 != data2.size()))
    {
      result = false;
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<float32> QuaternionHelper::Identity()
{
  std::vector<float32> out;
  
  out.push_back(0);
  out.push_back(0);
  out.push_back(0);
  out.push_back(1);

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
