#include "stdafx.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors

const CVector4 CVector4::ZERO            = CVector4( 0, 0, 0, 0 );
const CVector4 CVector4::UNIT_X          = CVector4( 1, 0, 0, 0 );
const CVector4 CVector4::UNIT_Y          = CVector4( 0, 1, 0, 0 );
const CVector4 CVector4::UNIT_Z          = CVector4( 0, 0, 1, 0 );
const CVector4 CVector4::UNIT_W          = CVector4( 0, 0, 0, 1 );
const CVector4 CVector4::NEGATIVE_UNIT_X = CVector4( -1, 0, 0, 0 );
const CVector4 CVector4::NEGATIVE_UNIT_Y = CVector4( 0, -1, 0, 0 );
const CVector4 CVector4::NEGATIVE_UNIT_Z = CVector4( 0, 0, -1, 0 );
const CVector4 CVector4::NEGATIVE_UNIT_W = CVector4( 0, 0, 0, -1 );
const CVector4 CVector4::ONE             = CVector4( 1, 1, 1, 1 );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors

CVector4::CVector4( void ) : x( 0 ), y( 0 ), z( 0 ), w( 0 )
{
}

CVector4::CVector4( float fX, float fY, float fZ, float fW ) : x( fX ), y( fY ), z( fZ ), w( fW )
{
}

CVector4::CVector4( const CVector4& cVector ) : x( cVector.x ), y( cVector.y ), z( cVector.z ), w( cVector.w )
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Destructors

CVector4::~CVector4( void )
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// OPERATORS

CVector4 CVector4::operator=( const CVector4& cVector )
{
  pack( cVector.x, cVector.y, cVector.z, cVector.w );

  return *this;
}

CVector4 CVector4::operator+( const CVector4& cVector )
{
	return CVector4( x+cVector.x, y+cVector.y, z+cVector.z,  w+cVector.w );
}

CVector4 CVector4::operator-( const CVector4& cVector )
{
	return CVector4( x-cVector.x, y-cVector.y, z-cVector.z,  w-cVector.w );
}

CVector4 CVector4::operator*( float fNumber )
{
	return CVector4( x*fNumber, y*fNumber, z*fNumber, w*fNumber );
}

CVector4 CVector4::operator/( float fNumber )
{
	return CVector4( x/fNumber, y/fNumber, z/fNumber, w/fNumber );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLICS

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATES