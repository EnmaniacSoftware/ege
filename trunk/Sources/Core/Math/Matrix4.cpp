#include "stdafx.h"

const CMatrix4 CMatrix4::IDENTITY = CMatrix4( 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 );
const CMatrix4 CMatrix4::ZERO     = CMatrix4( 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors

CMatrix4::CMatrix4( void )
{
}

CMatrix4::CMatrix4( float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, 
                    float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33 )
{
  // first column
  m_fData_[ 0 ]  = m00;
  m_fData_[ 1 ]  = m01;
  m_fData_[ 2 ]  = m02;
  m_fData_[ 3 ]  = m03;

  // second column
  m_fData_[ 4 ]  = m10;
  m_fData_[ 5 ]  = m11;
  m_fData_[ 6 ]  = m12;
  m_fData_[ 7 ]  = m13;

  // third column
  m_fData_[ 8 ]  = m20;
  m_fData_[ 9 ]  = m21;
  m_fData_[ 10 ] = m22;
  m_fData_[ 11 ] = m23;

  // forth column
  m_fData_[ 12 ] = m30;
  m_fData_[ 13 ] = m31;
  m_fData_[ 14 ] = m32;
  m_fData_[ 15 ] = m33;
}

CMatrix4::CMatrix4( const CMatrix4& cMatrix )
{
  *this = cMatrix;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Destructors

CMatrix4::~CMatrix4( void )
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLICS

CMatrix4 CMatrix4::operator+( const CMatrix4& cMatrix ) const
{
	CMatrix4 cNewMatrix( *this );

  cNewMatrix += cMatrix;

	return cNewMatrix;
}

CMatrix4 CMatrix4::operator+=( const CMatrix4& cMatrix )
{
	for ( register u32 uiEntry = 0; uiEntry < 16; ++uiEntry )
  {
		m_fData_[ uiEntry ] += cMatrix.m_fData_[ uiEntry ];
  }

	return *this;
}

CMatrix4 CMatrix4::operator-( const CMatrix4& cMatrix ) const
{
	CMatrix4 cNewMatrix( *this );

  cNewMatrix -= cMatrix;

	return cNewMatrix;
}

CMatrix4 CMatrix4::operator-=( const CMatrix4& cMatrix )
{
	for ( register u32 uiEntry = 0; uiEntry < 16; ++uiEntry )
  {
		m_fData_[ uiEntry ] -= cMatrix.m_fData_[ uiEntry ];
  }

	return *this;
}

void CMatrix4::makeTransform( const CVector3* pcPosition, const CVector3* pcScale, const CQuaternion* pcOrientation )
{
  assert( pcPosition != NULL && pcScale != NULL && pcOrientation != NULL && "No required parameter(s)" );

  // Ordering:
  //    1. Scale
  //    2. Rotate
  //    3. Translate

  CMatrix3 cRotationMatrix;
  CMatrix3 cScaleMatrix( pcScale->x, 0, 0, 0, pcScale->y, 0, 0, 0, pcScale->z );

  // convert quaternion into rotation matrix
  pcOrientation->convertTo( &cRotationMatrix );

  // set up final matrix with scale, rotation and translation
  set3x3Matrix( &cRotationMatrix.concatenate( cScaleMatrix ) );

  // apply translation
  setTranslation( pcPosition );

  // reset projection term
  m_fData[ 0 ][ 3 ] = 0; 
  m_fData[ 1 ][ 3 ] = 0; 
  m_fData[ 2 ][ 3 ] = 0; 
  m_fData[ 3 ][ 3 ] = 1;
}

CVector3 CMatrix4::transformAffine( const CVector3* pcVector ) const
{
  assert( pcVector != NULL && "No required parameter(s)" );
  assert( isAffine() && "Not affine" );

  return CVector3( m_fData[ 0 ][ 0 ]*pcVector->x+m_fData[ 1 ][ 0 ]*pcVector->y+m_fData[ 2 ][ 0 ]*pcVector->z+m_fData[ 3 ][ 0 ], 
                   m_fData[ 0 ][ 1 ]*pcVector->x+m_fData[ 1 ][ 1 ]*pcVector->y+m_fData[ 2 ][ 1 ]*pcVector->z+m_fData[ 3 ][ 1 ],
                   m_fData[ 0 ][ 2 ]*pcVector->x+m_fData[ 1 ][ 2 ]*pcVector->y+m_fData[ 2 ][ 2 ]*pcVector->z+m_fData[ 3 ][ 2 ] );
}

CVector3 CMatrix4::transform( const CVector3* pcVector ) const
{
  assert( pcVector != NULL && "No required parameter(s)" );

  CVector3 cResult;

  float fInvW = 1.0f/( m_fData[ 0 ][ 3 ]*pcVector->x+m_fData[ 1 ][ 3 ]*pcVector->y+m_fData[ 2 ][ 3 ]*pcVector->z+m_fData[ 3 ][ 3 ] );

  cResult.x = ( m_fData[ 0 ][ 0 ]*pcVector->x+m_fData[ 1 ][ 0 ]*pcVector->y+m_fData[ 2 ][ 0 ]*pcVector->z+m_fData[ 3 ][ 0 ] )*fInvW;
  cResult.y = ( m_fData[ 0 ][ 1 ]*pcVector->x+m_fData[ 1 ][ 1 ]*pcVector->y+m_fData[ 2 ][ 1 ]*pcVector->z+m_fData[ 3 ][ 1 ] )*fInvW;
  cResult.z = ( m_fData[ 0 ][ 2 ]*pcVector->x+m_fData[ 1 ][ 2 ]*pcVector->y+m_fData[ 2 ][ 2 ]*pcVector->z+m_fData[ 3 ][ 2 ] )*fInvW;

  return cResult;
}

CVector4 CMatrix4::transform( const CVector4* pcVector ) const
{
  assert( pcVector != NULL && "No required parameter(s)" );

  return CVector4( m_fData[ 0 ][ 0 ]*pcVector->x+m_fData[ 1 ][ 0 ]*pcVector->y+m_fData[ 2 ][ 0 ]*pcVector->z+m_fData[ 3 ][ 0 ]*pcVector->w, 
                   m_fData[ 0 ][ 1 ]*pcVector->x+m_fData[ 1 ][ 1 ]*pcVector->y+m_fData[ 2 ][ 1 ]*pcVector->z+m_fData[ 3 ][ 1 ]*pcVector->w,
                   m_fData[ 0 ][ 2 ]*pcVector->x+m_fData[ 1 ][ 2 ]*pcVector->y+m_fData[ 2 ][ 2 ]*pcVector->z+m_fData[ 3 ][ 2 ]*pcVector->w,
                   m_fData[ 0 ][ 3 ]*pcVector->x+m_fData[ 1 ][ 3 ]*pcVector->y+m_fData[ 2 ][ 3 ]*pcVector->z+m_fData[ 3 ][ 3 ]*pcVector->w );
}

void CMatrix4::setTranslation( const CVector3* pcTranslation )
{
  assert( pcTranslation != NULL && "No required parameter(s)" );

  m_fData_[ 12 ] = pcTranslation->x; 
  m_fData_[ 13 ] = pcTranslation->y; 
  m_fData_[ 14 ] = pcTranslation->z;
}

//CVector3 CMatrix4::getMultiplyVectorByMatrix3x3( const CVector3& cVector )
//{
//	CVector3 cNewVector;
//
//	cNewVector.x = m_fData[ 0 ]*cVector.x+m_fData[ 4 ]*cVector.y+m_fData[ 8 ]*cVector.z;
//	cNewVector.y = m_fData[ 1 ]*cVector.x+m_fData[ 5 ]*cVector.y+m_fData[ 9 ]*cVector.z;
//	cNewVector.z = m_fData[ 2 ]*cVector.x+m_fData[ 6 ]*cVector.y+m_fData[ 10 ]*cVector.z;
//
//	return cNewVector;
//}

//CMatrix4 CMatrix4::getRotateX( float fRadians )
//{
//	float fSinTheta = Math::Sin( fRadians );
//	float fCosTheta = Math::Cos( fRadians );
//
//	CMatrix4 cRotationMatrix( 1, 0, 0, 0, 0, fCosTheta, fSinTheta, 0, 0, -fSinTheta, fCosTheta, 0, 0, 0, 0, 1 );
//
//	// multiply
//	return *this*cRotationMatrix;
//}
//
//void CMatrix4::rotateX( float fRadians )
//{
//  *this = getRotateX( fRadians );
//}
//
//CMatrix4 CMatrix4::getRotateY( float fRadians )
//{
//	float fSinTheta = Math::Sin( fRadians );
//	float fCosTheta = Math::Cos( fRadians );
//
//	CMatrix4 cRotationMatrix( fCosTheta, 0, -fSinTheta, 0, 0, 1, 0, 0, fSinTheta, 0, fCosTheta, 0, 0, 0, 0, 1 );
//
//  // multiply
//	return *this*cRotationMatrix;
//}
//
//void CMatrix4::rotateY( float fRadians )
//{
//  *this = getRotateY( fRadians );
//}
//
//CMatrix4 CMatrix4::getRotateZ( float fRadians )
//{
//	float fSinTheta = Math::Sin( fRadians );
//	float fCosTheta = Math::Cos( fRadians );
//
//	CMatrix4 cRotationMatrix( fCosTheta, fSinTheta, 0, 0, -fSinTheta, fCosTheta, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 );
//
//	// multiply
//	return ( *this )*cRotationMatrix;
//}
//
//void CMatrix4::rotateZ( float fRadians )
//{
//  *this = getRotateZ( fRadians );
//}
//
//void CMatrix4::translate( const CVector3& cVector )
//{
//  *this = getTranslate( cVector );
//}
//
//CMatrix4 CMatrix4::getTranslate( const CVector3& cVector )
//{
//	CMatrix4 cNewMatrix( 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, cVector.x, cVector.y, cVector.z, 1 );
//
//	cNewMatrix *= *this;
//
//	return cNewMatrix;
//}
//
//void CMatrix4::scale( const CVector3& cScale )
//{
//  *this = getScale( cScale );
//}
//
//CMatrix4 CMatrix4::getScale( const CVector3& cScale )
//{
//	CMatrix4 cScaleMatrix( cScale.x, 0, 0, 0, 0, cScale.y, 0, 0, 0, 0, cScale.z, 0, 0, 0, 0, 1 );
//
//  // multiply
//  return ( *this )*cScaleMatrix;
//}
//
//void CMatrix4::inverse( void )
//{
//  *this = getInverse();
//}
//
CMatrix4 CMatrix4::getInverse( void ) const
{
#define MAT( m, r, c )( m )[ ( c )*4+( r ) ]

#define m11 MAT( m_fData_, 0, 0 )
#define m12 MAT( m_fData_, 0, 1 )
#define m13 MAT( m_fData_, 0, 2 )
#define m14 MAT( m_fData_, 0, 3 )
#define m21 MAT( m_fData_, 1, 0 )
#define m22 MAT( m_fData_, 1, 1 )
#define m23 MAT( m_fData_, 1, 2 )
#define m24 MAT( m_fData_, 1, 3 )
#define m31 MAT( m_fData_, 2, 0 )
#define m32 MAT( m_fData_, 2, 1 )
#define m33 MAT( m_fData_, 2, 2 )
#define m34 MAT( m_fData_, 2, 3 )
#define m41 MAT( m_fData_, 3, 0 )
#define m42 MAT( m_fData_, 3, 1 )
#define m43 MAT( m_fData_, 3, 2 )
#define m44 MAT( m_fData_, 3, 3 )

  CMatrix4 cResult;

  float det;
  float d12, d13, d23, d24, d34, d41;

  //            adjoint
  // Inverse = ---------
  //              det

  // pre-compute 2x2 dets for last two rows when computing cofactors of first two rows
  d12 = ( m31*m42-m41*m32 );
  d13 = ( m31*m43-m41*m33 );
  d23 = ( m32*m43-m42*m33 );
  d24 = ( m32*m44-m42*m34 );
  d34 = ( m33*m44-m43*m34 );
  d41 = ( m34*m41-m44*m31 );

  cResult.m_fData_[ 0 ] =  ( m22*d34-m23*d24+m24*d23 );
  cResult.m_fData_[ 1 ] = -( m21*d34+m23*d41+m24*d13 );
  cResult.m_fData_[ 2 ] =  ( m21*d24+m22*d41+m24*d12 );
  cResult.m_fData_[ 3 ] = -( m21*d23-m22*d13+m23*d12 );

  // compute determinant as early as possible using these cofactors
  det = m11*cResult.m_fData_[ 0 ]+m12*cResult.m_fData_[ 1 ]+m13*cResult.m_fData_[ 2 ]+m14*cResult.m_fData_[ 3 ];
  Math::ZeroRoundOff( det );

  // run singularity test
  if ( det == 0.0f )
  {
    return CMatrix4::IDENTITY;
  }

  float invDet = 1.0f/det;

  // compute rest of inverse
  cResult.m_fData_[ 0 ] *= invDet;
  cResult.m_fData_[ 1 ] *= invDet;
  cResult.m_fData_[ 2 ] *= invDet;
  cResult.m_fData_[ 3 ] *= invDet;

  cResult.m_fData_[ 4 ] = -( m12*d34-m13*d24+m14*d23 )*invDet;
  cResult.m_fData_[ 5 ] =  ( m11*d34+m13*d41+m14*d13 )*invDet;
  cResult.m_fData_[ 6 ] = -( m11*d24+m12*d41+m14*d12 )*invDet;
  cResult.m_fData_[ 7 ] =  ( m11*d23-m12*d13+m13*d12 )*invDet;

  // pre-compute 2x2 dets for first two rows when computing cofactors of last two rows
  d12 = m11*m22-m21*m12;
  d13 = m11*m23-m21*m13;
  d23 = m12*m23-m22*m13;
  d24 = m12*m24-m22*m14;
  d34 = m13*m24-m23*m14;
  d41 = m14*m21-m24*m11;

  cResult.m_fData_[ 8 ]  =  ( m42*d34-m43*d24+m44*d23 )*invDet;
  cResult.m_fData_[ 9 ]  = -( m41*d34+m43*d41+m44*d13 )*invDet;
  cResult.m_fData_[ 10 ] =  ( m41*d24+m42*d41+m44*d12 )*invDet;
  cResult.m_fData_[ 11 ] = -( m41*d23-m42*d13+m43*d12 )*invDet;
  cResult.m_fData_[ 12 ] = -( m32*d34-m33*d24+m34*d23 )*invDet;
  cResult.m_fData_[ 13 ] =  ( m31*d34+m33*d41+m34*d13 )*invDet;
  cResult.m_fData_[ 14 ] = -( m31*d24+m32*d41+m34*d12 )*invDet;
  cResult.m_fData_[ 15 ] =  ( m31*d23-m32*d13+m33*d12 )*invDet;

#undef m11
#undef m12
#undef m13
#undef m14
#undef m21
#undef m22
#undef m23
#undef m24
#undef m31
#undef m32
#undef m33
#undef m34
#undef m41
#undef m42
#undef m43
#undef m44
#undef MAT

  return cResult;
}
//
//void CMatrix4::multiplyByMatrix3x3( const CMatrix4& cMatrix )
//{
//  *this = getMultiplyByMatrix3x3( cMatrix );
//}
//
//CMatrix4 CMatrix4::getMultiplyByMatrix3x3( const CMatrix4& cMatrix )
//{
//  CMatrix4 cNewMatrix;
//
//  cNewMatrix.m_fData[ 0 ] = cMatrix.m_fData[ 0 ]*m_fData[ 0 ]+cMatrix.m_fData[ 4 ]*m_fData[ 1 ]+cMatrix.m_fData[ 8 ]*m_fData[ 2 ];
//  cNewMatrix.m_fData[ 4 ] = cMatrix.m_fData[ 0 ]*m_fData[ 4 ]+cMatrix.m_fData[ 4 ]*m_fData[ 5 ]+cMatrix.m_fData[ 8 ]*m_fData[ 6 ];
//  cNewMatrix.m_fData[ 8 ] = cMatrix.m_fData[ 0 ]*m_fData[ 8 ]+cMatrix.m_fData[ 4 ]*m_fData[ 9 ]+cMatrix.m_fData[ 8 ]*m_fData[ 10 ];
//
//  cNewMatrix.m_fData[ 1 ] = cMatrix.m_fData[ 1 ]*m_fData[ 0 ]+cMatrix.m_fData[ 5 ]*m_fData[ 1 ]+cMatrix.m_fData[ 9 ]*m_fData[ 2 ];
//  cNewMatrix.m_fData[ 5 ] = cMatrix.m_fData[ 1 ]*m_fData[ 4 ]+cMatrix.m_fData[ 5 ]*m_fData[ 5 ]+cMatrix.m_fData[ 9 ]*m_fData[ 6 ];
//  cNewMatrix.m_fData[ 9 ] = cMatrix.m_fData[ 1 ]*m_fData[ 8 ]+cMatrix.m_fData[ 5 ]*m_fData[ 9 ]+cMatrix.m_fData[ 9 ]*m_fData[ 10 ];
//
//  cNewMatrix.m_fData[ 2 ] = cMatrix.m_fData[ 2 ]*m_fData[ 0 ]+cMatrix.m_fData[ 6 ]*m_fData[ 1 ]+cMatrix.m_fData[ 10 ]*m_fData[ 2 ];
//  cNewMatrix.m_fData[ 6 ] = cMatrix.m_fData[ 2 ]*m_fData[ 4 ]+cMatrix.m_fData[ 6 ]*m_fData[ 5 ]+cMatrix.m_fData[ 10 ]*m_fData[ 6 ];
//  cNewMatrix.m_fData[ 10 ] = cMatrix.m_fData[ 2 ]*m_fData[ 8 ]+cMatrix.m_fData[ 6 ]*m_fData[ 9 ]+cMatrix.m_fData[ 10 ]*m_fData[ 10 ];
//
//  cNewMatrix.m_fData[ 3 ] = m_fData[ 3 ];
//  cNewMatrix.m_fData[ 7 ] = m_fData[ 7 ];
//  cNewMatrix.m_fData[ 11 ] = m_fData[ 11 ];
//
//  cNewMatrix.m_fData[ 12 ] = m_fData[ 12 ];
//  cNewMatrix.m_fData[ 13 ] = m_fData[ 13 ];
//  cNewMatrix.m_fData[ 14 ] = m_fData[ 14 ];
//  cNewMatrix.m_fData[ 15 ] = m_fData[ 15 ];
//
//  return cNewMatrix;
//}

CMatrix4 CMatrix4::concatenate( const CMatrix4& cMatrix ) const
{
  CMatrix4 cNewMatrix;

  cNewMatrix.m_fData[ 0 ][ 0 ] = m_fData[ 0 ][ 0 ]*cMatrix.m_fData[ 0 ][ 0 ]+m_fData[ 1 ][ 0 ]*cMatrix.m_fData[ 0 ][ 1 ]+m_fData[ 2 ][ 0 ]*cMatrix.m_fData[ 0 ][ 2 ]+m_fData[ 3 ][ 0 ]*cMatrix.m_fData[ 0 ][ 3 ];
  cNewMatrix.m_fData[ 1 ][ 0 ] = m_fData[ 0 ][ 0 ]*cMatrix.m_fData[ 1 ][ 0 ]+m_fData[ 1 ][ 0 ]*cMatrix.m_fData[ 1 ][ 1 ]+m_fData[ 2 ][ 0 ]*cMatrix.m_fData[ 1 ][ 2 ]+m_fData[ 3 ][ 0 ]*cMatrix.m_fData[ 1 ][ 3 ];
  cNewMatrix.m_fData[ 2 ][ 0 ] = m_fData[ 0 ][ 0 ]*cMatrix.m_fData[ 2 ][ 0 ]+m_fData[ 1 ][ 0 ]*cMatrix.m_fData[ 2 ][ 1 ]+m_fData[ 2 ][ 0 ]*cMatrix.m_fData[ 2 ][ 2 ]+m_fData[ 3 ][ 0 ]*cMatrix.m_fData[ 2 ][ 3 ];
  cNewMatrix.m_fData[ 3 ][ 0 ] = m_fData[ 0 ][ 0 ]*cMatrix.m_fData[ 3 ][ 0 ]+m_fData[ 1 ][ 0 ]*cMatrix.m_fData[ 3 ][ 1 ]+m_fData[ 2 ][ 0 ]*cMatrix.m_fData[ 3 ][ 2 ]+m_fData[ 3 ][ 0 ]*cMatrix.m_fData[ 3 ][ 3 ];

  cNewMatrix.m_fData[ 0 ][ 1 ] = m_fData[ 0 ][ 1 ]*cMatrix.m_fData[ 0 ][ 0 ]+m_fData[ 1 ][ 1 ]*cMatrix.m_fData[ 0 ][ 1 ]+m_fData[ 2 ][ 1 ]*cMatrix.m_fData[ 0 ][ 2 ]+m_fData[ 3 ][ 1 ]*cMatrix.m_fData[ 0 ][ 3 ];
  cNewMatrix.m_fData[ 1 ][ 1 ] = m_fData[ 0 ][ 1 ]*cMatrix.m_fData[ 1 ][ 0 ]+m_fData[ 1 ][ 1 ]*cMatrix.m_fData[ 1 ][ 1 ]+m_fData[ 2 ][ 1 ]*cMatrix.m_fData[ 1 ][ 2 ]+m_fData[ 3 ][ 1 ]*cMatrix.m_fData[ 1 ][ 3 ];
  cNewMatrix.m_fData[ 2 ][ 1 ] = m_fData[ 0 ][ 1 ]*cMatrix.m_fData[ 2 ][ 0 ]+m_fData[ 1 ][ 1 ]*cMatrix.m_fData[ 2 ][ 1 ]+m_fData[ 2 ][ 1 ]*cMatrix.m_fData[ 2 ][ 2 ]+m_fData[ 3 ][ 1 ]*cMatrix.m_fData[ 2 ][ 3 ];
  cNewMatrix.m_fData[ 3 ][ 1 ] = m_fData[ 0 ][ 1 ]*cMatrix.m_fData[ 3 ][ 0 ]+m_fData[ 1 ][ 1 ]*cMatrix.m_fData[ 3 ][ 1 ]+m_fData[ 2 ][ 1 ]*cMatrix.m_fData[ 3 ][ 2 ]+m_fData[ 3 ][ 1 ]*cMatrix.m_fData[ 3 ][ 3 ];

  cNewMatrix.m_fData[ 0 ][ 2 ] = m_fData[ 0 ][ 2 ]*cMatrix.m_fData[ 0 ][ 0 ]+m_fData[ 1 ][ 2 ]*cMatrix.m_fData[ 0 ][ 1 ]+m_fData[ 2 ][ 2 ]*cMatrix.m_fData[ 0 ][ 2 ]+m_fData[ 3 ][ 2 ]*cMatrix.m_fData[ 0 ][ 3 ];
  cNewMatrix.m_fData[ 1 ][ 2 ] = m_fData[ 0 ][ 2 ]*cMatrix.m_fData[ 1 ][ 0 ]+m_fData[ 1 ][ 2 ]*cMatrix.m_fData[ 1 ][ 1 ]+m_fData[ 2 ][ 2 ]*cMatrix.m_fData[ 1 ][ 2 ]+m_fData[ 3 ][ 2 ]*cMatrix.m_fData[ 1 ][ 3 ];
  cNewMatrix.m_fData[ 2 ][ 2 ] = m_fData[ 0 ][ 2 ]*cMatrix.m_fData[ 2 ][ 0 ]+m_fData[ 1 ][ 2 ]*cMatrix.m_fData[ 2 ][ 1 ]+m_fData[ 2 ][ 2 ]*cMatrix.m_fData[ 2 ][ 2 ]+m_fData[ 3 ][ 2 ]*cMatrix.m_fData[ 2 ][ 3 ];
  cNewMatrix.m_fData[ 3 ][ 2 ] = m_fData[ 0 ][ 2 ]*cMatrix.m_fData[ 3 ][ 0 ]+m_fData[ 1 ][ 2 ]*cMatrix.m_fData[ 3 ][ 1 ]+m_fData[ 2 ][ 2 ]*cMatrix.m_fData[ 3 ][ 2 ]+m_fData[ 3 ][ 2 ]*cMatrix.m_fData[ 3 ][ 3 ];

  cNewMatrix.m_fData[ 0 ][ 3 ] = m_fData[ 0 ][ 3 ]*cMatrix.m_fData[ 0 ][ 0 ]+m_fData[ 1 ][ 3 ]*cMatrix.m_fData[ 0 ][ 1 ]+m_fData[ 2 ][ 3 ]*cMatrix.m_fData[ 0 ][ 2 ]+m_fData[ 3 ][ 3 ]*cMatrix.m_fData[ 0 ][ 3 ];
  cNewMatrix.m_fData[ 1 ][ 3 ] = m_fData[ 0 ][ 3 ]*cMatrix.m_fData[ 1 ][ 0 ]+m_fData[ 1 ][ 3 ]*cMatrix.m_fData[ 1 ][ 1 ]+m_fData[ 2 ][ 3 ]*cMatrix.m_fData[ 1 ][ 2 ]+m_fData[ 3 ][ 3 ]*cMatrix.m_fData[ 1 ][ 3 ];
  cNewMatrix.m_fData[ 2 ][ 3 ] = m_fData[ 0 ][ 3 ]*cMatrix.m_fData[ 2 ][ 0 ]+m_fData[ 1 ][ 3 ]*cMatrix.m_fData[ 2 ][ 1 ]+m_fData[ 2 ][ 3 ]*cMatrix.m_fData[ 2 ][ 2 ]+m_fData[ 3 ][ 3 ]*cMatrix.m_fData[ 2 ][ 3 ];
  cNewMatrix.m_fData[ 3 ][ 3 ] = m_fData[ 0 ][ 3 ]*cMatrix.m_fData[ 3 ][ 0 ]+m_fData[ 1 ][ 3 ]*cMatrix.m_fData[ 3 ][ 1 ]+m_fData[ 2 ][ 3 ]*cMatrix.m_fData[ 3 ][ 2 ]+m_fData[ 3 ][ 3 ]*cMatrix.m_fData[ 3 ][ 3 ];

  return cNewMatrix;
}

CMatrix4 CMatrix4::concatenateAffine( const CMatrix4& cMatrix ) const
{
  assert( isAffine() && cMatrix.isAffine() && "Matrices must be affine!" );

  CMatrix4 cNewMatrix;

  cNewMatrix.m_fData[ 0 ][ 0 ] = m_fData[ 0 ][ 0 ]*cMatrix.m_fData[ 0 ][ 0 ]+m_fData[ 1 ][ 0 ]*cMatrix.m_fData[ 0 ][ 1 ]+m_fData[ 2 ][ 0 ]*cMatrix.m_fData[ 0 ][ 2 ];
  cNewMatrix.m_fData[ 1 ][ 0 ] = m_fData[ 0 ][ 0 ]*cMatrix.m_fData[ 1 ][ 0 ]+m_fData[ 1 ][ 0 ]*cMatrix.m_fData[ 1 ][ 1 ]+m_fData[ 2 ][ 0 ]*cMatrix.m_fData[ 1 ][ 2 ];
  cNewMatrix.m_fData[ 2 ][ 0 ] = m_fData[ 0 ][ 0 ]*cMatrix.m_fData[ 2 ][ 0 ]+m_fData[ 1 ][ 0 ]*cMatrix.m_fData[ 2 ][ 1 ]+m_fData[ 2 ][ 0 ]*cMatrix.m_fData[ 2 ][ 2 ];
  cNewMatrix.m_fData[ 3 ][ 0 ] = m_fData[ 0 ][ 0 ]*cMatrix.m_fData[ 3 ][ 0 ]+m_fData[ 1 ][ 0 ]*cMatrix.m_fData[ 3 ][ 1 ]+m_fData[ 2 ][ 0 ]*cMatrix.m_fData[ 3 ][ 2 ]+m_fData[ 3 ][ 0 ];

  cNewMatrix.m_fData[ 0 ][ 1 ] = m_fData[ 0 ][ 1 ]*cMatrix.m_fData[ 0 ][ 0 ]+m_fData[ 1 ][ 1 ]*cMatrix.m_fData[ 0 ][ 1 ]+m_fData[ 2 ][ 1 ]*cMatrix.m_fData[ 0 ][ 2 ];
  cNewMatrix.m_fData[ 1 ][ 1 ] = m_fData[ 0 ][ 1 ]*cMatrix.m_fData[ 1 ][ 0 ]+m_fData[ 1 ][ 1 ]*cMatrix.m_fData[ 1 ][ 1 ]+m_fData[ 2 ][ 1 ]*cMatrix.m_fData[ 1 ][ 2 ];
  cNewMatrix.m_fData[ 2 ][ 1 ] = m_fData[ 0 ][ 1 ]*cMatrix.m_fData[ 2 ][ 0 ]+m_fData[ 1 ][ 1 ]*cMatrix.m_fData[ 2 ][ 1 ]+m_fData[ 2 ][ 1 ]*cMatrix.m_fData[ 2 ][ 2 ];
  cNewMatrix.m_fData[ 3 ][ 1 ] = m_fData[ 0 ][ 1 ]*cMatrix.m_fData[ 3 ][ 0 ]+m_fData[ 1 ][ 1 ]*cMatrix.m_fData[ 3 ][ 1 ]+m_fData[ 2 ][ 1 ]*cMatrix.m_fData[ 3 ][ 2 ]+m_fData[ 3 ][ 1 ];

  cNewMatrix.m_fData[ 0 ][ 2 ] = m_fData[ 0 ][ 2 ]*cMatrix.m_fData[ 0 ][ 0 ]+m_fData[ 1 ][ 2 ]*cMatrix.m_fData[ 0 ][ 1 ]+m_fData[ 2 ][ 2 ]*cMatrix.m_fData[ 0 ][ 2 ];
  cNewMatrix.m_fData[ 1 ][ 2 ] = m_fData[ 0 ][ 2 ]*cMatrix.m_fData[ 1 ][ 0 ]+m_fData[ 1 ][ 2 ]*cMatrix.m_fData[ 1 ][ 1 ]+m_fData[ 2 ][ 2 ]*cMatrix.m_fData[ 1 ][ 2 ];
  cNewMatrix.m_fData[ 2 ][ 2 ] = m_fData[ 0 ][ 2 ]*cMatrix.m_fData[ 2 ][ 0 ]+m_fData[ 1 ][ 2 ]*cMatrix.m_fData[ 2 ][ 1 ]+m_fData[ 2 ][ 2 ]*cMatrix.m_fData[ 2 ][ 2 ];
  cNewMatrix.m_fData[ 3 ][ 2 ] = m_fData[ 0 ][ 2 ]*cMatrix.m_fData[ 3 ][ 0 ]+m_fData[ 1 ][ 2 ]*cMatrix.m_fData[ 3 ][ 1 ]+m_fData[ 2 ][ 2 ]*cMatrix.m_fData[ 3 ][ 2 ]+m_fData[ 3 ][ 2 ];

  cNewMatrix.m_fData[ 0 ][ 3 ] = 0;
  cNewMatrix.m_fData[ 1 ][ 3 ] = 0;
  cNewMatrix.m_fData[ 2 ][ 3 ] = 0;
  cNewMatrix.m_fData[ 3 ][ 3 ] = 1;

  return cNewMatrix;
}

void CMatrix4::set3x3Matrix( const CMatrix3* pcMatrix )
{
  assert( pcMatrix != NULL && "No required parameter(s)" );

  m_fData[ 0 ][ 0 ] = pcMatrix->m_fData[ 0 ][ 0 ]; 
  m_fData[ 0 ][ 1 ] = pcMatrix->m_fData[ 0 ][ 1 ]; 
  m_fData[ 0 ][ 2 ] = pcMatrix->m_fData[ 0 ][ 2 ];
  m_fData[ 1 ][ 0 ] = pcMatrix->m_fData[ 1 ][ 0 ]; 
  m_fData[ 1 ][ 1 ] = pcMatrix->m_fData[ 1 ][ 1 ]; 
  m_fData[ 1 ][ 2 ] = pcMatrix->m_fData[ 1 ][ 2 ];
  m_fData[ 2 ][ 0 ] = pcMatrix->m_fData[ 2 ][ 0 ]; 
  m_fData[ 2 ][ 1 ] = pcMatrix->m_fData[ 2 ][ 1 ]; 
  m_fData[ 2 ][ 2 ] = pcMatrix->m_fData[ 2 ][ 2 ];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATES
