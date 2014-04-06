#ifndef EGE_CORE_GRAPHICS_OPENGL3X_RENDERSYSTEM_PRIVATE_H
#define EGE_CORE_GRAPHICS_OPENGL3X_RENDERSYSTEM_PRIVATE_H

/*! This class is a base class for private implementations of OpenGL(ES) rendering systems based on programmable pipeline architecture.
 */

#include "EGE.h"
#include "EGEOpenGL.h"
#include "EGEDynamicArray.h"
#include "Core/Graphics/Render/RenderSystem.h"
#include "Core/Graphics/Render/Implementation/ComponentRenderer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RenderSystem;
class ProgramOGL;

EGE_DECLARE_SMART_CLASS(VertexBuffer, PVertexBuffer)
EGE_DECLARE_SMART_CLASS(IndexBuffer, PIndexBuffer)
EGE_DECLARE_SMART_CLASS(VertexArrayObject, PVertexArrayObject)
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
    /*! Setup vertex array object.
     *  @param  vertexArrayObject Vertex array object to setup.
     *  @param  vertexBuffer      Vertex buffer used for setup.
     *  @param  indexBuffer       Index buffer used for setup.
     *  @param  program           Shader program used for setup.
     */
    void setupVAO(PVertexArrayObject& vertexArrayObject, const PVertexBuffer& vertexBuffer, const PIndexBuffer& indexBuffer, const PProgram& program);
    /*! Creates VAOs and sets them up if necessary. 
     *  @param  component Render component for which to setup VAOs.
     */
    void createAndSetupVAOs(PRenderComponent component);

  protected:

    /*! Sets given viewport. */
    void setViewport(const PViewport& viewport);
    /*! Clears given viewport. */
    void clearViewport(const PViewport& viewport);
    /*! Enables/disables blending. */
    void setBlendEnabled(bool set);
    /*! Enables/disables scissor testing. */
    void setScissorTestEnabled(bool set);
    /*! Sends all geometry through the geometry pipeline to hardware. */
    void flush();
    /*! Activates given texture unit for server side. */
    void activateTextureUnit(u32 unit);
    /*! Binds texture to target. */
    void bindTexture(GLenum target, GLuint textureId);
    /*! Applies general parameters. 
     *  @note General parameters are the ones that require only one setup before component is rendered. 
     */
    void applyGeneralParams(const PRenderComponent& component);
    /*! Applies parameters for given pass. */
    void applyPassParams(const PRenderComponent& component, const PMaterial& material, const RenderPass& pass);
    /*! Applies vertex arrays. 
     *  @param  vertexDeclaration  Vertex buffer which arrays should be applied.
     *  @param  vertexData
     *  @returns List of all vertex attribute indices which were enabled.
     *  @note All arrays are processed except texture ones. These needs to be processed on per render pass basis.
     */
    List<u32> applyVertexArrays(const ProgramOGL* program, const VertexDeclaration& vertexDeclaration, void* vertexData);

    /*! Creates empty 2D texture. 
     *  @param  name  Texture name.
     *  @return Returns created texture. NULL if failed.
     */
    PTexture2D createEmptyTexture(const String& name);

    /*! @see RenderSystem::registerComponent. */
    bool registerComponent(PRenderComponent& component, NVertexBuffer::UsageType vertexUsage, const VertexDeclaration& vertexDeclaration, 
                           EGEIndexBuffer::UsageType indexUsage);
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
    /*! @see RenderSystem::createProgram. */
    PProgram createProgram(const String& name, const List<PShader>& shaders);

  protected:

    /*! Server side, currenty active texture unit. */
    u32 m_activeTextureUnit;
    /*! Blend enabled flag. */
    bool m_blendEnabled;
    /*! Scissor test enabled flag. */
    bool m_scissorTestEnabled;
    /*! */
    u32 m_activeTextureUnitsCount;
    /*! Projection matrix. */
    Matrix4f m_projectionMatrix;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_OPENGL3X_RENDERSYSTEM_PRIVATE_H