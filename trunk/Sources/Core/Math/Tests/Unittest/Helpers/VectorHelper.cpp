#include "Core/Math/Tests/Unittest/Helpers/VectorHelper.h"
#include "Core/Math/Tests/Unittest/Helpers/GeneratorHelper.h"
#include <math.h>
#include <limits>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<float32> VectorHelper::RandomVector2Data()
{
  return GeneratorHelper::RandomData(2);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<float32> VectorHelper::RandomVector3Data(bool normalized)
{
  std::vector<float32> data = GeneratorHelper::RandomData(3);
  
  if (normalized)
  {
    const float32 length = sqrtf((data[0] * data[0]) + (data[1] * data[1]) + (data[2] * data[2]));

    if (std::numeric_limits<float32>::epsilon() <= length)
    {
      // normalize
	    data[0] /= length;
	    data[1] /= length;
      data[2] /= length;
    }
  }

  return data;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<float32> VectorHelper::RandomVector4Data()
{
  return GeneratorHelper::RandomData(4);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool VectorHelper::AreEqual(const std::vector<float32>& data1, const std::vector<float32>& data2)
{
  bool result = GeneratorHelper::AreEqual(data1, data2);

  if (result)
  {
    // check if not valid data for 2D or 4D vector
    if (((2 != data1.size()) && (4 != data1.size())) || ((2 != data2.size()) && (4 != data2.size())))
    {
      result = false;
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------