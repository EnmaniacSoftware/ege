#ifndef EGE_CORE_GRAPHICS_OPENGL_FIXED_RENDERSYSTEMFIXED_PRIVATE_H
#define EGE_CORE_GRAPHICS_OPENGL_FIXED_RENDERSYSTEMFIXED_PRIVATE_H

/*! This class is a base class for private implementations of OpenGL(ES) rendering systems based on fixed pipeline architecture.
 */

#include "EGE.h"
#include "EGEOpenGL.h"
#include "EGEDynamicArray.h"
#include "Core/Graphics/Render/RenderSystem.h"
#include "Core/Graphics/Render/Implementation/ComponentRenderer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RenderSystem;

EGE_DECLARE_SMART_CLASS(VertexBuffer, PVertexBuffer)
EGE_DECLARE_SMART_CLASS(IndexBuffer, PIndexBuffer)
EGE_DECLARE_SMART_CLASS(Texture2D, PTexture2D)
EGE_DECLARE_SMART_CLASS(Shader, PShader)
EGE_DECLARE_SMART_CLASS(Program, PProgram)
EGE_DECLARE_SMART_CLASS(RenderComponent, PRenderComponent)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RenderSystemPrivate : public IComponentRenderer
{
  public:

    RenderSystemPrivate(RenderSystem* base);
    virtual ~RenderSystemPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(RenderSystem)

  private:

    /*! @see IComponentRenderer::renderComponent. */
    void renderComponent(const PRenderComponent& component, const Matrix4f& modelMatrix) override;

  protected:
 
    /*! Sets given viewport. */
    void setViewport(const PViewport& viewport);
    /*! Clears given viewport. */
    void clearViewport(const PViewport& viewport);
    /*! Sends all geometry through the geometry pipeline to hardware. */
    void flush();
    /*! Activates given texture unit. */
    void activateTextureUnit(u32 unit);
    /*! Binds texture to target. */
    void bindTexture(GLenum target, GLuint textureId);
    /*! Applies general parameters. 
     *  @note General parameters are the ones that require only one setup before component is rendered. 
     */
    void applyGeneralParams(const PRenderComponent& component);
    /*! Applies parameters for given pass. */
    void applyPassParams(const PRenderComponent& component, const PMaterial& material, const RenderPass* pass);

    /*! Binds given vertex buffer.
     *  @param buffer  Vertex buffer to bind.
     *  @return Returns base value pointing to begining of buffer data.
     */
    void* bindVertexBuffer(PVertexBuffer& buffer) const;
    /*! Unbinds given vertex buffer. */
    void unbindVertexBuffer(PVertexBuffer& buffer) const;
    /*! Binds given index buffer.
     *  @param buffer  Index buffer to bind.
     *  @return Returns base value pointing to begining of buffer data.
     */
    void* bindIndexBuffer(PIndexBuffer& buffer) const;
    /*! Unbinds given index buffer. */
    void unbindIndexBuffer(PIndexBuffer& buffer) const;

    /*! Creates empty 2D texture. 
     *  @param  name  Texture name.
     *  @return Returns created texture. NULL if failed.
     */
    PTexture2D createEmptyTexture(const String& name);
    /*! @see RenderSystem::createVertexBuffer. */
    PVertexBuffer createVertexBuffer(EGEVertexBuffer::UsageType usage) const;
    /*! @see RenderSystem::destroyVertexBuffer. */
    void destroyVertexBuffer(PVertexBuffer object) const;
    /*! @see RenderSystem::createIndexBuffer. */
    PIndexBuffer createIndexBuffer(EGEIndexBuffer::UsageType usage) const;
    /*! @see RenderSystem::destroyIndexBuffer. */
    void destroyIndexBuffer(PIndexBuffer object) const;
    /*! @see RenderSystem::createTexture2D. */
    PTexture2D createTexture2D(const String& name, const PImage& image);
    /*! @see RenderSystem::createTexture2D. */
    PTexture2D createTexture2D(const String& name, const PDataBuffer& data);
    /*! @see RenderSystem::createRenderTexture. */
    PTexture2D createRenderTexture(const String& name, s32 width, s32 height, PixelFormat format);
    /*! @see RenderSystem::destroyTexture2D. */
    void destroyTexture2D(PTexture2D texture);
    /*! @see RenderSystem::createShader. */
    PShader createShader(EGEGraphics::ShaderType type, const String& name, const PDataBuffer& data);
    /*! @see RenderSystem::destroyShader. */
    void destroyShader(PShader shader);
    /*! @see RenderSystem::createProgram. */
    PProgram createProgram(const String& name, const List<PShader>& shaders);
    /*! @see RenderSystem::destroyProgram. */
    void destroyProgram(PProgram program);

  protected:

    /*! Texture unit currently active. */
    u32 m_activeTextureUnit;
    /*! Pool of all currently active texture units. */
    DynamicArray<u32> m_activeTextureUnits;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_OPENGL_FIXED_RENDERSYSTEMFIXED_PRIVATE_H