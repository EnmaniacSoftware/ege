#include <gtest/gtest.h>
#include <math.h>
#include <stdlib.h>
#include <EGEMatrix.h>

/** Tests are focusing TMatrix4<float32> instantiations. */

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const int KRepetitionsCount = 20;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Matrix4Test : public ::testing::Test
{
  protected:

    static void SetUpTestCase();
    static void TearDownTestCase();

  protected:

    virtual void SetUp();
    virtual void TearDown();

  protected:

    /*! Returns random number. 
     *  @param  scale Scale of the returned value.
     *  @return Generated random number.
     *  @note Returned number is in [-scale,scale] interval.
     */
    float32 random(float32 scale = 1.0f) const;
    /*! Generates random matrix data. 
     *  @param  data  Array of data to be randomized.
     */
    void randomData(float32 data[16]) const;
    /*! Multiplies matrices.
     *  @param  data1   Matrix 1 data.
     *  @param  data2   Matrix 2 data.
     *  @paeam  dataOut Resulting matrix data.
     */
    void multiply(const float32 data1[16], const float32 data2[16], float32 dataOut[16]) const;
    /*! Multiplies matrix and vector.
     *  @param  matrix    Matrix data.
     *  @param  vector    Vector data.
     *  @paeam  vectorOut Resulting vector data.
     */
    void multiplyVector(const float32 matrix[16], const float32 vector[4], float32 vectorOut[4]) const;
    /*! Compares two matrix data sets. 
     *  @param  data1 Matrix 1 data.
     *  @data   data2 Matrix 2 data.
     *  @return Returns TRUE if both sets are the same.
     */
    bool areEqual(const float32 data1[16], const float32 data2[16]) const;
    /*! Calculates matrix tranpose.
     *  @param  data    Matrix data.
     *  @paeam  dataOut Resulting transposed matrix data.
     */
    void transpose(const float32 data[16], float32 dataOut[16]) const;
    /*! Adds matrices.
     *  @param  data1   Matrix 1 data.
     *  @param  data2   Matrix 2 data.
     *  @paeam  dataOut Resulting matrix data.
     */
    void add(const float32 data1[16], const float32 data2[16], float32 dataOut[16]) const;
    /*! Subtructs matrices.
     *  @param  data1   Matrix 1 data.
     *  @param  data2   Matrix 2 data.
     *  @paeam  dataOut Resulting matrix data.
     */
    void subtract(const float32 data1[16], const float32 data2[16], float32 dataOut[16]) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Matrix4Test::SetUpTestCase()
{
  srand(static_cast<unsigned int>(time(NULL)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Matrix4Test::randomData(float32 data[16]) const
{
  for (int i = 0; i < 16; ++i)
  {
    data[i] = random();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Matrix4Test::multiply(const float32 data1[16], const float32 data2[16], float32 dataOut[16]) const
{
  // 1st row
  dataOut[0]  = data1[0] * data2[0]  + data1[4] * data2[1]  + data1[8] * data2[2]  + data1[12] * data2[3];
  dataOut[4]  = data1[0] * data2[4]  + data1[4] * data2[5]  + data1[8] * data2[6]  + data1[12] * data2[7];
  dataOut[8]  = data1[0] * data2[8]  + data1[4] * data2[9]  + data1[8] * data2[10] + data1[12] * data2[11];
  dataOut[12] = data1[0] * data2[12] + data1[4] * data2[13] + data1[8] * data2[14] + data1[12] * data2[15];
  
  // 2nd row
  dataOut[1]  = data1[1] * data2[0]  + data1[5] * data2[1]  + data1[9] * data2[2]  + data1[13] * data2[3];
  dataOut[5]  = data1[1] * data2[4]  + data1[5] * data2[5]  + data1[9] * data2[6]  + data1[13] * data2[7];
  dataOut[9]  = data1[1] * data2[8]  + data1[5] * data2[9]  + data1[9] * data2[10] + data1[13] * data2[11];
  dataOut[13] = data1[1] * data2[12] + data1[5] * data2[13] + data1[9] * data2[14] + data1[13] * data2[15];

  // 3rd row
  dataOut[2]  = data1[2] * data2[0]  + data1[6] * data2[1]  + data1[10] * data2[2]  + data1[14] * data2[3];
  dataOut[6]  = data1[2] * data2[4]  + data1[6] * data2[5]  + data1[10] * data2[6]  + data1[14] * data2[7];
  dataOut[10] = data1[2] * data2[8]  + data1[6] * data2[9]  + data1[10] * data2[10] + data1[14] * data2[11];
  dataOut[14] = data1[2] * data2[12] + data1[6] * data2[13] + data1[10] * data2[14] + data1[14] * data2[15];

  // 4th row
  dataOut[3]  = data1[3] * data2[0]  + data1[7] * data2[1]  + data1[11] * data2[2]  + data1[15] * data2[3];
  dataOut[7]  = data1[3] * data2[4]  + data1[7] * data2[5]  + data1[11] * data2[6]  + data1[15] * data2[7];
  dataOut[11] = data1[3] * data2[8]  + data1[7] * data2[9]  + data1[11] * data2[10] + data1[15] * data2[11];
  dataOut[15] = data1[3] * data2[12] + data1[7] * data2[13] + data1[11] * data2[14] + data1[15] * data2[15];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Matrix4Test::multiplyVector(const float32 matrix[16], const float32 vector[4], float32 vectorOut[4]) const
{
  vectorOut[0] = matrix[0] * vector[0] + matrix[4] * vector[1] + matrix[8]  * vector[2] + matrix[12] * vector[3];
  vectorOut[1] = matrix[1] * vector[0] + matrix[5] * vector[1] + matrix[9]  * vector[2] + matrix[13] * vector[3];
  vectorOut[2] = matrix[2] * vector[0] + matrix[6] * vector[1] + matrix[10] * vector[2] + matrix[14] * vector[3];
  vectorOut[3] = matrix[3] * vector[0] + matrix[7] * vector[1] + matrix[11] * vector[2] + matrix[15] * vector[3];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Matrix4Test::transpose(const float32 data[16], float32 dataOut[16]) const
{
  // 1st column
  dataOut[0] = data[0];
  dataOut[1] = data[4];
  dataOut[2] = data[8];
  dataOut[3] = data[12];

  // 2nd column
  dataOut[4] = data[1];
  dataOut[5] = data[5];
  dataOut[6] = data[9];
  dataOut[7] = data[13];

  // 3rd column
  dataOut[8]  = data[2];
  dataOut[9]  = data[6];
  dataOut[10] = data[10];
  dataOut[11] = data[14];

  // 4rd column
  dataOut[12] = data[3];
  dataOut[13] = data[7];
  dataOut[14] = data[11];
  dataOut[15] = data[15];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Matrix4Test::areEqual(const float32 data1[16], const float32 data2[16]) const
{
  bool result = true;

  for (int i = 0; i < 16; ++i)
  {
    if (std::numeric_limits<float32>::epsilon() < fabs(data1[i] - data2[i]))
    {
      result = false;
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Matrix4Test::add(const float32 data1[16], const float32 data2[16], float32 dataOut[16]) const
{
  for (int i = 0; i < 16; ++i)
  {
    dataOut[i] = data1[i] + data2[i];
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Matrix4Test::subtract(const float32 data1[16], const float32 data2[16], float32 dataOut[16]) const
{
  for (int i = 0; i < 16; ++i)
  {
    dataOut[i] = data1[i] - data2[i];
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Matrix4Test::TearDownTestCase()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Matrix4Test::SetUp()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Matrix4Test::TearDown()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 Matrix4Test::random(float32 scale) const
{
  return (rand() / static_cast<float32>(RAND_MAX) - 0.5f) * 2.0f * scale;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Matrix4Test, SetValue)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    float32 data[16];

    // randomize data
    randomData(data);

    // form up 4 columns
    const float32 column1[4] = { data[0],   data[1],  data[2],  data[3] };
    const float32 column2[4] = { data[4],   data[5],  data[6],  data[7] };
    const float32 column3[4] = { data[8],   data[9],  data[10], data[11] };
    const float32 column4[4] = { data[12],  data[13], data[14], data[15] };

    // setting via constructors
    const Matrix4f matrix1(data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10], data[11], data[12], data[13], 
                           data[14], data[15]);
    const Matrix4f matrix2(column1, column2, column3, column4);
    const Matrix4f matrix3(matrix1);

    EXPECT_TRUE(areEqual(matrix1.data, matrix2.data));
    EXPECT_TRUE(areEqual(matrix1.data, matrix3.data));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Matrix4Test, MultiplyMatrix)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    float32 data1[16];
    float32 data2[16];
    float32 dataOut[16];

    // randomize data
    randomData(data1);
    randomData(data2);

    const Matrix4f matrix1(data1[0], data1[1], data1[2], data1[3], data1[4], data1[5], data1[6], data1[7], data1[8], data1[9], data1[10], data1[11], data1[12], 
                           data1[13], data1[14], data1[15]);
    const Matrix4f matrix2(data2[0], data2[1], data2[2], data2[3], data2[4], data2[5], data2[6], data2[7], data2[8], data2[9], data2[10], data2[11], data2[12], 
                           data2[13], data2[14], data2[15]);

    // multiply...
    multiply(data1, data2, dataOut);

    // ...by operators...
    Matrix4f matrixOut = matrix1 * matrix2;
    EXPECT_TRUE(areEqual(dataOut, matrixOut.data));
    
    matrixOut = matrix1;
    matrixOut *= matrix2;
    EXPECT_TRUE(areEqual(dataOut, matrixOut.data));

    // ...and by method call
    matrixOut = matrix1.multiply(matrix2);
    EXPECT_TRUE(areEqual(dataOut, matrixOut.data));

    // product of matrices is not cummutative (AB != BA)
    matrixOut = matrix2 * matrix1;
    EXPECT_FALSE(areEqual(dataOut, matrixOut.data));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Matrix4Test, Transpose)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    float32 data[16];
    float32 dataOut[16];

    // randomize data
    randomData(data);

    const Matrix4f matrix(data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10], data[11], data[12], data[13], 
                          data[14], data[15]);

    // transpose
    transpose(data, dataOut);

    const Matrix4f transposed = matrix.transposed();

    EXPECT_TRUE(areEqual(dataOut, transposed.data));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Matrix4Test, IsAffine)
{
  float32 data[16];

  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    // randomize data
    randomData(data);

    // check if generated data is affine
    bool isAffine = (0 == data[3]) && (0 == data[7]) && (0 == data[11]) && (1 == data[15]);

    const Matrix4f matrix(data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10], data[11], data[12], data[13], 
                          data[14], data[15]);

    EXPECT_EQ(isAffine, matrix.isAffine());
  }

  // randomize data
  randomData(data);

  // make it affine
  data[3]  = 0;
  data[7]  = 0;
  data[11] = 0;
  data[15] = 1;

  const Matrix4f matrix(data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10], data[11], data[12], data[13], 
                        data[14], data[15]);

  EXPECT_TRUE(matrix.isAffine());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Matrix4Test, SetScale)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    float32 data[16];

    // randomize data
    randomData(data);

    Matrix4f matrix(data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10], data[11], data[12], data[13], 
                    data[14], data[15]);
    
    // generate new scale factors
    const float32 scaleX = random();
    const float32 scaleY = random();
    const float32 scaleZ = random();

    // set new scale
    matrix.setScale(scaleX, scaleY, scaleZ);

    // verify
    EXPECT_FLOAT_EQ(scaleX, matrix.data[0]);
    EXPECT_FLOAT_EQ(scaleY, matrix.data[5]);
    EXPECT_FLOAT_EQ(scaleZ, matrix.data[10]);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Matrix4Test, SetTranslation)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    float32 data[16];

    // randomize data
    randomData(data);

    Matrix4f matrix(data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10], data[11], data[12], data[13], 
                    data[14], data[15]);
    
    // generate new translation values
    const float32 translateX = random();
    const float32 translateY = random();
    const float32 translateZ = random();

    // set new translation
    matrix.setTranslation(translateX, translateY, translateZ);

    // verify
    EXPECT_FLOAT_EQ(translateX, matrix.data[12]);
    EXPECT_FLOAT_EQ(translateY, matrix.data[13]);
    EXPECT_FLOAT_EQ(translateZ, matrix.data[14]);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Matrix4Test, Translation)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    float32 data[16];

    // randomize data
    randomData(data);

    Matrix4f matrix(data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10], data[11], data[12], data[13], 
                    data[14], data[15]);
    
    // get current translation values
    const Vector4f translation = matrix.translation();

    // verify
    EXPECT_FLOAT_EQ(translation.x, matrix.data[12]);
    EXPECT_FLOAT_EQ(translation.y, matrix.data[13]);
    EXPECT_FLOAT_EQ(translation.z, matrix.data[14]);
    EXPECT_FLOAT_EQ(translation.w, matrix.data[15]);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Matrix4Test, ¥rrayIndexing)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    float32 data[16];

    // randomize data
    randomData(data);
    
    // verify
    const Matrix4f matrix1(data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10], data[11], data[12], data[13], 
                           data[14], data[15]);

    EXPECT_FLOAT_EQ(matrix1[0][0], data[0]);
    EXPECT_FLOAT_EQ(matrix1[0][1], data[1]);
    EXPECT_FLOAT_EQ(matrix1[0][2], data[2]);
    EXPECT_FLOAT_EQ(matrix1[0][3], data[3]);

    EXPECT_FLOAT_EQ(matrix1[1][0], data[4]);
    EXPECT_FLOAT_EQ(matrix1[1][1], data[5]);
    EXPECT_FLOAT_EQ(matrix1[1][2], data[6]);
    EXPECT_FLOAT_EQ(matrix1[1][3], data[7]);

    EXPECT_FLOAT_EQ(matrix1[2][0], data[8]);
    EXPECT_FLOAT_EQ(matrix1[2][1], data[9]);
    EXPECT_FLOAT_EQ(matrix1[2][2], data[10]);
    EXPECT_FLOAT_EQ(matrix1[2][3], data[11]);

    EXPECT_FLOAT_EQ(matrix1[3][0], data[12]);
    EXPECT_FLOAT_EQ(matrix1[3][1], data[13]);
    EXPECT_FLOAT_EQ(matrix1[3][2], data[14]);
    EXPECT_FLOAT_EQ(matrix1[3][3], data[15]);

    Matrix4f matrix2(data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10], data[11], data[12], data[13], 
                     data[14], data[15]);

    EXPECT_FLOAT_EQ(matrix2[0][0], data[0]);
    EXPECT_FLOAT_EQ(matrix2[0][1], data[1]);
    EXPECT_FLOAT_EQ(matrix2[0][2], data[2]);
    EXPECT_FLOAT_EQ(matrix2[0][3], data[3]);

    EXPECT_FLOAT_EQ(matrix2[1][0], data[4]);
    EXPECT_FLOAT_EQ(matrix2[1][1], data[5]);
    EXPECT_FLOAT_EQ(matrix2[1][2], data[6]);
    EXPECT_FLOAT_EQ(matrix2[1][3], data[7]);

    EXPECT_FLOAT_EQ(matrix2[2][0], data[8]);
    EXPECT_FLOAT_EQ(matrix2[2][1], data[9]);
    EXPECT_FLOAT_EQ(matrix2[2][2], data[10]);
    EXPECT_FLOAT_EQ(matrix2[2][3], data[11]);

    EXPECT_FLOAT_EQ(matrix2[3][0], data[12]);
    EXPECT_FLOAT_EQ(matrix2[3][1], data[13]);
    EXPECT_FLOAT_EQ(matrix2[3][2], data[14]);
    EXPECT_FLOAT_EQ(matrix2[3][3], data[15]);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Matrix4Test, Addition)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    float32 data1[16];
    float32 data2[16];
    float32 dataOut[16];

    // randomize data
    randomData(data1);
    randomData(data2);

    Matrix4f matrix1(data1[0], data1[1], data1[2], data1[3], data1[4], data1[5], data1[6], data1[7], data1[8], data1[9], data1[10], data1[11], data1[12], 
                     data1[13], data1[14], data1[15]);
    Matrix4f matrix2(data2[0], data2[1], data2[2], data2[3], data2[4], data2[5], data2[6], data2[7], data2[8], data2[9], data2[10], data2[11], data2[12], 
                     data2[13], data2[14], data2[15]);

    // add using reference method
    add(data1, data2, dataOut);

    // add M1 and M2
    Matrix4f matrix = matrix1;
    matrix += matrix2;
    EXPECT_TRUE(areEqual(dataOut, matrix.data));

    matrix = matrix1 + matrix2;
    EXPECT_TRUE(areEqual(dataOut, matrix.data));

    // add M2 and M1
    matrix = matrix2;
    matrix += matrix1;
    EXPECT_TRUE(areEqual(dataOut, matrix.data));

    matrix = matrix2 + matrix1;
    EXPECT_TRUE(areEqual(dataOut, matrix.data));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Matrix4Test, Difference)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    float32 data1[16];
    float32 data2[16];
    float32 dataOut[16];

    // randomize data
    randomData(data1);
    randomData(data2);

    Matrix4f matrix1(data1[0], data1[1], data1[2], data1[3], data1[4], data1[5], data1[6], data1[7], data1[8], data1[9], data1[10], data1[11], data1[12], 
                     data1[13], data1[14], data1[15]);
    Matrix4f matrix2(data2[0], data2[1], data2[2], data2[3], data2[4], data2[5], data2[6], data2[7], data2[8], data2[9], data2[10], data2[11], data2[12], 
                     data2[13], data2[14], data2[15]);

    // subtract using reference method
    subtract(data1, data2, dataOut);

    Matrix4f matrix = matrix1;
    matrix -= matrix2;
    EXPECT_TRUE(areEqual(dataOut, matrix.data));

    matrix = matrix1 - matrix2;
    EXPECT_TRUE(areEqual(dataOut, matrix.data));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Matrix4Test, VectorMultiplication)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    float32 data[16];
    float32 vector[4];
    float32 vectorOut[4];

    // randomize data
    randomData(data);
    
    vector[0] = random();
    vector[1] = random();
    vector[2] = random();
    vector[3] = random();

    Vector4f vec(vector[0], vector[1], vector[2], vector[3]);
    const Matrix4f matrix(data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10], data[11], data[12], data[13], 
                          data[14], data[15]);

    // multiply 
    multiplyVector(data, vector, vectorOut);

    Vector4f vecOut = matrix * vec;

    // verify
    EXPECT_FLOAT_EQ(vectorOut[0], vecOut.x);
    EXPECT_FLOAT_EQ(vectorOut[1], vecOut.y);
    EXPECT_FLOAT_EQ(vectorOut[2], vecOut.z);
    EXPECT_FLOAT_EQ(vectorOut[3], vecOut.w);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
