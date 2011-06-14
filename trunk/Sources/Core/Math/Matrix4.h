#ifndef EGE_CORE_MATRIX4_H
#define EGE_CORE_MATRIX4_H

// Matrix (col, row) is of the following form: (column major)
//
// | m00 m10 m20 m30 |
// | m01 m11 m21 m31 |
// | m02 m12 m22 m32 |
// | m03 m13 m23 m33 |

#include <EGE.h>
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class TMatrix4
{
	public:

    TMatrix4();
		TMatrix4(T m00, T m01, T m02, T m03, T m10, T m11, T m12, T m13, T m20, T m21, T m22, T m23, T m30, T m31, T m32, T m33);
		TMatrix4(const TMatrix4& matrix);

    inline TMatrix4  operator+(const TMatrix4& matrix) const;
    inline TMatrix4  operator-(const TMatrix4& matrix) const;
		inline TMatrix4& operator+=(const TMatrix4& matrix);
	  inline TMatrix4& operator-=(const TMatrix4& matrix);
    inline const T&  operator()(u32 column, u32 row) const;
    inline T&        operator()(u32 column, u32 row);
		inline T*        operator[](u32 column);
    inline const T*  operator[](u32 column) const;

    /* Multiplies current matrix by given one. */
    TMatrix4<T> multiply(const TMatrix4<T>& matrix) const;
    /* Returns transposed matrix. */
    inline TMatrix4<T> transposed() const;
    /* Returns TRUE if matrix is affine. */
    inline bool isAffine() const;
    
    //  CMatrix4 concatenateAffine( const CMatrix4& cMatrix ) const;                                      // multiplies affine matrices

  //  CMatrix4 getInverse( void ) const;                                                                // gets inversed matrix

  //  void set3x3Matrix( const CMatrix3* pcMatrix );                                                    // sets upper-left 3x3 sub-matrix

  //  void setTranslation( const CVector3* pcTranslation );                                             // sets translation vector within matrix


    T data[16];

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
TMatrix4<T>::TMatrix4(const TMatrix4<T>& matrix)
{
  *this = matrix;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TMatrix4<T> TMatrix4<T>::operator+(const TMatrix4<T>& matrix) const
{
  return *this += matrix;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TMatrix4<T> TMatrix4<T>::operator-(const TMatrix4<T>& matrix) const
{
  return *this -= matrix;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TMatrix4<T>& TMatrix4<T>::operator+=(const TMatrix4<T>& matrix)
{
  for (register u32 entry = 0; entry < 16; ++entry)
  {
    data[entry] += matrix.data[entry];
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TMatrix4<T>& TMatrix4<T>::operator-=(const TMatrix4<T>& matrix)
{
  for (register u32 entry = 0; entry < 16; ++entry)
  {
    data[entry] -= matrix.data[entry];
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
const T& TMatrix4<T>::operator()(u32 column, u32 row) const
{	
  EGE_ASSERT((4 > column) && (0 <= column) && (4 > row) && (0 <= row));
  return data[column * 4 + row]; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T& TMatrix4<T>::operator()(u32 column, u32 row)
{	
  EGE_ASSERT((4 > column) && (0 <= column) && (4 > row) && (0 <= row));
  return data[column * 4 + row]; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T* TMatrix4<T>::operator[](u32 column)
{
  EGE_ASSERT((4 > column) && (0 <= column));
  return &data[column * 4];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
const T* TMatrix4<T>::operator[](u32 column) const
{
  EGE_ASSERT((4 > column) && (0 <= column));
  return &data[column * 4];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if matrix is affine. */
template <typename T>
bool TMatrix4<T>::isAffine() const
{
  // NOTE: last row needs to be 0 0 0 1 for matrix to be affine
  return (0 == data[3]) && (0 == data[7]) && (0 == data[11]) && (1 == data[15]);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Multiplies current matrix by given one. */
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
/*! Returns transposed matrix. */
template <typename T>
TMatrix4<T> TMatrix4<T>::transposed() const
{
  return TMatrix4<T>(data[0], data[4], data[8], data[12], data[1], data[5], data[9], data[13], data[2], data[6], data[10], data[14], data[3], data[7], data[11], 
                     data[15]);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATRIX4_H