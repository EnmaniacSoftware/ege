#include "stdafx.h"

const CQuaternion CQuaternion::IDENTITY = CQuaternion( 0, 0, 0, 1 );
const CQuaternion CQuaternion::ZERO     = CQuaternion( 0, 0, 0, 0 );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors

CQuaternion::CQuaternion( void )
{
}

CQuaternion::CQuaternion( float fX, float fY, float fZ, float fW )
{
  // set values
  x = fX;
  y = fY;
  z = fZ;
  w = fW;
}

CQuaternion::CQuaternion( const CQuaternion& cQuaternion )
{
  *this = cQuaternion;
}

CQuaternion::CQuaternion( const CVector3* pcAxis, float fRadians )
{
  assert( pcAxis != NULL && "No required parameter(s)" );

  // create from axis and rotation along it
  create( pcAxis, fRadians );
}

CQuaternion::CQuaternion( const CMatrix3* pcMatrix )
{
  assert( pcMatrix != NULL && "No required parameter(s)" );

  // create from matrix
  create( pcMatrix );
}

CQuaternion::CQuaternion( const CVector3* pcAxisX, const CVector3* pcAxisY, const CVector3* pcAxisZ )
{
  assert( pcAxisX != NULL && pcAxisY != NULL && pcAxisZ != NULL && "No required parameter(s)" );

  // create from axes
  create( pcAxisX, pcAxisY, pcAxisZ );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Destructors

CQuaternion::~CQuaternion( void )
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLICS

CQuaternion CQuaternion::operator+( const CQuaternion& cQuaternion ) const
{
  return CQuaternion( x+cQuaternion.x, y+cQuaternion.y, z+cQuaternion.z, w+cQuaternion.w );
}

CQuaternion CQuaternion::operator-( const CQuaternion& cQuaternion ) const
{
  return CQuaternion( x-cQuaternion.x, y-cQuaternion.y, z-cQuaternion.z, w-cQuaternion.w );
}
 
CQuaternion CQuaternion::operator/( const CQuaternion& cQuaternion ) const
{
  CQuaternion cInv;
  CQuaternion cNew;

  // inverse (conjugate) denominator quaternion
  cInv = cQuaternion.getInverse();

  // multiply current quaternion by inverse of the other one
  cNew = concatenate( cInv );

  return cNew;
}

CQuaternion CQuaternion::operator*( float fNumber ) const
{
  return CQuaternion( x*fNumber, y*fNumber, z*fNumber, w*fNumber );
}

CQuaternion CQuaternion::operator/( float fNumber ) const
{
  assert( fNumber != 0.0f );

  float fInv = 1.0f/fNumber;

  return CQuaternion( x*fInv, y*fInv, z*fInv, w*fInv );
}

void CQuaternion::operator+=( const CQuaternion& cQuaternion )
{
  x += cQuaternion.x;
  y += cQuaternion.y;
  z += cQuaternion.z;
  w += cQuaternion.w;
}

void CQuaternion::operator-=( const CQuaternion& cQuaternion )
{
  x -= cQuaternion.x;
  y -= cQuaternion.y;
  z -= cQuaternion.z;
  w -= cQuaternion.w;
}

void CQuaternion::operator*=( const CQuaternion& cQuaternion )
{
  CQuaternion cNew;

  // combine the existing quaternion with the new one
  cNew.x = w*cQuaternion.x+x*cQuaternion.w+y*cQuaternion.z-z*cQuaternion.y;
  cNew.y = w*cQuaternion.y+y*cQuaternion.w+z*cQuaternion.x-x*cQuaternion.z;
  cNew.z = w*cQuaternion.z+z*cQuaternion.w+x*cQuaternion.y-y*cQuaternion.x;
  cNew.w = w*cQuaternion.w-x*cQuaternion.x-y*cQuaternion.y-z*cQuaternion.z;

  // normalize it
  cNew.normalize();

  // store as current one
  x = cNew.x;
  y = cNew.y;
  z = cNew.z;
  w = cNew.w;
}

void CQuaternion::operator/=( const CQuaternion& cQuaternion )
{
  CQuaternion cInv;
  CQuaternion cNew;

  // inverse (conjugate) denominator quaternion
  cInv = cQuaternion.getInverse();

  // multiply current quaternion by inverse of the other one
  cNew = concatenate( cInv );

  *this = cNew;
}

void CQuaternion::operator*=( float fNumber )
{
  x *= fNumber;
  y *= fNumber;
  z *= fNumber;
  w *= fNumber;
}

void CQuaternion::operator/=( float fNumber )
{
  assert( fNumber != 0.0f );

  float fInv = 1.0f/fNumber;

  x *= fInv;
  y *= fInv;
  z *= fInv;
  w *= fInv;
}

CVector3 CQuaternion::transform( const CVector3* pcVector ) const
{
  assert( pcVector != NULL && "No required parameter(s)" );

	CVector3 uv;
  CVector3 uuv;
	CVector3 qvec( x, y, z );

  // nVidia SDK implementation	
  uv = qvec.getCrossProduct( *pcVector );
	uuv = qvec.getCrossProduct( uv );
	uv *= ( 2.0f*w );
	uuv *= 2.0f;

	return ( *pcVector )+uv+uuv;
}

void CQuaternion::create( const CVector3* pcAxis, float fRadians )
{
  assert( pcAxis != NULL && "No required parameter(s)" );

  // cAxis is unit length
  //
  // The quaternion representing the rotation is
  //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

  // get half angle
  float fHalfAngle = fRadians*0.5f;

  // calculate the sin( fHalfAngle ) once for optimization
	float fSin = Math::Sin( fHalfAngle );
		
	// calculate the x, y and z of the quaternion
	x = pcAxis->x*fSin;
	y = pcAxis->y*fSin;
	z = pcAxis->z*fSin;
	w = Math::Cos( fHalfAngle );
}

void CQuaternion::create( const CMatrix3* pcMatrix )
{
  assert( pcMatrix != NULL && "No required parameter(s)" );

  float fMatrixTrace;
  float fS;
//  float fInvS;

  // get matrix trace (sum of diagonal elements)
  fMatrixTrace = pcMatrix->m_fData_[ 0 ]+pcMatrix->m_fData_[ 4 ]+pcMatrix->m_fData_[ 8 ];

  // check if trace is positive
  if ( fMatrixTrace > 0 )
  {
    fS = Math::Sqrt( fMatrixTrace+1 );

    w = fS*0.5f;

    fS = 0.5f/fS;

    //x = ( cMatrix.m_fData_[ 5 ]-cMatrix.m_fData_[ 7 ] )*fS;
    //y = ( cMatrix.m_fData_[ 6 ]-cMatrix.m_fData_[ 2 ] )*fS;
    //z = ( pcMatrix.m_fData_[ 1 ]-pcMatrix.m_fData_[ 3 ] )*fS;
    x = ( pcMatrix->m_fData_[ 7 ]-pcMatrix->m_fData_[ 5 ] )*fS;
    y = ( pcMatrix->m_fData_[ 2 ]-pcMatrix->m_fData_[ 6 ] )*fS;
    z = ( pcMatrix->m_fData_[ 3 ]-pcMatrix->m_fData_[ 1 ] )*fS;
  }
  else
  {
    // |w| <= 1/2
    static u32 uiNext[ 3 ] = { 1, 2, 0 };
    
    u32 i = 0;
    
    if ( pcMatrix[ 1 ][ 1 ] > pcMatrix[ 0 ][ 0 ] )
    {
      i = 1;
    }

    if ( pcMatrix[ 2 ][ 2 ] > pcMatrix[ i ][ i ] )
    {
      i = 2;
    }

    u32 j = uiNext[ i ];
    u32 k = uiNext[ j ];

    fS = Math::Sqrt( ( *pcMatrix )[ i ][ i ]-( *pcMatrix )[ j ][ j ]-( *pcMatrix )[ k ][ k ]+1.0f );
    float* apkQuat[ 3 ] = { &x, &y, &z };

    *apkQuat[ i ] = 0.5f*fS;

    fS = 0.5f/fS;

    w = ( ( *pcMatrix )[ k ][ j ]-( *pcMatrix )[ j ][ k ] )*fS;

    *apkQuat[ j ] = ( ( *pcMatrix )[ j ][ i ]+( *pcMatrix )[ i ][ j ] )*fS;
    *apkQuat[ k ] = ( ( *pcMatrix )[ k ][ i ]+( *pcMatrix )[ i ][ k ] )*fS;
  }

  //  // check which element of the main diagonal (3x3) is the biggest
  //  if ( ( pcMatrix.m_fData_[ 0 ] > pcMatrix.m_fData_[ 4 ] ) && ( pcMatrix.m_fData_[ 0 ] > pcMatrix.m_fData_[ 8 ] ) )
  //  { 
  //    fS = Math::Sqrt( 1.0f+pcMatrix.m_fData_[ 0 ]-pcMatrix.m_fData_[ 4 ]-pcMatrix.m_fData_[ 8 ] )*2.0f; 
  //    fInvS = 1.0f/fS;

  //    x = 0.25f*fS;
  //    y = ( pcMatrix.m_fData_[ 1 ]+pcMatrix.m_fData_[ 3 ] )*fInvS;
  //    z = ( pcMatrix.m_fData_[ 2 ]+pcMatrix.m_fData_[ 6 ] )*fInvS;
  //    w = ( pcMatrix.m_fData_[ 5 ]-pcMatrix.m_fData_[ 7 ] )*fInvS;
  //  }
  //  else if ( pcMatrix.m_fData_[ 4 ] > pcMatrix.m_fData_[ 8 ] )
  //  {
  //    fS = Math::Sqrt( 1.0f+pcMatrix.m_fData_[ 4 ]-pcMatrix.m_fData_[ 0 ]-pcMatrix.m_fData_[ 8 ] )*2.0f; 
  //    fInvS = 1.0f/fS;

  //    x = ( pcMatrix.m_fData_[ 3 ]+pcMatrix.m_fData_[ 1 ] )*fInvS;
  //    y = 0.25f*fS;
  //    z = ( pcMatrix.m_fData_[ 7 ]+pcMatrix.m_fData_[ 5 ] )*fInvS;
  //    w = ( pcMatrix.m_fData_[ 6 ]-pcMatrix.m_fData_[ 2 ] )*fInvS;
  //  }
  //  else
  //  {
  //    fS = Math::Sqrt( 1.0f+pcMatrix.m_fData_[ 8 ]-pcMatrix.m_fData_[ 0 ]-pcMatrix.m_fData_[ 4 ] )*2.0f; 
  //    fInvS = 1.0f/fS;

  //    x = ( pcMatrix.m_fData_[ 6 ]+pcMatrix.m_fData_[ 2 ] )*fInvS;
  //    y = ( pcMatrix.m_fData_[ 7 ]+pcMatrix.m_fData_[ 5 ] )*fInvS;
  //    z = 0.25f*fS;
  //    w = ( pcMatrix.m_fData_[ 3 ]-pcMatrix.m_fData_[ 1 ] )*fInvS;
  //  }
  //}

//  int nxt[3] = {5, 10, 0};
//
//  
//	  // diagonal is negative
//    
//	  int i = 0;
//
//      if ( fm_fData_[ 5 ] > fm_fData_[ 0 ] ) i = 5; //m[1][1] > m[0][0]) i = 1;
//	  if (  fm_fData_[ 10 ] > fm_fData_[ 5 ] ) i = 10; //if (m[2][2] > m[i][i]) i = 2;
//    
//	  int j = nxt[i/5];
//      int k = nxt[j/5];
//
//      float s = sqrt ((fm_fData_[ i ] - (fm_fData_[ j ] + fm_fData_[ k ])) + 1.0f);
////      s = sqrt ((m[i][i] - (m[j][j] + m[k][k])) + 1.0);
//
//      float q[4];
//
//	  q[i/5] = s * 0.5f;
//
//      if (s != 0.0f) s = 0.5f / s;
//
//	  q[3]   = (fm_fData_[ k/5*4+j/5 ] - fm_fData_[ j/5*4+k/5 ] ) * s;
//    q[j/5] = (fm_fData_[ j/5*4+i/5 ] - fm_fData_[ i/5*4+j/5 ] )*s;//m[i][j] + m[j][i]) * s;
//    q[k/5] = (fm_fData_[ k/5*4+i/5 ] - fm_fData_[ i/5*4+k/5 ] )*s;//m[i][k] + m[k][i]) * s;
//
//	  //q[3] = (m[j][k] - m[k][j]) * s;
//   //   q[j/5] = (m[i][j] + m[j][i]) * s;
//   //   q[k/5] = (m[i][k] + m[k][i]) * s;
//
//	  x = q[0];
//	  y = q[1];
//	  z = q[2];
//	  w = q[3];

  //
  // here is the shorter code
  //
  //GLfloat  tr, s;
  //GLfloat  q[4];
  //GLint    i, j, k;

  //int nxt[3] = {1, 2, 0};

  //
	 // // diagonal is negative
  //  
	 // i = 0;

  //    if (m[1][1] > m[0][0]) i = 1;
	 // if (m[2][2] > m[i][i]) i = 2;
  //  
	 // j = nxt[i];
  //    k = nxt[j];

  //    s = sqrt ((m[i][i] - (m[j][j] + m[k][k])) + 1.0);
  //    
	 // q[i] = s * 0.5;

  //    if (s != 0.0) s = 0.5 / s;

	 // q[3] = (m[j][k] - m[k][j]) * s;
  //    q[j] = (m[i][j] + m[j][i]) * s;
  //    q[k] = (m[i][k] + m[k][i]) * s;

	 // quat->x = q[0];
	 // quat->y = q[1];
	 // quat->z = q[2];
	 // quat->w = q[3];
  //}
}

void CQuaternion::create( const CVector3* pcAxisX, const CVector3* pcAxisY, const CVector3* pcAxisZ )
{
  assert( pcAxisX != NULL && pcAxisY != NULL && pcAxisZ != NULL && "No required parameter(s)" );

  CMatrix3 cRotationMatrix;

  // compose rotation matrix
  cRotationMatrix[ 0 ][ 0 ] = pcAxisX->x;
  cRotationMatrix[ 1 ][ 0 ] = pcAxisX->y;
  cRotationMatrix[ 2 ][ 0 ] = pcAxisX->z;

  cRotationMatrix[ 0 ][ 1 ] = pcAxisY->x;
  cRotationMatrix[ 1 ][ 1 ] = pcAxisY->y;
  cRotationMatrix[ 2 ][ 1 ] = pcAxisY->z;

  cRotationMatrix[ 0 ][ 2 ] = pcAxisZ->x;
  cRotationMatrix[ 1 ][ 2 ] = pcAxisZ->y;
  cRotationMatrix[ 2 ][ 2 ] = pcAxisZ->z;

  // create from rotation matrix
  create( &cRotationMatrix );
}

void CQuaternion::convertTo( CMatrix3* pcMatrix ) const
{
  assert( pcMatrix != NULL && "No required parameter(s)" );

  // first column
	pcMatrix->m_fData_[ 0 ] = 1.0f-2.0f*( y*y+z*z ); 
	pcMatrix->m_fData_[ 1 ] = 2.0f*( x*y-z*w );
	pcMatrix->m_fData_[ 2 ] = 2.0f*( x*z+y*w );

	// second column
	pcMatrix->m_fData_[ 3 ] = 2.0f*( x*y+z*w );  
	pcMatrix->m_fData_[ 4 ] = 1.0f-2.0f*( x*x+z*z ); 
	pcMatrix->m_fData_[ 5 ] = 2.0f*( z*y-x*w );  

	// third column
	pcMatrix->m_fData_[ 6 ] = 2.0f*( x*z-y*w );
	pcMatrix->m_fData_[ 7 ] = 2.0f*( y*z+x*w );
	pcMatrix->m_fData_[ 8 ] = 1.0f-2.0f*( x*x+y*y );  
}

void CQuaternion::convertTo( CVector3* pcAxis, float& fRadians ) const
{
  assert( pcAxis != NULL && "No required parameter(s)" );

  float fLength;

  // get length of imaginary axes
  fLength = Math::Pow2( x )+Math::Pow2( y )+Math::Pow2( z );

  // check if length is greater than error thershold
  if ( fLength > Math::DELTA )
  {
    // calculate axes
    pcAxis->x = x*( 1.0f/fLength );
    pcAxis->y = y*( 1.0f/fLength );
    pcAxis->z = z*( 1.0f/fLength );

    fRadians = 2.0f*Math::ACos( w );
  }
  else
  {
    // length is 0 or errorous
    pcAxis->x = 0;
    pcAxis->y = 0;
    pcAxis->z = 0;

    fRadians = 0;
  }
}

void CQuaternion::convertTo( CVector3* pcAxes ) const
{
  assert( pcAxes != NULL );

  CMatrix3 cRotationMatrix;

  // convert quternion into rotation matrix
  convertTo( &cRotationMatrix );

  for ( u32 iCol = 0; iCol < 3; iCol++ )
  {
    pcAxes[ iCol ].x = cRotationMatrix[ 0 ][ iCol ];
    pcAxes[ iCol ].y = cRotationMatrix[ 1 ][ iCol ];
    pcAxes[ iCol ].z = cRotationMatrix[ 2 ][ iCol ];
  }
}

float CQuaternion::getMagnitude( void ) const
{
	return Math::Sqrt( getSquaredMagnitude() );
}

float CQuaternion::getSquaredMagnitude( void ) const
{
	return Math::Pow2( x )+Math::Pow2( y )+Math::Pow2( z )+Math::Pow2( w );
}

void CQuaternion::normalize( void )
{
  *this = getNormalized();
}

CQuaternion CQuaternion::getNormalized( void )
{
	CQuaternion cQuaternion( *this );

  float fMagnitude;
	
  // get maginitude
  fMagnitude = cQuaternion.getMagnitude();

  // check if differs from 0
  if ( fMagnitude != 0 )
  {
    // get inverse of magnitude (to speed up calculations)
    fMagnitude = 1.0f/fMagnitude;
  }
  else
  {
    // make it 1
    fMagnitude = 1.0f;
  }

	return cQuaternion*fMagnitude;
}

CQuaternion CQuaternion::performSlerp( const CQuaternion& cDestination, float fTime )
{
  CQuaternion cNew;
  CQuaternion cTemp;

  float fSinOmega;
  float fCosOmega;
  float fOmega;
  float fScale0;
  float fScale1;

  //
  // p     - source quaternion
  // q     - destination quaternion
  // t     - time
  // omega - angle between p and q
  //
  //                    p*sin( ( 1-t )*omega )+q*sin( t*omega )
  // slerp( p, q, t ) = ---------------------------------------
  //                                 sin( omega )
  //

  // calculate cosine omega (dot product of 2 quaternions)
  fCosOmega = getDotProduct( cDestination );

  // adjust the signs
  if ( fCosOmega < 0 )
  {
    cTemp.x = -cDestination.x;
    cTemp.y = -cDestination.y;
    cTemp.z = -cDestination.z;
    cTemp.w = -cDestination.w;
  }
  else
  {
    cTemp.x = cDestination.x;
    cTemp.y = cDestination.y;
    cTemp.z = cDestination.z;
    cTemp.w = cDestination.w;
  }

  // calculate coefficients
  if ( 1.0f-fCosOmega > Math::DELTA )
  {
    // standard case
    fOmega    = Math::ACos( fCosOmega );
    fSinOmega = Math::Sin( fOmega );

    fScale0 = Math::Sin( ( 1.0f-fTime )*fOmega )/fSinOmega;
    fScale1 = Math::Sin( fTime*fOmega )/fSinOmega;
  }
  else
  {
    // source and destination quaternions are very close so we perform lineat interpolation
    fScale0 = 1.0f-fTime;
    fScale1 = fTime;
  }

	// calculate final values
	cNew.x = fScale0*x+fScale1*cTemp.x;
	cNew.y = fScale0*y+fScale1*cTemp.y;
	cNew.z = fScale0*z+fScale1*cTemp.z;
	cNew.w = fScale0*w+fScale1*cTemp.w;

  return cNew;
}

//CQuaternion CQuaternion::performFastSlerp( CQuaternion* pcDestination, float fTime )
//{
//  CQuaternion cNew;
//  CQuaternion cTemp;
//
//  float fCosOmega;
//  float fScale0;
//  float fScale1;
//  float f;
//  float k;
//  float b;
//  float c;
//  float d;
//
//  // calculate cosine omega (dot product of 2 quaternions)
//  fCosOmega = x*pcDestination->x+y*pcDestination->y+z*pcDestination->z+w*pcDestination->w;
//
//  // correct fTime in such a way so the speed near the middle of the sphere (for Lerp is more or less equal to the speed far way from the center)
//  f = 1.0f-0.7878088f*fCosOmega;
//  
//  k = 0.5069269f;
//
//  f *= f;
//  k *= f;
//
//  b = 2*k;
//  c = -3*k;
//  d = 1+k;
//
//  fTime = fTime*( b*fTime+c )+d;
//
//  // adjust the signs
//  if ( fCosOmega < 0 )
//  {
//    cTemp.x = -pcDestination->x;
//    cTemp.y = -pcDestination->y;
//    cTemp.z = -pcDestination->z;
//    cTemp.w = -pcDestination->w;
//  }
//  else
//  {
//    cTemp.x = pcDestination->x;
//    cTemp.y = pcDestination->y;
//    cTemp.z = pcDestination->z;
//    cTemp.w = pcDestination->w;
//  }
//
//  // interpolate linearly
//  fScale0 = 1.0f-fTime;
//  fScale1 = fTime;
//
//	// calculate final values
//	cNew.x = fScale0*x+fScale1*cTemp.x;
//	cNew.y = fScale0*y+fScale1*cTemp.y;
//	cNew.z = fScale0*z+fScale1*cTemp.z;
//	cNew.w = fScale0*w+fScale1*cTemp.w;
//
//  cNew.normalize();
//
//  return cNew;
//}
 
CQuaternion CQuaternion::performLerp( const CQuaternion& cDestination, float fTime )
{
  CQuaternion cNew;
  CQuaternion cTemp;

  float fCosOmega;
  float fScale0;
  float fScale1;

  // calculate cosine omega (dot product of 2 quaternions)
  fCosOmega = getDotProduct( cDestination );

  // adjust the signs
  if ( fCosOmega < 0 )
  {
    cTemp.x = -cDestination.x;
    cTemp.y = -cDestination.y;
    cTemp.z = -cDestination.z;
    cTemp.w = -cDestination.w;
  }
  else
  {
    cTemp.x = cDestination.x;
    cTemp.y = cDestination.y;
    cTemp.z = cDestination.z;
    cTemp.w = cDestination.w;
  }

  // interpolate linearly
  fScale0 = 1.0f-fTime;
  fScale1 = fTime;

	// calculate final values
	cNew.x = fScale0*x+fScale1*cTemp.x;
	cNew.y = fScale0*y+fScale1*cTemp.y;
	cNew.z = fScale0*z+fScale1*cTemp.z;
	cNew.w = fScale0*w+fScale1*cTemp.w;

  cNew.normalize();

  return cNew;
}

void CQuaternion::inverse( void )
{
  *this = getInverse();
}

CQuaternion CQuaternion::getInverse( void ) const
{
  CQuaternion cNew( *this );

  // normalize quaternion
  cNew.normalize();

  // inverse
  cNew.x *= -1.0f;
  cNew.y *= -1.0f;
  cNew.z *= -1.0f;

  return cNew;
}

float CQuaternion::getDotProduct( const CQuaternion& cQuaternion ) const
{
  return x*cQuaternion.x+y*cQuaternion.y+z*cQuaternion.z+w*cQuaternion.w;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATES
