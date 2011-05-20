#include "stdafx.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// STATICS

const CVector2 CVector2::ZERO            = CVector2(  0,  0 );
const CVector2 CVector2::UNIT_X          = CVector2(  1,  0 );
const CVector2 CVector2::UNIT_Y          = CVector2(  0,  1 );
const CVector2 CVector2::NEGATIVE_UNIT_X = CVector2( -1,  0 );
const CVector2 CVector2::NEGATIVE_UNIT_Y = CVector2(  0, -1 );
const CVector2 CVector2::ONE             = CVector2(  1, 1 );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors

CVector2::CVector2( void ) : x( 0 ), y( 0 )
{
}

CVector2::CVector2( float fX, float fY ) : x( fX ), y( fY )
{
}

CVector2::CVector2( const CVector2& cVector ) : x( cVector.x ), y( cVector.y )
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Destructors

CVector2::~CVector2( void )
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLICS

CVector2 CVector2::operator=( const CVector2& cVector )
{
  pack( cVector.x, cVector.y );

	return *this;
}

CVector2 CVector2::operator+( const CVector2& cVector ) const
{
	return CVector2( x+cVector.x, y+cVector.y );
}

CVector2 CVector2::operator-( const CVector2& cVector ) const
{
	return CVector2( x-cVector.x, y-cVector.y );
}

CVector2 CVector2::operator*( float fNumber ) const
{
	return CVector2( x*fNumber, y*fNumber );
}

CVector2 CVector2::operator/( float fNumber ) const
{
	return CVector2( x/fNumber, y/fNumber );
}

void CVector2::operator+=( const CVector2& cVector )
{
  x += cVector.x;
  y += cVector.y;
}

void CVector2::operator-=( const CVector2& cVector )
{
  x -= cVector.x;
  y -= cVector.y;
}

bool CVector2::operator==( const CVector2& cVector ) const
{
  return x == cVector.x && y == cVector.y;
}

bool CVector2::operator!=( const CVector2& cVector ) const
{
  return !( operator==( cVector ) );
}

CVector2 CVector2::getNormalized( void )
{
  float fMagnitude;
	
  // get maginitude
  fMagnitude = getMagnitude();
  
  // check magnitude is 0
  if ( fMagnitude == 0 )
  {
    // make it non zero
    fMagnitude = 1;
  }

	return ( *this )/fMagnitude;
}

void CVector2::normalize( void )
{
  *this = getNormalized();
}

float CVector2::getMagnitude( void ) const
{
  return Math::Sqrt( getSquaredMagnitude() ); 
}

float CVector2::getSquaredMagnitude( void ) const
{
  return Math::Pow2( x )+Math::Pow2( y );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATES