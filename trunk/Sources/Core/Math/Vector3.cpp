#include "EGE.h"

using namespace std;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------------------------------------------------------------

/*
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors

CVector3::CVector3( void ) : x( 0 ), y( 0 ), z( 0 )
{
}

CVector3::CVector3( float fX, float fY, float fZ ) : x( fX ), y( fY ), z( fZ )
{
}

CVector3::CVector3( const CVector3& cVector ) : x( cVector.x ), y( cVector.y ), z( cVector.z )
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Destructors

CVector3::~CVector3( void )
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLICS

CVector3 CVector3::operator-( void ) const
{
  return CVector3( -x, -y, -z );
}

CVector3 CVector3::operator=( const CVector3& cVector )
{
  pack( cVector.x, cVector.y, cVector.z );

	return *this;
}

CVector3 CVector3::operator+( const CVector3& cVector ) const
{
	return CVector3( x+cVector.x, y+cVector.y, z+cVector.z );
}

CVector3 CVector3::operator-( const CVector3& cVector ) const
{
	return CVector3( x-cVector.x, y-cVector.y, z-cVector.z );
}

CVector3 CVector3::operator*( const CVector3& cVector ) const
{
	return CVector3( x*cVector.x, y*cVector.y, z*cVector.z );
}

CVector3 CVector3::operator*( float fNumber ) const
{
	return CVector3( x*fNumber, y*fNumber, z*fNumber );
}

CVector3 CVector3::operator/( float fNumber ) const
{
  assert( fNumber != 0.0f );

  float fInv = 1.0f/fNumber;

	return CVector3( x*fInv, y*fInv, z*fInv );
}

void CVector3::operator+=( const CVector3& cVector )
{
	x += cVector.x;
  y += cVector.y;
  z += cVector.z;
}

void CVector3::operator-=( const CVector3& cVector )
{
	x -= cVector.x;
  y -= cVector.y;
  z -= cVector.z;
}

void CVector3::operator*=( float fNumber )
{
  x *= fNumber;
  y *= fNumber;
  z *= fNumber;
}

void CVector3::operator/=( float fNumber )
{
  assert( fNumber != 0.0f );

  float fInv = 1.0f/fNumber;

  x *= fInv;
  y *= fInv;
  z *= fInv;
}

bool CVector3::operator==( const CVector3& cVector ) const
{
  // check if NOT equal
  if ( x != cVector.x || y != cVector.y || z != cVector.z )
  {
    // not equal
    return false;
  }

  return true;
}

bool CVector3::operator!=( const CVector3& cVector ) const
{
  return !( *this == cVector );
}

CVector3 CVector3::getNormalized( void ) const
{
  float fMagnitude;
	
  // get maginitude
  fMagnitude = getMagnitude();
  
  // check if magnitude is 0
  if ( fMagnitude < Math::DELTA )
  {
    // make sure it is not zero
    fMagnitude = 1;
  }

	return ( *this )/fMagnitude;
}

void CVector3::normalize( void )
{
	float fMagnitude;

  // get magnitude
	fMagnitude = getMagnitude();

  // check if can be done
  if ( fMagnitude >= Math::DELTA )
  {
    // normalize
	  x /= fMagnitude;
	  y /= fMagnitude;
	  z /= fMagnitude;
  }
}

float CVector3::getAngleBetween( const CVector3& cVector ) const
{
  float fMagnitude = getMagnitude()*cVector.getMagnitude();

  // check if magnitude is 0
  if ( fMagnitude == 0 )
  {
    // make sure it is not zero
    fMagnitude = 1;
  }

  return Math::ACos( getDotProduct( cVector )/fMagnitude );
}

CQuaternion CVector3::getRotationTo( const CVector3& cDest, const CVector3& cFallbackAxis ) const
{
  // Based on Stan Melax's article in Game Programming Gems
  CQuaternion q;
 
  // Copy, since cannot modify local
  CVector3 v0 = *this;
  CVector3 v1 = cDest;
  v0.normalize();
  v1.normalize();

  float d = v0.getDotProduct( v1 );

  // If dot == 1, vectors are the same
  if ( d >= 1.0f )
  {
    return CQuaternion::IDENTITY;
  }

  if ( d < Math::DELTA-1.0f )
  {
    // check if fallback axis given
    if ( cFallbackAxis != CVector3::ZERO )
    {
      // rotate 180 degrees about the fallback axis
      q.create( &cFallbackAxis, Math::PI );
    }
    else
    {
      // generate an axis
      CVector3 axis = CVector3::UNIT_X.getCrossProduct( *this );
      
      // check if collinear
      if ( axis.isZeroLength() == true )
      {
        // pick another
	      axis = CVector3::UNIT_Y.getCrossProduct( *this );
      }

      axis.normalize();
      
      q.create( &axis, Math::PI );
    }
  }
  else
  {
    float s = Math::Sqrt( ( 1+d )*2 );
    float invs = 1 / s;

    CVector3 c = v0.getCrossProduct( v1 );

    q.x = c.x*invs;
    q.y = c.y*invs;
	  q.z = c.z*invs;
	  q.w = s*0.5f;
 
    q.normalize();
  }

  return q;
}

CVector3 CVector3::getRandomDeviant( const float& fAngle, const CVector3& cUp ) const
{
  CVector3 cNewUp;

  // check if up vector should be generated
  if ( cUp == CVector3::ZERO )
  {
    // generate an up vector
    cNewUp = getPerpendicular();
  }
  else
  {
    // use provided
    cNewUp = cUp;
  }

  // rotate up vector by random amount around this
  CQuaternion q;
  q.create( this, CRandomGenerator::GetSingletonPtr()->getRandFLOAT()*Math::TWO_PI );
  cNewUp = q.transform( &cNewUp );

  // finally rotate this by given angle around randomised up
  q.create( &cNewUp, fAngle );
  return q.transform( this );
}

CVector3 CVector3::getPerpendicular( void ) const
{
  CVector3 cPerp = getCrossProduct( CVector3::UNIT_X );

  // check length
  if( cPerp.getSquaredMagnitude() < Math::DELTA_SQUARED )
  {
    // vector is the Y axis multiplied by a scalar, so we have to use another axis
    cPerp = getCrossProduct( CVector3::UNIT_Y );
  }

  cPerp.normalize();

  return cPerp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATES
*/