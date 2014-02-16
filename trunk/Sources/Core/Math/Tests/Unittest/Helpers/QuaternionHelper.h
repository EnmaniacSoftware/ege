#ifndef EGE_CORE_MATH_TESTS_UNITTEST_QUATERNIONHELPER_H
#define EGE_CORE_MATH_TESTS_UNITTEST_QUATERNIONHELPER_H

#include <EGE.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class QuaternionHelper
{
  public:

    /*! Generates random quaternion data. 
     *  @param  data  Array of data to be randomized.
     */
    static void RandomData(float32 data[4]);
    /*! Compares two quaternion data sets. 
     *  @param  data1 Quaternion 1 data.
     *  @data   data2 Quaternion 2 data.
     *  @return Returns TRUE if both sets are the same.
     */
    static bool AreEqual(const float32 data1[4], const float32 data2[4]);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_TESTS_UNITTEST_QUATERNIONHELPER_H