#ifndef EGE_CORE_OVERLAY_H
#define EGE_CORE_OVERLAY_H

#include "EGEString.h"
#include "EGETime.h"
#include "EGEAlignment.h"
#include "EGERenderComponent.h"
#include "Core/Component/Physics/PhysicsComponent.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IRenderer;
class Viewport;
EGE_DECLARE_SMART_CLASS(Overlay, POverlay)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Overlay : public Object
{
  public: 

    Overlay(Application* app, const String& name, egeObjectDeleteFunc deleteFunc = NULL);
    virtual ~Overlay();
    
    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Returns TRUE if object is valid. */
    virtual bool isValid() const;
    /*! Updates overlay. */
    virtual void update(const Time& time);
    /*! Sets alignment. */
    virtual void setAlignment(Alignment align);
    /*! Returns name. */
    const String& name() const { return m_name; }
    /*! Returns physics component. */
    PPhysicsComponent physics() const { return m_physics; }
    /*! Returns TRUE if overlay is visible. */
    bool visible() const { return m_visible; }
    /*! Sets visibility. */
    void setVisible(bool set);
    /*! Returns render component. */
    PRenderComponent renderData() const { return m_renderData; }
    /*! Returns current alignment. */
    const Alignment& alignment() const { return m_alignment; }
    /*! Renders overlay. */
    virtual void addForRendering(IRenderer* renderer, const Matrix4f& transform = Matrix4f::IDENTITY) = 0;
    /*! Sets transparency level. */
    virtual void setAlpha(float32 alpha);

    /*! Returns size. */
    virtual const Vector2f& size() const;
    /*! Sets size. */
    void setSize(const Vector2f& size);

    /*! Returns rectangle containing the overlay. */

    //// position and dimensions related methods
    //float getX( void ) const { return m_fX; }                                                                 // gets X relative position [0,1]
    //float getY( void ) const { return m_fY; }                                                                 // gets Y relative position [0,1]
    //float getWidth( void ) const { return m_fWidth; }                                                         // gets relative width [0,1]
    //float getHeight( void ) const { return m_fHeight; }                                                       // gets relative height [0,1]

    //float getDerivedX( void );                                                                                // gets derived (from own and parent) X position [0,1]
    //float getDerivedY( void );                                                                                // gets derived (from own and parent) Y position [0,1]
    //float getDerivedWidth( void );                                                                            // gets derived (from own and parent) width [0,1]
    //float getDerivedHeight( void );                                                                           // gets derived (from own and parent) height [0,1]

    //void setDimensions( float fX, float fY, float fWidth, float fHeight );                                    // sets relative position and dimensions

    // update related methods
    //inline bool  isUpdateNeeded( void ) const { return m_bNeedUpdate; }                                       // returns TRUE if update is needed
    //virtual void update( void );                                                                              // makes update

    //// color related methods
    //void                 setColor( const CColor& cColor );                                                    // sets color
    //inline const CColor& getColor( void ) const { return m_cColor; }                                          // gets color

    // CRenderable overrides
//    virtual const CMatrix4& getWorldTransformMatrix( void ){ return CMatrix4::IDENTITY; }                     // gets world transformation matrix

  //  virtual void findVisibleObject( CCamera* pcCamera, CViewport* pcViewport, CRenderQueue* pcRenderQueue );

  protected:

    Overlay(Application* app, const String& name, u32 uid, egeObjectDeleteFunc deleteFunc = NULL);
    /*! Invalidates object forcing it to be updated next time it's possible. */
    void invalidate();
    /*! Validates object. */
    void validate();
    /*! Returns TRUE if update is needed. */
    bool isUpdateNeeded() const { return m_updateNeeded; }
    /*! Initializes object. */
    virtual void initialize();

  protected:

    /*! Render component. */
    PRenderComponent m_renderData;

  private:

    /*! Name. */
    String m_name;
    /*! Physics component. */
    PPhysicsComponent m_physics;
    /*! Update flag. */
    bool m_updateNeeded;
    /*! Visibility flag. */
    bool m_visible;
    /*! Alignment. */
    Alignment m_alignment;
    /*! Size. */
    Vector2f m_size;

    //string m_strText;               // text (multipurpose)

    //EType m_eType;                  // type

    //bool m_bNeedDerivedUpdate;      // TRUE if derived data needs to be refreshed

    //float m_fX;                     // relative X position within viewport [0,1]
    //float m_fY;                     // relative Y position within viewport [0,1]
    //float m_fWidth;                 // relative width within viewport [0,1]
    //float m_fHeight;                // relative height within viewport [0,1]
    //float m_fDerivedX;              // cached derived X position within viewport [0,1]
    //float m_fDerivedY;              // cached derived Y position within viewport [0,1]
    //float m_fDerivedWidth;          // cached derived width within viewport [0,1]
    //float m_fDerivedHeight;         // cached derived height within viewport [0,1]

    //CColor m_cColor;                // color


    //// update related methods
    //void updateFromParent( void );
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_OVERLAY_H