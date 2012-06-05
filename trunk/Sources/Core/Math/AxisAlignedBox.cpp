#include "Core/Math/AxisAlignedBox.h"
#include "Core/Math/Vector4.h"
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AxisAlignedBox::AxisAlignedBox() : m_minimum(Vector4f::ZERO), 
                                   m_maximum(Vector4f::ZERO), 
                                   m_extent(EXTENT_NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AxisAlignedBox::AxisAlignedBox(const AxisAlignedBox& cBox)
{
//  *this = cBox; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AxisAlignedBox::AxisAlignedBox(const Vector4f& cMin, const Vector4f& cMax)
{
  setExtents(cMin, cMax);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AxisAlignedBox::~AxisAlignedBox()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AxisAlignedBox::setExtents(const Vector4f& min, const Vector4f& max)
{
  EGE_ASSERT(min.x <= max.x && min.y <= max.y && min.z <= max.z);

  m_extent = EXTENT_FINITE;
  m_minimum = min;
  m_maximum = max;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// PUBLICS
//
//AxisAlignedBox& AxisAlignedBox::operator=( const AxisAlignedBox& cBox )
//{
//  // Specifically override to avoid copying m_pcCorners
//  if ( cBox.isNull() == true )
//  {
//	  setNull();
//  }
//  else if ( cBox.isInfinite() == true )
//  {
//	  setInfinite();
//  }
//  else
//  {
//	  setExtents( &cBox.m_cMinimum, &cBox.m_cMaximum );
//  }
//
//  return *this;
//}
//
//bool AxisAlignedBox::operator==( const AxisAlignedBox& cBox ) const
//{
//  // check trivial cases
//  if ( m_eExtent != cBox.m_eExtent )
//  {
//    return false;
//  }
//
//  if ( isFinite() == false )
//  {
//    return true;
//  }
//
//  return this->m_cMinimum == cBox.m_cMinimum && this->m_cMaximum == cBox.m_cMaximum;
//}
//
//void AxisAlignedBox::merge( const AxisAlignedBox& cBox )
//{
//  // do nothing if rhs null, or this is infinite
//  if ( ( cBox.m_eExtent == EXTENT_NULL ) || ( m_eExtent == EXTENT_INFINITE ) )
//  {
//    // do nothing
//	  return;
//  }
//  else if ( cBox.m_eExtent == EXTENT_INFINITE )
//  {
//	  // if rhs is infinite, make this infinite, too
//	  m_eExtent = EXTENT_INFINITE;
//  }
//  else if ( m_eExtent == EXTENT_NULL )
//  {
//	  // otherwise if current null, just take rhs
//	  setExtents( &cBox.m_cMinimum, &cBox.m_cMaximum );
//  }
//  else
//  {
//	  // otherwise merge
//	  CVector3 cMin = m_cMinimum;
//	  CVector3 cMax = m_cMaximum;
//		
//    cMax.makeCeil( cBox.m_cMaximum );
//	  cMin.makeFloor( cBox.m_cMinimum );
//
//	  setExtents( &cMin, &cMax );
//  }
//}
//
//void AxisAlignedBox::merge( const CVector3* pcPoint )
//{
//  assert( pcPoint != NULL && "No required parameter(s)" );
//
//  // process according to current extent
//  switch( m_eExtent )
//  {
//    case EXTENT_NULL:
//
//      // use point
//      setExtents( pcPoint, pcPoint );
//	    return;
//
//    case EXTENT_FINITE:
//
//	    m_cMaximum.makeCeil( *pcPoint );
//	    m_cMinimum.makeFloor( *pcPoint );
//	    return;
//
//    case EXTENT_INFINITE:
//		  
//      // no need to do anything
//      return;
//  }
//
//  assert( false && "Never reached" );
//}
//
//void AxisAlignedBox::transform( const CMatrix4* pcMatrix )
//{
//  assert( pcMatrix != NULL && "No required parameter(s)" );
//
//  // NOTE: By calling this method you get the axis-aligned box which surrounds the transformed version of this box. Therefore each
//  //       corner of the box is transformed by the matrix, then the extents are mapped back onto the axes to produce another
//  //       AABB. Useful when you have a local AABB for an object which	is then transformed.
//
//  // do nothing if current null or infinite
//  if ( m_eExtent != EXTENT_FINITE )
//  {
//    // do nothing
//	  return;
//  }
//
//  CVector3 oldMin;
//  CVector3 oldMax;
//  CVector3 currentCorner;
//
//  // getting the old values so that we can use the existing merge method.
//  oldMin = m_cMinimum;
//  oldMax = m_cMaximum;
//
//  // We sequentially compute the corners in the following order:
//  // 0, 6, 5, 1, 2, 4 ,7 , 3
//  // This sequence allows us to only change one member at a time to get at all corners
//
//  // For each one, we transform it using the matrix
//  // Which gives the resulting point and merge the resulting point.
//
//  // min min min
//  currentCorner = oldMin;
//  merge( &pcMatrix->transform( &currentCorner ) );
//
//  // min min max
//  currentCorner.z = oldMax.z;
//  merge( &pcMatrix->transform( &currentCorner ) );
//
//  // min max max
//  currentCorner.y = oldMax.y;
//  merge( &pcMatrix->transform( &currentCorner ) );
//
//  // min max min
//  currentCorner.z = oldMin.z;
//  merge( &pcMatrix->transform( &currentCorner ) );
//
//  // max max min
//  currentCorner.x = oldMax.x;
//  merge( &pcMatrix->transform( &currentCorner ) );
//
//  // max max max
//  currentCorner.z = oldMax.z;
//  merge( &pcMatrix->transform( &currentCorner ) );
//
//  // max min max
//  currentCorner.y = oldMin.y;
//  merge( &pcMatrix->transform( &currentCorner ) );
//
//  // max min min
//  currentCorner.z = oldMin.z;
//  merge( &pcMatrix->transform( &currentCorner ) ); 
//}
//
//void AxisAlignedBox::transformAffine( const CMatrix4* pcMatrix )
//{
//  assert( pcMatrix != NULL && "No required parameter(s)" );
//	assert( pcMatrix->isAffine() );
//
//  // NOTE: By calling this method you get the axis-aligned box which surrounds the transformed version of this box. Therefore each
//  //       corner of the box is transformed by the matrix, then the extents are mapped back onto the axes to produce another
//  //       AABB. Useful when you have a local AABB for an object which is then transformed
//
//	// do nothing if current null or infinite
//	if ( m_eExtent != EXTENT_FINITE )
//  {
//		return;
//  }
//
//  // get AAB data
//	CVector3 cCentre   = getCenter();
//	CVector3 cHalfSize = getHalfSize();
//
//	CVector3 cNewCentre = pcMatrix->transformAffine( &cCentre );
//
//	CVector3 cNewHalfSize( Math::FastAbs( ( *pcMatrix )[ 0 ][ 0 ] )*cHalfSize.x+Math::FastAbs( ( *pcMatrix )[ 0 ][ 1 ] )*cHalfSize.y+Math::FastAbs( ( *pcMatrix )[ 0 ][ 2 ] )*cHalfSize.z, 
//		                     Math::FastAbs( ( *pcMatrix )[ 1 ][ 0 ] )*cHalfSize.x+Math::FastAbs( ( *pcMatrix )[ 1 ][ 1 ] )*cHalfSize.y+Math::FastAbs( ( *pcMatrix )[ 1 ][ 2 ] )*cHalfSize.z,
//		                     Math::FastAbs( ( *pcMatrix )[ 2 ][ 0 ] )*cHalfSize.x+Math::FastAbs( ( *pcMatrix )[ 2 ][ 1 ] )*cHalfSize.y+Math::FastAbs( ( *pcMatrix )[ 2 ][ 2 ] )*cHalfSize.z );
//
//	setExtents( &( cNewCentre-cNewHalfSize ), &( cNewCentre+cNewHalfSize ) );
//}
//
//void AxisAlignedBox::transformAffine( const CMatrix3* pcMatrix )
//{
//  assert( pcMatrix != NULL && "No required parameter(s)" );
//
//  // NOTE: By calling this method you get the axis-aligned box which surrounds the transformed version of this box. Therefore each
//  //       corner of the box is transformed by the matrix, then the extents are mapped back onto the axes to produce another
//  //       AABB. Useful when you have a local AABB for an object which is then transformed
//
//	// do nothing if current null or infinite
//	if ( m_eExtent != EXTENT_FINITE )
//  {
//		return;
//  }
//
//  // get AAB data
//	CVector3 cCentre   = getCenter();
//	CVector3 cHalfSize = getHalfSize();
//
//	CVector3 cNewCentre = pcMatrix->transform( &cCentre );
//
//	CVector3 cNewHalfSize( Math::FastAbs( ( *pcMatrix )[ 0 ][ 0 ] )*cHalfSize.x+Math::FastAbs( ( *pcMatrix )[ 0 ][ 1 ] )*cHalfSize.y+Math::FastAbs( ( *pcMatrix )[ 0 ][ 2 ] )*cHalfSize.z, 
//		                     Math::FastAbs( ( *pcMatrix )[ 1 ][ 0 ] )*cHalfSize.x+Math::FastAbs( ( *pcMatrix )[ 1 ][ 1 ] )*cHalfSize.y+Math::FastAbs( ( *pcMatrix )[ 1 ][ 2 ] )*cHalfSize.z,
//		                     Math::FastAbs( ( *pcMatrix )[ 2 ][ 0 ] )*cHalfSize.x+Math::FastAbs( ( *pcMatrix )[ 2 ][ 1 ] )*cHalfSize.y+Math::FastAbs( ( *pcMatrix )[ 2 ][ 2 ] )*cHalfSize.z );
//
//	setExtents( &( cNewCentre-cNewHalfSize ), &( cNewCentre+cNewHalfSize ) );
//}
//
//AxisAlignedBox AxisAlignedBox::getIntersection( const AxisAlignedBox& cBox ) const
//{
//  // check boundary condition cases
//  if ( isNull() == true || cBox.isNull() == true )
//  {
//	  return AxisAlignedBox();
//  }
//  else if ( isInfinite() == true )
//  {
//	  return cBox;
//  }
//  else if ( cBox.isInfinite() == true )
//  {
//	  return *this;
//  }
//
//  CVector3 cMin = m_cMinimum;
//  CVector3 cMax = m_cMaximum;
//
//  cMin.makeCeil( *cBox.getMinimum() );
//  cMax.makeFloor( *cBox.getMaximum() );
//
//  // check intersection isn't null
//  if ( cMin.x < cMax.x && cMin.y < cMax.y && cMin.z < cMax.z )
//  {
//    return AxisAlignedBox( &cMin, &cMax );
//  }
//
//  return AxisAlignedBox();
//}
//
//void AxisAlignedBox::move( const CVector3* pcVector )
//{
//  assert( pcVector != NULL && "No required parameter(s)" );
//
//  m_cMinimum += *pcVector;
//  m_cMaximum += *pcVector;
//}
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// PRIVATES

EGE_NAMESPACE_END