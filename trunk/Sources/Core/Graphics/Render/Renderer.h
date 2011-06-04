#ifndef EGE_CORE_RENDERER_H
#define EGE_CORE_RENDERER_H

#include "EGE.h"
#include "Core/Math/Matrix4.h"
#include "Core/Math/Angle.h"
#include "EGEString.h"
#include "Core/Components/Render/RenderComponent.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class SceneNode;

EGE_DECLARE_SMART_CLASS(RenderComponent, PRenderComponent)
EGE_DECLARE_SMART_CLASS(Viewport, PViewport)
EGE_DECLARE_SMART_CLASS(Material, PMaterial)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Renderer : public Object
{
  public:

    Renderer(Application* app);
   ~Renderer();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Sends all geometry through the geometry pipeline to hardware. */
    void flush();
    /* Clears given viewport. */
    void clearViewport(const PViewport& viewport);
    /* Applies material. */
    void applyMaterial(const PMaterial& material);
    /* Sets orientation rotation. */
    void setOrientationRotation(const Angle& angle);
    /*! Returns orientation rotation. */
    const Angle& orientationRotation() const { return m_orientationRotation; }

    bool addForRendering(const Matrix4f& worldMatrix, PRenderComponent& component);

    /* Resets statistics. */
    void resetStats();
    /*! Returns number of batches rendered last frame. */
    inline u32 batchCount() const { return m_batchCount; }
    /*! Returns numberh of triangles rendered last frame. */
    inline u32 triangleCount() const { return m_triangleCount; }

    //PRenderComponent addComponent(const EGEString& name = "");

//    void setViewport(PViewport pViewport);
    void setProjectionMatrix(const Matrix4f& matrix);
    void setViewMatrix(const Matrix4f& matrix);

    // scene manager related methods
//    void setSceneManager( const SceneManager* pcSceneManager );                              // sets scene manager which is in use

    // render related methods
 //   void render( const CRenderOperation& cRenderOp );                                         // renders given operation

    // polygon mode related methods
    //void setPolygonMode( Camera::EPolygonMode eMode );                                       // sets polygon mode

    //void applyRenderableSettings( const CRenderable* pcRenderable );

    //// texture related methods
    //CRenderTexture* createRenderTexture( const string& strName, u32 uiWidth, u32 uiHeight, 
    //                                     CResource::EType eType, CTexture::EFormat eFormat );

  protected:

    struct SRENDERDATA
    {
      PRenderComponent renderComponent;
      
      Matrix4f worldMatrix;
    };

    /*! Orientation rotation angle. */
    Angle m_orientationRotation;

   // const SceneManager* m_pcSceneManager;          // current scene manager

  //  PViewport m_pViewport;                          // current viewport for which rendering is done

    std::multimap<s32, SRENDERDATA> m_renderData;       // render data map <priority, renderdata>

    Matrix4f m_projectionMatrix;                    // projection matrix
    Matrix4f m_viewMatrix;                          // view matrix (camera matrix)

   // std::vector<PRenderComponent> m_components;     // components pool

    u32 m_batchCount;                               // batch count
    u32 m_triangleCount;                            // triangle count

/*
    RenderTarget* m_pcActiveRenderTarget;          // pointer to currently active render target

    u32 m_uiBindedTextureIDs[ MAX_TEXTURE_UNITS ];  // currently binded textures ids

    Matrix4f m_cProjectionMatrix;                   // cached projection matrix
    Matrix4f m_cViewMatrix;                         // cached view matrix
    Matrix4f m_cModelMatrix;                        // cached model matrix

    IRTTManager* m_pcRTTManager;                    // render to texture manager


    // render target related methods
    void setRenderTarget( RenderTarget* pcRenderTarget );                                    // sets render target

    // texture related methods
    void bindTexture( CTexture* pcTexture, u32 uiTextureUnit );                               // binds given texture to texture unit
    void unbindTexture( CTexture* pcTexture, u32 uiTextureUnit );                             // unbinds given texture to texture unit

    // pass related methods
    void applyPassSettings( CRenderPass* pcPass );                                            // applies given pass parameters

    // blending related methods
    void setBlending( CRenderable::EBlendFactor eSrc, CRenderable::EBlendFactor eDst ) const; // sets blending mode

    // scissor box related methods
    void setScissor( const CRect& cRect ) const;                                              // sets scissor box      */              

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(Renderer);
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RENDERER_H