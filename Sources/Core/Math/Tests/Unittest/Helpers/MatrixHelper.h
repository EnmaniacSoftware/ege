#ifndef EGE_CORE_MATH_TESTS_UNITTEST_MATRIXHELPER_H
#define EGE_CORE_MATH_TESTS_UNITTEST_MATRIXHELPER_H

#include <EGE.h>
#include <vector>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class MatrixHelper
{
  public:

    /*! Generates random 4x4 matrix data. 
     *  @return Randomized 4x4 matrix.
     */
    static std::vector<float32> RandomMatrix4();
    /*! Compares two matrix data sets. 
     *  @param  data1 Matrix 1 data.
     *  @data   data2 Matrix 2 data.
     *  @return Returns TRUE if both sets are the same.
     */
    static bool AreEqual(const std::vector<float32>& data1, const std::vector<float32>& data2);
    static bool AreEqual(const std::vector<float32>& data1, const float32 data2[16]);
    static bool AreEqual(const float32 data1[16], const float32 data2[16]);
    /*! Multiplies matrices.
     *  @param  data1 Matrix 1 data.
     *  @param  data2 Matrix 2 data.
     *  @returns Resulting matrix data.
     */
    static std::vector<float32> Multiply(const std::vector<float32>& data1, const std::vector<float32>& data2);
    /*! Returns 4x4 identity matrix. */
    static std::vector<float32> Identity();
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_TESTS_UNITTEST_MATRIXHELPER_H