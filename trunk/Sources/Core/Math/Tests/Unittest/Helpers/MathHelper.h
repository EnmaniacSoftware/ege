#ifndef EGE_CORE_MATH_TESTS_UNITTEST_MATHHELPER_H
#define EGE_CORE_MATH_TESTS_UNITTEST_MATHHELPER_H

#include <EGE.h>
#include <vector>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class MathHelper
{
  public:

    /*! Multiplies matrix and vector.
     *  @param  matrix    4x4 matrix data.
     *  @param  vector    4D vector data.
     *  @return Resulting vector.
     */
    static std::vector<float32> MultiplyVector(const std::vector<float32>& matrix, const std::vector<float32>& vector);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_TESTS_UNITTEST_MATHHELPER_H