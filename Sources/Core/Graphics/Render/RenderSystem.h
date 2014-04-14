#ifndef EGE_CORE_GRAPHICS_RENDER_RENDERSYSTEM_H
#define EGE_CORE_GRAPHICS_RENDER_RENDERSYSTEM_H

#include "EGE.h"
#include "EGEString.h"
#include "EGEMatrix.h"
#include "EGEMutex.h"
#include "EGEObjectList.h"
#include "EGEList.h"
#include "EGEMap.h"
#include "EGERenderComponent.h"
#include "EGEComponent.h"
#include "Core/Graphics/HardwareResourceProvider.h"
#include "Core/Graphics/Render/Interface/Renderer.h"
#include "Core/Graphics/Render/Implementation/ComponentRenderer.h"
#include "Core/Event/EventListener.h"

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
class RenderSystem : public Object
                   , public ComponentHost
                   , public IRenderer
                   , public IComponentRenderer
                   , public IHardwareResourceProvider
                   , public IEventListener
{
  public:

    RenderSystem(Application* app);
    virtual ~RenderSystem();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Available states. */
    enum State
    {
      STATE_NONE = -1,      /*!< Uninitialized. */
      STATE_READY,          /*!< Ready to use. */
      STATE_CLOSING,        /*!< Closing. */
      STATE_CLOSED          /*!< Closed. */
    };

  public:

    /*! Creates object. */
    EGEResult construct();
    /*! Returns current state. */
    State state() const;
    /*! Updates object. */
    void update();

    /*! Sends all geometry through the geometry pipeline to hardware. */
    virtual void flush() = 0;
    /*! Clears given viewport. 
     *  @param  viewport  Viewport to clear.
     */
    virtual void clearViewport(const PViewport& viewport) = 0;
    /*! Sets given viewport. 
     *  @param  viewport  Viewport to set.
     */
    virtual void setViewport(const PViewport& viewport);

    /*! Sets texture minifying function filter. 
     *  @param  filter  Texture minifying filter to set.
     */
    void setTextureMinFilter(TextureFilter filter);
    /*! Sets texture magnification function filter. 
     *  @param  filter  Texture magnification filter to set.
     */
    void setTextureMagFilter(TextureFilter filter);
    /*! Sets texture addressing mode for S texture coordinate. 
     *  @param  mode  Texture addressing mode for S coordinate.
     */
    void setTextureAddressingModeS(TextureAddressingMode mode);
    /*! Sets texture addressing mode for T texture coordinate. 
     *  @param  mode  Texture addressing mode for T coordinate.
     */
    void setTextureAddressingModeT(TextureAddressingMode mode);
    /*! Enables/disables texture mipmapping for newly created textures. 
     *  @param  set If set, enables mipmapping.
     */
    void setTextureMipMapping(bool set);

  protected:

    /*! @see IRenderer::currentRenderTarget. */
    PRenderTarget currentRenderTarget() const override;
    
    /*! Updates rectangle coordinates by given angle. 
     *  @param  rect  Rectangle to be rotated.
     *  @param  angle Angle by which to rotate rectangle.
     *  @return Rotated rectangle.
     *  @note This only rotates by multiples of 90degs.
     */
    Rectf applyRotation(const Rectf& rect, const Angle& angle) const;
    /*! Sets render component being currently processed.
     *  @param  component Render component being processed at the moment.
     */
    void setActiveRenderComponent(const PRenderComponent& component);
    /*! Returns render component currently being processed. */
    const PRenderComponent& activeRenderComponent() const;
    /*! Applies pass specific parameters.
     *  @param  pass      Render pass currently being processed.
     */
    virtual void applyPassParams(const PRenderPass& pass) = 0;
    /*! Applies generic parameters for component currently being rendered. */
    virtual void applyGeneralParams() = 0;

  protected:

    /*! Map of render queue lists sorted by render priority. */
    typedef Map<s32, List<PRenderQueue> > RenderQueueMap;

  protected:

    /*! Projection matrix. */
    Matrix4f m_projectionMatrix;
    /*! View matrix. */
    Matrix4f m_viewMatrix;
    /*! Map of list of rendering queues sorted by hash value. */
    RenderQueueMap m_renderQueues;            
    /*! Texture minifying function filter. */
    TextureFilter m_textureMinFilter;
    /*! Texture magnification function filter. */
    TextureFilter m_textureMagFilter;
    /*! Texture addressing mode for S texture coordinate. */
    TextureAddressingMode m_textureAddressingModeS;
    /*! Texture addressing mode for T texture coordinate. */
    TextureAddressingMode m_textureAddressingModeT;
    /*! Texture mip mapping flag. */
    bool m_textureMipMapping;

  private:

    /*! Available request types. */
    enum RequestType
    {
      REQUEST_CREATE_TEXTURE_2D = 0,
      REQUEST_DESTROY_TEXTURE_2D,
      REQUEST_CREATE_SHADER,
      REQUEST_DESTROY_SHADER,
      REQUEST_CREATE_PROGRAM,
      REQUEST_DESTROY_PROGRAM
    };

    /*! Resource request data struct. */
    struct RequestData
    {
      u32 id;                                             /*!< Assigned request id. */
      RequestType type;                                   /*!< Request type. */
      String name;                                        /*!< Name associated with request. May be empty. */

      ObjectList objects;                                 /*!< List of object associated with request. May be NULL. */

      TextureFilter textureMinFilter;                     /*!< Texture minifying function filter. */
      TextureFilter textureMagFilter;                     /*!< Texture magnification function filter. */
      TextureAddressingMode textureAddressingModeS;       /*!< Texture addressing mode for S texture coordinate. */
      TextureAddressingMode textureAddressingModeT;       /*!< Texture addressing mode for T texture coordinate. */
      bool textureMipMapping;                             /*!< Texture mip mapping flag. */

      EGEGraphics::ShaderType shaderType;                 /*!< Shader type. */
    };

    typedef List<RequestData> RequestDataList;

  private:

    /*! @see IRenderer::addForRendering. */
    bool addForRendering(const PRenderComponent& component, const Matrix4f& worldMatrix = Matrix4f::IDENTITY) override;
    /*! @see IRenderer::addForRendering. */
    bool addForRendering(const PRenderQueue& queue) override;
    /*! @see IRenderer::setViewMatrix. */
    void setViewMatrix(const Matrix4f& matrix) override;
    /*! @see IRenderer::viewMatrix. */
    const Matrix4f& viewMatrix() const override;
    /*! @see IRenderer::setProjectionMatrix. */
    void setProjectionMatrix(const Matrix4f& matrix) override;

    /*! @see IHardwareResourceProvider::requestCreateTexture2D. */
    u32 requestCreateTexture2D(const String& name, const PImage& image) override;
    /*! @see IHardwareResourceProvider::requestDestroyTexture2D. */
    u32 requestDestroyTexture2D(PTexture2D texture) override;
    /*! @see IHardwareResourceProvider::requestCreateShader. */
    u32 requestCreateShader(EGEGraphics::ShaderType type, const String& name, const PDataBuffer& data) override;
    /*! @see IHardwareResourceProvider::requestDestroyShader. */
    u32 requestDestroyShader(PShader shader) override;
    /*! @see IHardwareResourceProvider::requestCreateProgram. */
    u32 requestCreateProgram(const String& name, const List<PShader>& shaders) override;
    /*! @see IHardwareResourceProvider::requestDestroyProgram. */
    u32 requestDestroyProgram(PProgram program) override;

    /*! @see IEventListener::onEventRecieved. */
    void onEventRecieved(PEvent event) override;
 
  private:

    /*! Current state. */
    State m_state;
    /*! Currently active render target. */
    PRenderTarget m_renderTarget;
    /*! Next available request ID. */
    u32 m_nextRequestID;
    /*! List of pending requests. */
    RequestDataList m_requests;
    /*! Request data list mutex. */
    PMutex m_requestsMutex;
    /*! Render component being processed. */
    PRenderComponent m_renderComponent;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_RENDER_RENDERSYSTEM_H