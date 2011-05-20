#include "stdafx.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors

CPlane::CPlane( void ) : m_cNormal( CVector3::ZERO ), m_d( 0 )
{
}

CPlane::CPlane( const CPlane& cPlane )
{
  *this = cPlane; 
}

CPlane::CPlane( const CVector3* pcNormal, float d )
{
  assert( pcNormal != NULL && "No required parameter(s)" );

  m_cNormal = *pcNormal;

  m_d = d;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Destructors

CPlane::~CPlane( void )
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLICS

bool CPlane::operator==( const CPlane& cPlane )
{
  // check if different
  if ( m_d != cPlane.m_d || m_cNormal != cPlane.m_cNormal )
  {
    // different
    return false;
  }

  return true;
}

CPlane& CPlane::operator=( const CPlane& cPlane )
{
  // copy data
  create( cPlane );

  return *this;
}

CPlane::ESide CPlane::getSide( const CVector3* pcPoint ) const
{
  assert( pcPoint != NULL && "No required parameter(s)" );

  // get distance from plane
  float fDistance = getDistance( pcPoint );

  if ( fDistance > 0 )
  {
    return CPlane::POSITIVE_SIDE;
  }
  else if ( fDistance < 0 )
  {
    return CPlane::NEGATIVE_SIDE;
  }

  // on plane
  return CPlane::NO_SIDE;
}

CPlane::ESide CPlane::getSide( const CAxisAlignedBox* pcBox ) const
{
  assert( pcBox != NULL && "No required parameter(s)" );

  if ( pcBox->isNull() == true )
  {
		return NO_SIDE;
  }

	if ( pcBox->isInfinite() == true )
  {
		return BOTH_SIDE;
  }

  return getSide( &pcBox->getCenter(), &pcBox->getHalfSize() );
}

CPlane::ESide CPlane::getSide( const CVector3* pcCentre, const CVector3* pcHalfSize ) const
{
  assert( pcCentre != NULL && pcHalfSize != NULL && "No required parameter(s)" );

  // calculate the distance between box centre and the plane
  float fDist = getDistance( pcCentre );

  // Calculate the maximise allows absolute distance for the distance between box centre and plane
  float fMaxAbsDist = m_cNormal.absDotProduct( *pcHalfSize );

  if ( fDist < -fMaxAbsDist )
  {
    return CPlane::NEGATIVE_SIDE;
  }

  if ( fDist > +fMaxAbsDist )
  {
    return CPlane::POSITIVE_SIDE;
  }

  return CPlane::BOTH_SIDE;
}

float CPlane::getDistance( const CVector3* pcPoint ) const
{
  assert( pcPoint != NULL && "No required parameter(s)" );

  return m_cNormal.getDotProduct( *pcPoint )+m_d;
}

void CPlane::create( const CVector3* pcNormal, const float d )
{
  assert( pcNormal != NULL && "No required parameter(s)" );

  m_cNormal = *pcNormal;
  m_d       = d;
}

void CPlane::create( const CPlane& cPlane )
{
  m_cNormal = cPlane.m_cNormal;
  m_d       = cPlane.m_d;
}

void CPlane::normalize( void )
{
  // get normal magnitude
  float fMagnitude = m_cNormal.getMagnitude();

  // normalize normal
  m_cNormal.normalize();

  // normalize displacement
  m_d /= fMagnitude;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATES
