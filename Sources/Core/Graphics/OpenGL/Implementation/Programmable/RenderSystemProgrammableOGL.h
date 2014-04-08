#ifndef EGE_CORE_GRAPHICS_OPENGL_RENDERSYSTEMPROGRAMMABLEOGL_H
#define EGE_CORE_GRAPHICS_OPENGL_RENDERSYSTEMPROGRAMMABLEOGL_H

/*! OpenGL/OpenGLES implementation based on programmable pipeline.
 */

#include "EGE.h"
#include "EGEList.h"
#include "Core/Graphics/OpenGL/Implementation/RenderSystemOGL.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ProgramOGL;

EGE_DECLARE_SMART_CLASS(VertexBuffer, PVertexBuffer)
EGE_DECLARE_SMART_CLASS(IndexBuffer, PIndexBuffer)
EGE_DECLARE_SMART_CLASS(VertexArrayObject, PVertexArrayObject)
EGE_DECLARE_SMART_CLASS(Texture2D, PTexture2D)
EGE_DECLARE_SMART_CLASS(Shader, PShader)
EGE_DECLARE_SMART_CLASS(Program, PProgram)
EGE_DECLARE_SMART_CLASS(RenderPass, PRenderPass)
EGE_DECLARE_SMART_CLASS(RenderComponent, PRenderComponent)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RenderSystemProgrammableOGL : public RenderSystemOGL
{
  public:

    RenderSystemProgrammableOGL(Application* app);
    virtual ~RenderSystemProgrammableOGL();

  private:

    /*! @see IComponentRenderer::renderComponent. */
    void renderComponent(const PRenderComponent& component, const Matrix4f& modelMatrix) override;
    /*! @see IComponentRenderer::createShader. */
    PShader createShader(EGEGraphics::ShaderType type, const String& name, const PDataBuffer& data) override;
    /*! @see IComponentRenderer::createProgram. */
    PProgram createProgram(const String& name, const List<PShader>& shaders) override;

    /*! @see RenderSystem::applyPassParams. */
    void applyPassParams(const PRenderPass& pass) override;
    /*! @see RenderSystem::flush. */
    void flush() override;

    /*! @see RenderSystemOGL::setupVAO. */
    void setupVAO(PVertexArrayObject& vertexArrayObject, const PVertexBuffer& vertexBuffer, const PIndexBuffer& indexBuffer, const PRenderPass& pass) override;
    /*! @see RenderSystemOGL::applyVertexArrays. */
    void applyVertexArrays() override;

  private:

    /*! Final projection matrix i.e. after rotations etc. */
    Matrix4f m_finalProjectionMatrix;
    /*! List of enabled vertex attribute arrays indices. */
    List<s32> m_enabledVertexAttributeIndices;
    /*! Pointer to active shader program. */
    ProgramOGL* m_activeProgram;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_OPENGL_RENDERSYSTEMPROGRAMMABLEOGL_H