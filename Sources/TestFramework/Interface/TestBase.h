#ifndef EGE_CORE_TESTFRAMEWORK_TESTBASE_H
#define EGE_CORE_TESTFRAMEWORK_TESTBASE_H

#include <EGE.h>
#include <gtest/gtest.h>
#include <limits>
#include <vector>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define EGE_EXPECT_FLOAT_EQ(expected, actual, epsilon) TestBase::ExpectFloatEqual(expected, actual, epsilon, __FILE__, __LINE__)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class TestBase : public ::testing::Test
{
  protected:

    /*! Assertion hit info struct. */
    struct AssertHitInfo
    {
      const char* fileName;
      const char* reason;
      s32 lineNo;
    };

  protected:

    TestBase(float32 epsilon = std::numeric_limits<float32>::epsilon());
    virtual ~TestBase();

    virtual void SetUp();
    virtual void TearDown();

    /*! Returns random number. 
     *  @param  scale Scale of the returned value.
     *  @return Generated random number.
     *  @note Returned number is in [-scale,scale] interval.
     */
    float32 random(float32 scale = 1.0f) const;
    /*! Returns random positive number. 
     *  @param  scale Scale of the returned value.
     *  @return Generated random number.
     *  @note Returned number is in [0.0001f,scale] interval.
     */
    float32 randomPositive(float32 scale = 1.0f) const;
    /*! Returns random rangle between [0-2PI]. 
     *  @return Generated random angle.
     */
    float32 randomAngle() const;
    /*! Returns random integer value. 
     *  @return Randomly generated integer value.
     */
    s32 randomInteger() const;

    /*! Returns floating point epsilon value. */
    float32 epsilon() const;

    /*! Compares two floating point numbers with given accuracy.
     *  @param  value1    Expected value.
     *  @param  value2    Actual value.
     *  @param  epsilon   Maximum allowed difference for which values are treated as equal.
     *  @param  fileName  File name from which call to this method occured.
     *  @param  lineNo    Line number within the file at which the call has been made to this function.
     *  @note Last two parameters are used to better track where exactly the values under test came from.
     */
    void ExpectFloatEqual(float32 expected, float32 actual, float32 epsilon, const char* fileName, s32 lineNo);
    
    /*! Returns assert hits. */
    const std::vector<AssertHitInfo>& assertHitInfo() const;

  private:

    /*! Custom simple assertion handler. */
    static void HandleAssert(const char* fileName, s32 lineNo);
    /*! Custom described assertion handler. */
    static void HandleAssertX(const char* reason, const char* fileName, s32 lineNo);

  private:

    /*! List of triggered assertions. */
    static std::vector<AssertHitInfo> m_hits;

  private:

    /*! Current epsilon value for floating point comparisons. */
    float32 m_epsilon;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_CORE_TESTFRAMEWORK_TESTBASE_H