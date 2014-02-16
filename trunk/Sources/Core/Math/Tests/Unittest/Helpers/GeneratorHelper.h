#ifndef EGE_CORE_MATH_TESTS_UNITTEST_HELPERS_GENERATORHELPER_H
#define EGE_CORE_MATH_TESTS_UNITTEST_HELPERS_GENERATORHELPER_H

#include <EGE.h>
#include <vector>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class GeneratorHelper
{
  public:

    /*! Generates random data. 
     *  @param  elements  Number of elements to randomly generate.
     *  @return Randomized data of a given size.
     */
    static std::vector<float32> RandomData(s32 elements);
    /*! Compares two data sets. 
     *  @param  data1 Data set 1.
     *  @data   data2 Data set 2.
     *  @return Returns TRUE if both sets are the same.
     */
    static bool AreEqual(const std::vector<float32>& data1, const std::vector<float32>& data2);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_TESTS_UNITTEST_HELPERS_GENERATORHELPER_H