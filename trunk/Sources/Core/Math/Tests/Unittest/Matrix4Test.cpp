#include "TestFramework/Interface/TestBase.h"
#include "Core/Math/Tests/Unittest/Helpers/MatrixHelper.h"
#include "Core/Math/Tests/Unittest/Helpers/VectorHelper.h"
#include "Core/Math/Tests/Unittest/Helpers/MathHelper.h"
#include <EGEMatrix.h>

/** Tests are focusing TMatrix4<float32> instantiations. */

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const int KRepetitionsCount = 20;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Matrix4Test : public TestBase
{
  protected:

    /*! Calculates matrix tranpose.
     *  @param  data    Matrix data.
     *  @return Resulting transposed matrix data.
     */
    std::vector<float32> transpose(const std::vector<float32>& data) const;
    /*! Adds matrices.
     *  @param  data1   Matrix 1 data.
     *  @param  data2   Matrix 2 data.
     *  @return Resulting matrix data.
     */
    std::vector<float32> add(const std::vector<float32>& data1, const std::vector<float32>& data2) const;
    /*! Subtructs matrices.
     *  @param  data1   Matrix 1 data.
     *  @param  data2   Matrix 2 data.
     *  @return Resulting matrix data.
     */
    std::vector<float32> subtract(const std::vector<float32>& data1, const std::vector<float32>& data2) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<float32> Matrix4Test::transpose(const std::vector<float32>& data) const
{
  std::vector<float32> dataOut(16, 0);

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

  return dataOut;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<float32> Matrix4Test::add(const std::vector<float32>& data1, const std::vector<float32>& data2) const
{
  std::vector<float32> dataOut;

  for (int i = 0; i < 16; ++i)
  {
    dataOut.push_back(data1[i] + data2[i]);
  }

  return dataOut;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<float32> Matrix4Test::subtract(const std::vector<float32>& data1, const std::vector<float32>& data2) const
{
  std::vector<float32> dataOut;

  for (int i = 0; i < 16; ++i)
  {
     dataOut.push_back(data1[i] - data2[i]);
  }

  return dataOut;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Matrix4Test, SetValue)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    // randomize data
    std::vector<float32> data = MatrixHelper::RandomMatrix4();

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

    EXPECT_TRUE(MatrixHelper::AreEqual(matrix1.data, matrix2.data));
    EXPECT_TRUE(MatrixHelper::AreEqual(matrix1.data, matrix3.data));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Matrix4Test, MultiplyMatrix)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    std::vector<float32> data1;
    std::vector<float32> data2;
    std::vector<float32> dataOut;

    // randomize data
    data1 = MatrixHelper::RandomMatrix4();
    data2 = MatrixHelper::RandomMatrix4();

    const Matrix4f matrix1(data1[0], data1[1], data1[2], data1[3], data1[4], data1[5], data1[6], data1[7], data1[8], data1[9], data1[10], data1[11], data1[12], 
                           data1[13], data1[14], data1[15]);
    const Matrix4f matrix2(data2[0], data2[1], data2[2], data2[3], data2[4], data2[5], data2[6], data2[7], data2[8], data2[9], data2[10], data2[11], data2[12], 
                           data2[13], data2[14], data2[15]);

    // multiply...
    dataOut = MatrixHelper::Multiply(data1, data2);

    // ...by operators...
    Matrix4f matrixOut = matrix1 * matrix2;
    EXPECT_TRUE(MatrixHelper::AreEqual(dataOut, matrixOut.data));
    
    matrixOut = matrix1;
    matrixOut *= matrix2;
    EXPECT_TRUE(MatrixHelper::AreEqual(dataOut, matrixOut.data));

    // ...and by method call
    matrixOut = matrix1.multiply(matrix2);
    EXPECT_TRUE(MatrixHelper::AreEqual(dataOut, matrixOut.data));

    // product of matrices is not cummutative (AB != BA)
    matrixOut = matrix2 * matrix1;
    EXPECT_FALSE(MatrixHelper::AreEqual(dataOut, matrixOut.data));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Matrix4Test, Transpose)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    std::vector<float32> data;
    std::vector<float32> dataOut;

    // randomize data
    data = MatrixHelper::RandomMatrix4();

    const Matrix4f matrix(data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10], data[11], data[12], data[13], 
                          data[14], data[15]);

    // transpose
    dataOut = transpose(data);

    const Matrix4f transposed = matrix.transposed();

    EXPECT_TRUE(MatrixHelper::AreEqual(dataOut, transposed.data));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Matrix4Test, IsAffine)
{
  std::vector<float32> data;

  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    // randomize data
    data = MatrixHelper::RandomMatrix4();

    // check if generated data is affine
    bool isAffine = (0 == data[3]) && (0 == data[7]) && (0 == data[11]) && (1 == data[15]);

    const Matrix4f matrix(data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10], data[11], data[12], data[13], 
                          data[14], data[15]);

    EXPECT_EQ(isAffine, matrix.isAffine());
  }

  // randomize data
  data = MatrixHelper::RandomMatrix4();

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
    std::vector<float32> data;

    // randomize data
    data = MatrixHelper::RandomMatrix4();

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
    std::vector<float32> data;

    // randomize data
    data = MatrixHelper::RandomMatrix4();

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
    std::vector<float32> data;

    // randomize data
    data = MatrixHelper::RandomMatrix4();

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
    std::vector<float32> data;

    // randomize data
    data = MatrixHelper::RandomMatrix4();
    
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
    std::vector<float32> data1;
    std::vector<float32> data2;
    std::vector<float32> dataOut;

    // randomize data
    data1 = MatrixHelper::RandomMatrix4();
    data2 = MatrixHelper::RandomMatrix4();

    Matrix4f matrix1(data1[0], data1[1], data1[2], data1[3], data1[4], data1[5], data1[6], data1[7], data1[8], data1[9], data1[10], data1[11], data1[12], 
                     data1[13], data1[14], data1[15]);
    Matrix4f matrix2(data2[0], data2[1], data2[2], data2[3], data2[4], data2[5], data2[6], data2[7], data2[8], data2[9], data2[10], data2[11], data2[12], 
                     data2[13], data2[14], data2[15]);

    // add using reference method
    dataOut = add(data1, data2);

    // add M1 and M2
    Matrix4f matrix = matrix1;
    matrix += matrix2;
    EXPECT_TRUE(MatrixHelper::AreEqual(dataOut, matrix.data));

    matrix = matrix1 + matrix2;
    EXPECT_TRUE(MatrixHelper::AreEqual(dataOut, matrix.data));

    // add M2 and M1
    matrix = matrix2;
    matrix += matrix1;
    EXPECT_TRUE(MatrixHelper::AreEqual(dataOut, matrix.data));

    matrix = matrix2 + matrix1;
    EXPECT_TRUE(MatrixHelper::AreEqual(dataOut, matrix.data));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Matrix4Test, Difference)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    std::vector<float32> data1;
    std::vector<float32> data2;
    std::vector<float32> dataOut;

    // randomize data
    data1 = MatrixHelper::RandomMatrix4();
    data2 = MatrixHelper::RandomMatrix4();

    Matrix4f matrix1(data1[0], data1[1], data1[2], data1[3], data1[4], data1[5], data1[6], data1[7], data1[8], data1[9], data1[10], data1[11], data1[12], 
                     data1[13], data1[14], data1[15]);
    Matrix4f matrix2(data2[0], data2[1], data2[2], data2[3], data2[4], data2[5], data2[6], data2[7], data2[8], data2[9], data2[10], data2[11], data2[12], 
                     data2[13], data2[14], data2[15]);

    // subtract using reference method
    dataOut = subtract(data1, data2);

    Matrix4f matrix = matrix1;
    matrix -= matrix2;
    EXPECT_TRUE(MatrixHelper::AreEqual(dataOut, matrix.data));

    matrix = matrix1 - matrix2;
    EXPECT_TRUE(MatrixHelper::AreEqual(dataOut, matrix.data));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(Matrix4Test, VectorMultiplication)
{
  // perform fixed number of tests
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    std::vector<float32> data;
    std::vector<float32> vector;
    std::vector<float32> vectorOut;

    // randomize data
    data = MatrixHelper::RandomMatrix4();
    vector = VectorHelper::RandomVector4Data();

    Vector4f vec(vector[0], vector[1], vector[2], vector[3]);
    const Matrix4f matrix(data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10], data[11], data[12], data[13], 
                          data[14], data[15]);

    // multiply 
    vectorOut = MathHelper::MultiplyVector(data, vector);

    Vector4f vecOut = matrix * vec;

    // verify
    EXPECT_FLOAT_EQ(vectorOut[0], vecOut.x);
    EXPECT_FLOAT_EQ(vectorOut[1], vecOut.y);
    EXPECT_FLOAT_EQ(vectorOut[2], vecOut.z);
    EXPECT_FLOAT_EQ(vectorOut[3], vecOut.w);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
