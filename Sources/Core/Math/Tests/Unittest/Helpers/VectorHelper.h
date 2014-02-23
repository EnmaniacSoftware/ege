#ifndef EGE_CORE_MATH_TESTS_UNITTEST_VECTORHELPER_H
#define EGE_CORE_MATH_TESTS_UNITTEST_VECTORHELPER_H

#include <EGE.h>
#include <vector>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class VectorHelper
{
  public:

    /*! Generates random 2D vector data. 
     *  @return Randomized 2D vector.
     */
    static std::vector<float32> RandomVector2Data();
    /*! Generates random 3D vector data. 
     *  @param  normalizes  If TRUE data is normalized.
     *  @return Randomized 3D vector.
     */
    static std::vector<float32> RandomVector3Data(bool normalized = false);
    /*! Generates random 4D vector data. 
     *  @return Randomized 4D vector.
     */
    static std::vector<float32> RandomVector4Data();
    /*! Compares two vector data sets. 
     *  @param  data1 Vector 1 data.
     *  @data   data2 Vector 2 data.
     *  @return Returns TRUE if both sets are the same.
     */
    static bool AreEqual(const std::vector<float32>& data1, const std::vector<float32>& data2);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_TESTS_UNITTEST_VECTORHELPER_H