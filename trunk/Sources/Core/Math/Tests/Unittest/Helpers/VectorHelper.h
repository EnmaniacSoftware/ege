#ifndef EGE_CORE_MATH_TESTS_UNITTEST_VECTORHELPER_H
#define EGE_CORE_MATH_TESTS_UNITTEST_VECTORHELPER_H

#include <EGE.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class VectorHelper
{
  public:

    /*! Generates random 4D vector data. 
     *  @param  data  Array of data to be randomized.
     */
    static void RandomData(float32 data[4]);
    /*! Compares two vector data sets. 
     *  @param  data1 Vector 1 data.
     *  @data   data2 Vector 2 data.
     *  @return Returns TRUE if both sets are the same.
     */
    static bool AreEqual(const float32 data1[4], const float32 data2[4]);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_TESTS_UNITTEST_VECTORHELPER_H