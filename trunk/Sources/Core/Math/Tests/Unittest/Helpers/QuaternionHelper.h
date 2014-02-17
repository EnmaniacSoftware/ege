#ifndef EGE_CORE_MATH_TESTS_UNITTEST_QUATERNIONHELPER_H
#define EGE_CORE_MATH_TESTS_UNITTEST_QUATERNIONHELPER_H

#include <EGE.h>
#include <vector>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class QuaternionHelper
{
  public:

    /*! Generates random quaternion data. 
     *  @return Randomize quaternion data.
     */
    static std::vector<float32> RandomData();
    /*! Compares two quaternion data sets. 
     *  @param  data1 Quaternion 1 data.
     *  @data   data2 Quaternion 2 data.
     *  @return Returns TRUE if both sets are the same.
     */
    static bool AreEqual(const std::vector<float32>& data1, const std::vector<float32>& data2);
    /*! Returns identity quaternion. */
    static std::vector<float32> Identity();
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_TESTS_UNITTEST_QUATERNIONHELPER_H