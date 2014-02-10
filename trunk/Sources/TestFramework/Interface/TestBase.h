#include <gtest/gtest.h>
#include <limits>
#include <EGE.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define EGE_EXPECT_FLOAT_EQ(expected, actual, epsilon) TestBase::ExpectFloatEqual(expected, actual, epsilon, __FILE__, __LINE__)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class TestBase : public ::testing::Test
{
  protected:

    static void SetUpTestCase();

    /*! Compares two floating point numbers with given accuracy.
     *  @param  value1    Expected value.
     *  @param  value2    Actual value.
     *  @param  epsilon   Maximum allowed difference for which values are treated as equal.
     *  @param  fileName  File name from which call to this method occured.
     *  @param  lineNo    Line number within the file at which the call has been made to this function.
     *  @note Last two parameters are used to better track where exactly the values under test came from.
     */
    void ExpectFloatEqual(float32 expected, float32 actual, float32 epsilon, const char* fileName, s32 lineNo);

  protected:

    /*! Returns random number. 
     *  @param  scale Scale of the returned value.
     *  @return Generated random number.
     *  @note Returned number is in [-scale,scale] interval.
     */
    float32 random(float32 scale = 1.0f) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
