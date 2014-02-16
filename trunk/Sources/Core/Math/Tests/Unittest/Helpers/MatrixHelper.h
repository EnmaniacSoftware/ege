#ifndef EGE_CORE_MATH_TESTS_UNITTEST_MATRIXHELPER_H
#define EGE_CORE_MATH_TESTS_UNITTEST_MATRIXHELPER_H

#include <EGE.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class MatrixHelper
{
  public:

    /*! Generates random matrix data. 
     *  @param  data  Array of data to be randomized.
     */
    static void RandomData(float32 data[16]);
    /*! Compares two matrix data sets. 
     *  @param  data1 Matrix 1 data.
     *  @data   data2 Matrix 2 data.
     *  @return Returns TRUE if both sets are the same.
     */
    static bool AreEqual(const float32 data1[16], const float32 data2[16]);
    /*! Multiplies matrices.
     *  @paeam  dataOut Resulting matrix data.
     *  @param  data1   Matrix 1 data.
     *  @param  data2   Matrix 2 data.
     */
    static void Multiply(float32 dataOut[16], const float32 data1[16], const float32 data2[16]);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_TESTS_UNITTEST_MATRIXHELPER_H