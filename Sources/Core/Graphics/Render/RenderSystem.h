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
#include "Core/Base/Interface/IUpdateable.h"
#include "Core/Engine/Interface/EngineModule.h"
#include "Core/Graphics/HardwareResourceProvider.h"
#include "Core/Graphics/Render/Interface/Renderer.h"
#include "Core/Graphics/Render/Interface/IRenderSystem.h"
#include "Core/Graphics/Render/Implementation/ComponentRenderer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SceneNode;
class RenderPass;

EGE_PREDECLARE_SMART_CLASS(RenderComponent, PRenderComponent)
EGE_PREDECLARE_SMART_CLASS(Viewport, PViewport)
EGE_PREDECLARE_SMART_CLASS(Material, PMaterial)
EGE_PREDECLARE_SMART_CLASS(RenderQueue, PRenderQueue)
EGE_PREDECLARE_SMART_CLASS(RenderTarget, PRenderTarget)
EGE_PREDECLARE_SMART_CLASS(DataBuffer, PDataBuffer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RenderSystem : public ComponentHost
                   , public IRenderer
                   , public IComponentRenderer
                   , public IHardwareResourceProvider
                   , public IUpdateable
                   , public IRenderSystem
{
  public:

    RenderSystem(Engine& engine);
    virtual ~RenderSystem();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! @see EngineModule::construct. */
    EGEResult construct();
    /*! Returns current render target. */
    PRenderTarget currentRenderTarget() const;

  protected:

    /*! @see IRenderSystem::setViewport. */
    void setViewport(const PViewport& viewport) override;

    /*! Returns engine object. */
    Engine& engine() const;
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
      RequestType type;                                   /*!< Request type. */
      String name;                                        /*!< Name associated with request. May be empty. */

      ObjectList objects;                                 /*!< List of object associated with request. May be NULL. */

      TextureFilter textureMinFilter;                     /*!< Texture minifying function filter. */
      TextureFilter textureMagFilter;                     /*!< Texture magnification function filter. */
      TextureAddressingMode textureAddressingModeS;       /*!< Texture addressing mode for S texture coordinate. */
      TextureAddressingMode textureAddressingModeT;       /*!< Texture addressing mode for T texture coordinate. */
      bool textureMipMapping;                             /*!< Texture mip mapping flag. */

      EGEGraphics::ShaderType shaderType;                 /*!< Shader type. */
      
      HardwareResourceProviderSlot callbackSlot;          /*!< Callback slot. */
    };

    typedef List<RequestData> RequestDataList;

  private:

    /*! @see IRenderer::addForRendering. */
    EGEResult addForRendering(const PRenderComponent& component, const Matrix4f& worldMatrix = Matrix4f::IDENTITY) override;
    /*! @see IRenderer::addForRendering. */
    EGEResult addForRendering(const PRenderQueue& queue) override;

    /*! @see IHardwareResourceProvider::requestCreateTexture2D. */
    bool requestCreateTexture2D(const String& name, const PImage& image, const HardwareResourceProviderSlot& slot = HardwareResourceProviderSlot()) override;
    /*! @see IHardwareResourceProvider::requestDestroyTexture2D. */
    bool requestDestroyTexture2D(PTexture2D texture, const HardwareResourceProviderSlot& slot = HardwareResourceProviderSlot()) override;
    /*! @see IHardwareResourceProvider::requestCreateShader. */
    bool requestCreateShader(EGEGraphics::ShaderType type, const String& name, const PDataBuffer& data, const HardwareResourceProviderSlot& slot) override;
    /*! @see IHardwareResourceProvider::requestDestroyShader. */
    bool requestDestroyShader(PShader shader, const HardwareResourceProviderSlot& slot = HardwareResourceProviderSlot()) override;
    /*! @see IHardwareResourceProvider::requestCreateProgram. */
    bool requestCreateProgram(const String& name, const List<PShader>& shaders,
                              const HardwareResourceProviderSlot& slot = HardwareResourceProviderSlot()) override;
    /*! @see IHardwareResourceProvider::requestDestroyProgram. */
    bool requestDestroyProgram(PProgram program, const HardwareResourceProviderSlot& slot = HardwareResourceProviderSlot()) override;

    /*! @see IUpdateable::update. */
    void update(const Time& time) override;

    /*! @see IRenderSystem::setViewMatrix. */
    void setViewMatrix(const Matrix4f& matrix) override;
    /*! @see IRenderSystem::viewMatrix. */
    const Matrix4f& viewMatrix() const override;
    /*! @see IRenderSystem::setProjectionMatrix. */
    void setProjectionMatrix(const Matrix4f& matrix) override;
    /*! @see IRenderSystem::setTextureMinFilter. */
    void setTextureMinFilter(TextureFilter filter) override;
    /*! @see IRenderSystem::setTextureMagFilter. */
    void setTextureMagFilter(TextureFilter filter) override;
    /*! @see IRenderSystem::setTextureAddressingModeS. */
    void setTextureAddressingModeS(TextureAddressingMode mode) override;
    /*! @see IRenderSystem::setTextureAddressingModeT. */
    void setTextureAddressingModeT(TextureAddressingMode mode) override;
    /*! @see IRenderSystem::setTextureMipMapping. */
    void setTextureMipMapping(bool set) override;

  private:

    /*! Reference to engine. */
    Engine& m_engine;
    /*! Currently active render target. */
    PRenderTarget m_renderTarget;
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