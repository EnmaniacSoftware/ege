#ifndef EGE_CORE_GRAPHICS_RENDER_RENDERSYSTEM_H
#define EGE_CORE_GRAPHICS_RENDER_RENDERSYSTEM_H

#include <EGE.h>
#include <EGEString.h>
#include <EGEMatrix.h>
#include "Core/Graphics/Render/Renderer.h"
#include "Core/Components/Render/RenderComponent.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SceneNode;
class RenderPass;

EGE_DECLARE_SMART_CLASS(RenderComponent, PRenderComponent)
EGE_DECLARE_SMART_CLASS(Viewport, PViewport)
EGE_DECLARE_SMART_CLASS(Material, PMaterial)
EGE_DECLARE_SMART_CLASS(RenderQueue, PRenderQueue)
EGE_DECLARE_SMART_CLASS(RenderTarget, PRenderTarget)
EGE_DECLARE_SMART_CLASS(DataBuffer, PDataBuffer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RenderSystem : public Object, public IRenderer
{
  public:

    RenderSystem(Application* app);
   ~RenderSystem();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Creates object. */
    EGEResult construct();
    /*! Sends all geometry through the geometry pipeline to hardware. */
    void flush();
    /*! Sets given viewport. */
    void setViewport(const PViewport& viewport);
    /*! Clears given viewport. */
    void clearViewport(const PViewport& viewport);
    /*! Applies material for given pass. */
    void applyMaterial(const PMaterial& material, const RenderPass* pass);
    /*! @see IRenderer::addForRendering. */
    bool addForRendering(const PRenderComponent& component, const Matrix4f& worldMatrix = Matrix4f::IDENTITY) override;

    /*! Resets statistics. */
    void resetStats();
    /*! Returns number of batches rendered last frame. */
    u32 batchCount() const { return m_batchCount; }
    /*! Returns number of vertices rendered last frame. */
    u32 vertexCount() const { return m_vertexCount; }

    /*! Returns current render target. */
    PRenderTarget currentRenderTarget() const { return m_renderTarget; }

    //PRenderComponent addComponent(const String& name = "");

//    void setViewport(PViewport pViewport);
    /*! Sets projection matrix. */
    void setProjectionMatrix(const Matrix4f& matrix);
    /*! Sets view matrix. */
    void setViewMatrix(const Matrix4f& matrix);

    // TAGE - IRenderer, update comments etc
    /*! Returns view matrix. */
    const Matrix4f& viewMatrix() const { return m_viewMatrix; }

    /*! Creates vertex buffer obejct. */
    PVertexBuffer createVertexBuffer(EGEVertexBuffer::UsageType usage) const;
    /*! Creates index buffer obejct. */
    PIndexBuffer createIndexBuffer(EGEIndexBuffer::UsageType usage) const;
    /*! Creates 2D texture from given image. 
     *  @param  name  Name of the texture.
     *  @param  image Image data for texture.
     */
    PTexture2D createTexture2D(const String& name, const PImage& image);
    /*! Creates 2D texture from given data. 
     *  @param  name  Name of the texture.
     *  @param  image Image data for texture.
     */
    PTexture2D createTexture2D(const String& name, const PDataBuffer& data);

    /*! Sets texture minifying function filter. */
    void setTextureMinFilter(EGETexture::Filter filter);
    /*! Sets texture magnification function filter. */
    void setTextureMagFilter(EGETexture::Filter filter);
    /*! Sets texture addressing mode for S texture coordinate. */
    void setTextureAddressingModeS(EGETexture::AddressingMode mode);
    /*! Sets texture addressing mode for T texture coordinate. */
    void setTextureAddressingModeT(EGETexture::AddressingMode mode);

    // scene manager related method
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

   // const SceneManager* m_pcSceneManager;          // current scene manager

  //  PViewport m_pViewport;                          // current viewport for which rendering is done

    /*! Projection matrix. */
    Matrix4f m_projectionMatrix;
    /*! View matrix. */
    Matrix4f m_viewMatrix;
    /*! Number of batches rendered. */
    u32 m_batchCount;
    /*! Number of vertices rendered. */
    u32 m_vertexCount;
    /*! Map of rendering queues sorted by render priority. */
    Map<s32, PRenderQueue> m_renderQueues;
    /*! Texture minifying function filter. */
    EGETexture::Filter m_textureMinFilter;
    /*! Texture magnification function filter. */
    EGETexture::Filter m_textureMagFilter;
    /*! Texture addressing mode for S texture coordinate. */
    EGETexture::AddressingMode m_textureAddressingModeS;
    /*! Texture addressing mode for T texture coordinate. */
    EGETexture::AddressingMode m_textureAddressingModeT;


   // std::vector<PRenderComponent> m_components;     // components pool

/*

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
    void setBlending( CRenderable::BlendFactor eSrc, CRenderable::BlendFactor eDst ) const; // sets blending mode

    // scissor box related methods
    void setScissor( const CRect& cRect ) const;                                              // sets scissor box      */              

  private:

    /*! Updates rectangle coordinates by given angle. */
    Rectf applyRotation(const Rectf& rect, const Angle& angle) const;

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(RenderSystem);

    /*! Currently active render target. */
    PRenderTarget m_renderTarget;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_RENDER_RENDERSYSTEM_H