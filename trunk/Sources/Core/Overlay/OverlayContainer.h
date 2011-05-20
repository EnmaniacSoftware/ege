#pragma once

// classes
class _E3DExport COverlayContainer : public COverlay
{
  public: 

    COverlayContainer( COverlay* pcParent, const string& strName );
    virtual ~COverlayContainer( void );

    static COverlayContainer* New( COverlay* pcParent, const string& strName );

    // overlays related methods
    COverlay* addOverlay( COverlay::EType eType, const string& strName );           // adds overlay, returns NULL if failed
    void      removeOverlay( COverlay::EType eType, const string& strName );        // removes given overlay
    void      removeAllOverlays( void );                                            // removes all overlays
    COverlay* getOverlay( COverlay::EType eType, const string& strName ) const;     // gets given overlay, NULL if not found

    // COverlay overrides
    virtual void findVisibleObject( CCamera* pcCamera, CViewport* pcViewport, 
                                    CRenderQueue* pcRenderQueue );
    virtual void invalidate( void );                                                // invalidates overlay

  private:

    typedef vector<COverlay*> OverlaysVector;
    OverlaysVector m_vpcOverlays;              // overlays pool
};
