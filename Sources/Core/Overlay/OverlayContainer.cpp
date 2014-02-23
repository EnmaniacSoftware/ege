#include "stdafx.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors

COverlayContainer::COverlayContainer( COverlay* pcParent, const string& strName ) 
: COverlay( pcParent, COverlay::CONTAINER, strName )
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Destructors

COverlayContainer::~COverlayContainer( void )
{
  // remove overlays
  removeAllOverlays();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLICS

COverlayContainer* COverlayContainer::New( COverlay* pcParent, const string& strName )
{
  COverlayContainer* pcObject;

  // allocate object
  if ( ( pcObject = new COverlayContainer( pcParent, strName ) ) == NULL )
  {
    // error!
    DELETE_PTR( pcObject );
    return NULL;
  }

  return pcObject;
}

COverlay* COverlayContainer::addOverlay( COverlay::EType eType, const string& strName )
{
  COverlay* pcOverlay = NULL;

  // check if already present
  if ( getOverlay( eType, strName ) != NULL )
  {
    // error!
    return NULL;
  }

  // create according to type
  switch( eType )
  {
    case COverlay::TEXT:      pcOverlay = CTextOverlay::New( this, strName ); break;
    case COverlay::CONTAINER: pcOverlay = COverlayContainer::New( this, strName ); break;
  }

  // check if succeeded
  if ( pcOverlay != NULL )
  {
    // add into vector
    m_vpcOverlays.push_back( pcOverlay );
  }

  return pcOverlay;
}

void COverlayContainer::removeOverlay( COverlay::EType eType, const string& strName )
{
  // go thru all overlays
  for ( OverlaysVector::iterator iter = m_vpcOverlays.begin(); iter != m_vpcOverlays.end(); ++iter )
  {
    // check if found
    if ( ( *iter )->getType() == eType && ( *iter )->getName() == strName )
    {
      // found
      m_vpcOverlays.erase( iter );
      return;
    }
  }
}

void COverlayContainer::removeAllOverlays( void )
{
  DELETE_VECTOR( OverlaysVector, m_vpcOverlays );
}

COverlay* COverlayContainer::getOverlay( COverlay::EType eType, const string& strName ) const
{
  // go thru all overlays
  for ( OverlaysVector::const_iterator iter = m_vpcOverlays.begin(); iter != m_vpcOverlays.end(); ++iter )
  {
    // check if found
    if ( ( *iter )->getType() == eType && ( *iter )->getName() == strName )
    {
      // found
      return *iter;
    }
  }

  // not found
  return NULL;
}

void COverlayContainer::findVisibleObject( CCamera* pcCamera, CViewport* pcViewport, CRenderQueue* pcRenderQueue )
{
  // check if NOT visible
  if ( m_bVisible == false )
  {
    // we are done
    return;
  }

  // go thru all overlays
  for ( OverlaysVector::iterator iter = m_vpcOverlays.begin(); iter != m_vpcOverlays.end(); ++iter )
  {
    COverlay* pcOverlay = *iter;

    // check if visible
    if ( pcOverlay->isVisible() == true )
    {
      // check if update is needed
      if ( pcOverlay->isUpdateNeeded() == true )
      {
        // update
        pcOverlay->update();
      }

      // check if CONTAINER
      if ( pcOverlay->getType() == COverlay::CONTAINER )
      {
        // find visible overlays within container
        pcOverlay->findVisibleObject( pcCamera, pcViewport, pcRenderQueue );
      }
      else
      {
        // add into render queue
        pcRenderQueue->addRenderable( pcOverlay, CRenderQueueGroup::OVERLAY );
      }
    }
  }
}

void COverlayContainer::invalidate( void )
{
  // go thru all overlays
  for ( OverlaysVector::iterator iter = m_vpcOverlays.begin(); iter != m_vpcOverlays.end(); ++iter )
  {
    // invalidate
    ( *iter )->invalidate();
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATES
