#ifndef EGE_CORE_MATH_TESTS_UNITTEST_MATHHELPER_H
#define EGE_CORE_MATH_TESTS_UNITTEST_MATHHELPER_H

#include <EGE.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class MathHelper
{
  public:

    /*! Multiplies matrix and vector.
     *  @param  matrix    Matrix data.
     *  @param  vector    Vector data.
     *  @paeam  vectorOut Resulting vector data.
     */
    static void MultiplyVector(const float32 matrix[16], const float32 vector[4], float32 vectorOut[4]);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_TESTS_UNITTEST_MATHHELPER_H