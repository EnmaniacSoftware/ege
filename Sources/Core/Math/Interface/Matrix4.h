#ifndef EGE_CORE_MATH_MATRIX4_H
#define EGE_CORE_MATH_MATRIX4_H

/** Class representing 4x4 matrix, in column major form (col, row). Each column represents a vector. Order of transformation is from right-to-left.
 *
 * | m00 m10 m20 m30 |
 * | m01 m11 m21 m31 |
 * | m02 m12 m22 m32 |
 * | m03 m13 m23 m33 |
 *
 */

#include "EGETypes.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
class TMatrix4
{
	public:

    TMatrix4();
		explicit TMatrix4(const T data[16]);
		TMatrix4(T m00, T m01, T m02, T m03, T m10, T m11, T m12, T m13, T m20, T m21, T m22, T m23, T m30, T m31, T m32, T m33);
		TMatrix4(const T column0[4], const T column1[4], const T column2[4], const T column3[4]);
		TMatrix4(const TMatrix4& matrix);

  operators:

		TMatrix4& operator += (const TMatrix4& matrix);
	  TMatrix4& operator -= (const TMatrix4& matrix);
	  TMatrix4& operator *= (const TMatrix4& matrix);		
    bool      operator == (const TMatrix4& other) const;
    bool      operator != (const TMatrix4& other) const;
    T*        operator [] (u32 column);
    const T*  operator [] (u32 column) const;

  public:

    /*! Concatenates current and given matrix. 
     *  @param  matrix  Matrix to concatenate with.
     *  @return Resulting matrix.
     */
    TMatrix4<T> multiply(const TMatrix4<T>& matrix) const;
    /*! Returns transposed matrix. */
    TMatrix4<T> transposed() const;
    /*! Returns TRUE if matrix is affine. */
    bool isAffine() const;
    /*! Sets scale part of the matrix. */
    void setScale(T x, T y, T z);
    /*! Returns scale factor along X axis. */
    T scaleX() const;
    /*! Returns scale factor along X axis. */
    T scaleY() const;
    /*! Returns scale factor along X axis. */
    T scaleZ() const;
    /*! Sets translation part of the matrix. */
    void setTranslation(T x, T y, T z);
    /*! Returns translation along X axis component value. */
    T translationX() const;
    /*! Returns translation along Y axis component value. */
    T translationY() const;
    /*! Returns translation along Z axis component value. */
    T translationZ() const;

  public:

    /*! Matrix data. */
    T data[16];

  public:

    static const TMatrix4<T> IDENTITY;
    static const TMatrix4<T> ZERO;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
const TMatrix4<T> TMatrix4<T>::IDENTITY = TMatrix4<T>(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
template <typename T>
const TMatrix4<T> TMatrix4<T>::ZERO     = TMatrix4<T>(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TMatrix4<T>::TMatrix4()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TMatrix4<T>::TMatrix4(T m00, T m01, T m02, T m03, T m10, T m11, T m12, T m13, T m20, T m21, T m22, T m23, T m30, T m31, T m32, T m33)
{
  data[0]  = m00;
  data[1]  = m01;
  data[2]  = m02;
  data[3]  = m03;
  data[4]  = m10;
  data[5]  = m11;
  data[6]  = m12;
  data[7]  = m13;
  data[8]  = m20;
  data[9]  = m21;
  data[10] = m22;
  data[11] = m23;
  data[12] = m30;
  data[13] = m31;
  data[14] = m32;
  data[15] = m33;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TMatrix4<T>::TMatrix4(const T column0[4], const T column1[4], const T column2[4], const T column3[4])
{
  data[0]  = column0[0];
  data[1]  = column0[1];
  data[2]  = column0[2];
  data[3]  = column0[3];
  data[4]  = column1[0];
  data[5]  = column1[1];
  data[6]  = column1[2];
  data[7]  = column1[3];
  data[8]  = column2[0];
  data[9]  = column2[1];
  data[10] = column2[2];
  data[11] = column2[3];
  data[12] = column3[0];
  data[13] = column3[1];
  data[14] = column3[2];
  data[15] = column3[3];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TMatrix4<T>::TMatrix4(const T data[16])
{
  EGE_MEMCPY(this->data, data, sizeof (data) * 16);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TMatrix4<T>::TMatrix4(const TMatrix4<T>& matrix)
{
  *this = matrix;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TMatrix4<T>& TMatrix4<T>::operator += (const TMatrix4<T>& matrix)
{
  for (register u32 entry = 0; entry < 16; ++entry)
  {
    data[entry] += matrix.data[entry];
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TMatrix4<T>& TMatrix4<T>::operator -= (const TMatrix4<T>& matrix)
{
  for (register u32 entry = 0; entry < 16; ++entry)
  {
    data[entry] -= matrix.data[entry];
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TMatrix4<T>& TMatrix4<T>::operator *= (const TMatrix4<T>& matrix)
{
  *this = multiply(matrix);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TMatrix4<T>::operator == (const TMatrix4<T>& other) const
{
  if ((data[0]  != other.data[0])  || (data[1]  != other.data[1])  || (data[2]  != other.data[2])  || (data[3]  != other.data[3])  ||
      (data[4]  != other.data[4])  || (data[5]  != other.data[5])  || (data[6]  != other.data[6])  || (data[7]  != other.data[7])  ||
      (data[8]  != other.data[8])  || (data[9]  != other.data[9])  || (data[10] != other.data[10]) || (data[11] != other.data[11]) ||
      (data[12] != other.data[12]) || (data[13] != other.data[13]) || (data[14] != other.data[14]) || (data[15] != other.data[15]))
  {
    return false;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TMatrix4<T>::operator != (const TMatrix4<T>& other) const
{
  return ! this->operator==(other);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T* TMatrix4<T>::operator [] (u32 column)
{
  EGE_ASSERT((4 > column) && (0 <= column));
  return &data[column * 4];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
const T* TMatrix4<T>::operator [] (u32 column) const
{
  EGE_ASSERT((4 > column) && (0 <= column));
  return &data[column * 4];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TMatrix4<T>::isAffine() const
{
  // NOTE: last row needs to be 0 0 0 1 for matrix to be affine
  return (0 == data[3]) && (0 == data[7]) && (0 == data[11]) && (1 == data[15]);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TMatrix4<T> TMatrix4<T>::multiply(const TMatrix4<T>& matrix) const
{
  TMatrix4<T> newMatrix;

  // 1st row
  newMatrix.data[0]  = data[0] * matrix.data[0]  + data[4] * matrix.data[1]  + data[8] * matrix.data[2]  + data[12] * matrix.data[3];
  newMatrix.data[4]  = data[0] * matrix.data[4]  + data[4] * matrix.data[5]  + data[8] * matrix.data[6]  + data[12] * matrix.data[7];
  newMatrix.data[8]  = data[0] * matrix.data[8]  + data[4] * matrix.data[9]  + data[8] * matrix.data[10] + data[12] * matrix.data[11];
  newMatrix.data[12] = data[0] * matrix.data[12] + data[4] * matrix.data[13] + data[8] * matrix.data[14] + data[12] * matrix.data[15];
  
  // 2nd row
  newMatrix.data[1]  = data[1] * matrix.data[0]  + data[5] * matrix.data[1]  + data[9] * matrix.data[2]  + data[13] * matrix.data[3];
  newMatrix.data[5]  = data[1] * matrix.data[4]  + data[5] * matrix.data[5]  + data[9] * matrix.data[6]  + data[13] * matrix.data[7];
  newMatrix.data[9]  = data[1] * matrix.data[8]  + data[5] * matrix.data[9]  + data[9] * matrix.data[10] + data[13] * matrix.data[11];
  newMatrix.data[13] = data[1] * matrix.data[12] + data[5] * matrix.data[13] + data[9] * matrix.data[14] + data[13] * matrix.data[15];

  // 3rd row
  newMatrix.data[2]  = data[2] * matrix.data[0]  + data[6] * matrix.data[1]  + data[10] * matrix.data[2]  + data[14] * matrix.data[3];
  newMatrix.data[6]  = data[2] * matrix.data[4]  + data[6] * matrix.data[5]  + data[10] * matrix.data[6]  + data[14] * matrix.data[7];
  newMatrix.data[10] = data[2] * matrix.data[8]  + data[6] * matrix.data[9]  + data[10] * matrix.data[10] + data[14] * matrix.data[11];
  newMatrix.data[14] = data[2] * matrix.data[12] + data[6] * matrix.data[13] + data[10] * matrix.data[14] + data[14] * matrix.data[15];

  // 4th row
  newMatrix.data[3]  = data[3] * matrix.data[0]  + data[7] * matrix.data[1]  + data[11] * matrix.data[2]  + data[15] * matrix.data[3];
  newMatrix.data[7]  = data[3] * matrix.data[4]  + data[7] * matrix.data[5]  + data[11] * matrix.data[6]  + data[15] * matrix.data[7];
  newMatrix.data[11] = data[3] * matrix.data[8]  + data[7] * matrix.data[9]  + data[11] * matrix.data[10] + data[15] * matrix.data[11];
  newMatrix.data[15] = data[3] * matrix.data[12] + data[7] * matrix.data[13] + data[11] * matrix.data[14] + data[15] * matrix.data[15];

  return newMatrix;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TMatrix4<T> TMatrix4<T>::transposed() const
{
  return TMatrix4<T>(data[0], data[4], data[8], data[12], data[1], data[5], data[9], data[13], data[2], data[6], data[10], data[14], data[3], data[7], data[11], 
                     data[15]);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TMatrix4<T>::setScale(T x, T y, T z)
{
  data[0]  = x;
  data[5]  = y;
  data[10] = z;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TMatrix4<T>::scaleX() const
{
  return data[0];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TMatrix4<T>::scaleY() const
{
  return data[5];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TMatrix4<T>::scaleZ() const
{
  return data[10];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void TMatrix4<T>::setTranslation(T x, T y, T z)
{
  data[12] = x;
  data[13] = y;
  data[14] = z;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TMatrix4<T>::translationX() const
{
  return data[12];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TMatrix4<T>::translationY() const
{
  return data[13];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TMatrix4<T>::translationZ() const
{
  return data[14];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TMatrix4<T> operator + (const TMatrix4<T>& left, const TMatrix4<T>& right)
{
  TMatrix4<T> out(left);
  out += right;

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TMatrix4<T> operator - (const TMatrix4<T>& left, const TMatrix4<T>& right)
{
  TMatrix4<T> out(left);
  out -= right;

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TMatrix4<T> operator * (const TMatrix4<T>& left, const TMatrix4<T>& right)
{
  return left.multiply(right);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
Debug operator << (Debug debug, const TMatrix4<T>& obj)
{
  debug.nospace() << "Matrix4(" << obj.data[0] << ", " << obj.data[4] << ", " << obj.data[8] << ", " << obj.data[12] << "\n";
  debug.nospace() << "        " << obj.data[1] << ", " << obj.data[5] << ", " << obj.data[9] << ", " << obj.data[13] << "\n";
  debug.nospace() << "        " << obj.data[2] << ", " << obj.data[6] << ", " << obj.data[10] << ", " << obj.data[14] << "\n";
  debug.nospace() << "        " << obj.data[3] << ", " << obj.data[7] << ", " << obj.data[11] << ", " << obj.data[15] << ")";

  return debug.space();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_MATRIX4_H