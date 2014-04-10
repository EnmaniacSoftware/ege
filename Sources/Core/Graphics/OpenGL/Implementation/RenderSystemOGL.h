#ifndef EGE_CORE_GRAPHICS_OPENGL_RENDERSYSTEM_H
#define EGE_CORE_GRAPHICS_OPENGL_RENDERSYSTEM_H

/*! This class is a base class for implementations of OpenGL/OpenGLES rendering systems based on both fixed or pipeline architectures.
 */

#include "EGE.h"
#include "EGEOpenGL.h"
#include "Core/Graphics/Render/RenderSystem.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(VertexBuffer, PVertexBuffer)
EGE_DECLARE_SMART_CLASS(IndexBuffer, PIndexBuffer)
EGE_DECLARE_SMART_CLASS(VertexArrayObject, PVertexArrayObject)
EGE_DECLARE_SMART_CLASS(Texture2D, PTexture2D)
EGE_DECLARE_SMART_CLASS(RenderComponent, PRenderComponent)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RenderSystemOGL : public RenderSystem
{
  public:

    RenderSystemOGL(Application* app);
    virtual ~RenderSystemOGL();

  protected:

    /*! @see RenderSystem::setViewport. */
    void setViewport(const PViewport& viewport) override;
    /*! @see RenderSystem::clearViewport. */
    void clearViewport(const PViewport& viewport) override;
    /*! @see RenderSystem::flush. */
    void flush() override;
    /*! @see RenderSystem::applyGeneralParams. */
    void applyGeneralParams() override;

    /*! @see IHardwareResourceProvider::registerComponent. */
    bool registerComponent(PRenderComponent& component, NVertexBuffer::UsageType vertexUsage, const VertexDeclaration& vertexDeclaration, 
                           EGEIndexBuffer::UsageType indexUsage) override;
    /*! @see IHardwareResourceProvider::createTexture2D. */
    PTexture2D createTexture2D(const String& name, const PImage& image) override;
    /*! @see IHardwareResourceProvider::createTexture2D. */
    PTexture2D createTexture2D(const String& name, const PDataBuffer& data) override;
    /*! @see IHardwareResourceProvider::createRenderTexture. */
    PTexture2D createRenderTexture(const String& name, s32 width, s32 height, PixelFormat format) override;

    /*! Enables/disables blending. */
    void setBlendEnabled(bool set);
    /*! Enables/disables scissor testing. */
    void setScissorTestEnabled(bool set);
    /*! Activates given texture unit at server side. 
     *  @param  unit  Texture unit to activate.
     */
    void activateTextureUnit(u32 unit);
    /*! Binds texture to target. 
     *  @param  target    Texture type.
     *  @param  textureId Texture object id which is to be bound to target.
     */
    void bindTexture(GLenum target, GLuint textureId);
    /*! Creates VAOs and sets them up if necessary. 
     *  @note This is done for currently rendered component.
     */
    void createAndSetupVAOs();
    /*! Maps texture filter into OpenGL equivalent. 
     *  @param  filter      Framework's texture filter value.
     *  @param  mipmapping  Set if filter is to be considered in mip-mapping context.
     *  @return OpenGL equivalent value.
     */
    GLint mapTextureFilter(TextureFilter filter, bool mipmapping) const;
    /*! Maps texture addressing mode into OpenGL equivalent. 
     *  @param  mode  Framework's texture addressing mode value.
     *  @return OpenGL equivalent value.
     */
    GLint mapTextureAddressingMode(TextureAddressingMode mode) const;
    /*! Maps blending factor mode to OpenGL equivalent.
     *  @param  factor  Framework's blending factor value.
     *  @return OpenGL equivalent value;
     */
    GLenum mapBlendFactor(EGEGraphics::BlendFactor factor) const;
    /*! Maps rendering primitive type to OpenGL equivalent.
     *  @param  type  Framework's render primitive type value.
     *  @return OpenGL equivalent value.
     */
    GLenum mapPrimitiveType(EGEGraphics::RenderPrimitiveType type) const;
    /*! Maps render primitive index size to OpenGL equivalent.
     *  @param  size  Framework's render primitive index size value.
     *  @return OpenGL equivalent value.
     */
    GLenum mapIndexSize(EGEIndexBuffer::IndexSize size) const;
    /*! Creates empty 2D texture. 
     *  @param  name  Texture name.
     *  @return Returns created texture. NULL if failed.
     */
    virtual PTexture2D createEmptyTexture(const String& name);
    /*! Creates appripriate vertex buffer object. 
     *  @param  name              Name of the vertex buffer component.
     *  @param  vertexDeclaration Vertex semantics declaration.
     *  @param  usage             Usage hint.
     *  @return On success created component. Otherwise, NULL.
     */
    PVertexBuffer createVertexBuffer(const String& name, const VertexDeclaration& vertexDeclaration, NVertexBuffer::UsageType usage) const;
    /*! Creates appripriate index buffer object. 
     *  @param  name  Name of the index buffer component.
     *  @param  usage Usage hint.
     *  @return On success created component. Otherwise, NULL.
     */
    PIndexBuffer createIndexBuffer(const String& name, EGEIndexBuffer::UsageType usage) const;
    /*! Setup vertex array object for a given pass.
     *  @param  vertexArrayObject Vertex array object to setup.
     *  @param  vertexBuffer      Vertex buffer used for setup.
     *  @param  indexBuffer       Index buffer used for setup.
     *  @param  pass              Render pass for which setup is done.
     */
    virtual void setupVAO(PVertexArrayObject& vertexArrayObject, const PVertexBuffer& vertexBuffer, const PIndexBuffer& indexBuffer, 
                          const PRenderPass& pass) = 0;
    /*! Applies vertex arrays for currently rendered component. */
    virtual void applyVertexArrays() = 0;

  protected:

    /*! Server side, currenty active texture unit. */
    u32 m_activeTextureUnit;
    /*! Blend enabled flag. */
    bool m_blendEnabled;
    /*! Scissor test enabled flag. */
    bool m_scissorTestEnabled;
    /*! Number of active texture units in use (counted from zero). */
    u32 m_activeTextureUnitsCount;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_OPENGL_RENDERSYSTEM_H