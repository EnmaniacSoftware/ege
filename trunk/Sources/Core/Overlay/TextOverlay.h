#ifndef EGE_CORE_TEXTOVERLAY_H
#define EGE_CORE_TEXTOVERLAY_H

#include <EGETime.h>
#include <EGEString.h>
#include "Core/Overlay/Overlay.h"
#include "Core/Graphics/Font.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(TextOverlay, PTextOverlay)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class TextOverlay : public Overlay
{
  public: 

    TextOverlay(Application* app, const String& name, egeObjectDeleteFunc deleteFunc = NULL);
    virtual ~TextOverlay();
    
    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Sets text. */
    void setText(const Text& text);
    /*! Returns text. */
    const Text& text() const { return m_text; }
    /* Sets font. */
    void setFont(PFont font);
    /*! Returns font. */
    inline PFont font() const { return m_font; }
    /* Overlay override. Sets alignment. */
    virtual void setAlignment(Alignment align) override;
    /* Returns text size (in pixels). */
    Vector2f textSize();
    /* Overlay override. Updates overlay. */
    void update(const Time& time) override;

    // visiblility related methods
    //inline bool isVisible( void ) const { return m_bVisible; }                                                // returns TRUE if overlay is visible
    //void        show( bool bShow ){ m_bVisible = bShow; }                                                     // shows/hides the overlay

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

    //// color related methods
    //void                 setColor( const CColor& cColor );                                                    // sets color
    //inline const CColor& getColor( void ) const { return m_cColor; }                                          // gets color

    // CRenderable overrides
//    virtual const CMatrix4& getWorldTransformMatrix( void ){ return CMatrix4::IDENTITY; }                     // gets world transformation matrix

  //  virtual void findVisibleObject( CCamera* pcCamera, CViewport* pcViewport, CRenderQueue* pcRenderQueue );

  private:

    /* Overlay override. Renders element. */
    void render(const Viewport* viewport, Renderer* renderer) override;
    /* Overlay override. Initializes object. */
    void initialize() override;
    /* Updates render data. */
    void updateRenderData();

  private slots:

    /* Slot called when physics data has been updated. */
    void transformationChanged();

  private:

    /*! Overlay text. */
    Text m_text;
    /*! Font. */
    PFont m_font;
    /*! Aligment position offset. */
    Vector4f m_alignmentOffset;
    /*! Cached text size. */
    Vector2f m_textSize;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_TEXTOVERLAY_H
