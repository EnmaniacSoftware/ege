#ifndef EGE_CORE_GRAPHICS_OPENGL_RENDERSYSTEMFIXEDOGL_H
#define EGE_CORE_GRAPHICS_OPENGL_RENDERSYSTEMFIXEDOGL_H

/*! OpenGL/OpenGLES implementation based on fixed pipeline.
 */

#include "EGE.h"
#include "EGEDynamicArray.h"
#include "Core/Graphics/OpenGL/Implementation/RenderSystemOGL.h"
#include "Core/Graphics/OpenGL/Implementation/Fixed/TextureUnitState.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(VertexBuffer, PVertexBuffer)
EGE_DECLARE_SMART_CLASS(IndexBuffer, PIndexBuffer)
EGE_DECLARE_SMART_CLASS(VertexArrayObject, PVertexArrayObject)
EGE_DECLARE_SMART_CLASS(Texture2D, PTexture2D)
EGE_DECLARE_SMART_CLASS(Shader, PShader)
EGE_DECLARE_SMART_CLASS(Program, PProgram)
EGE_DECLARE_SMART_CLASS(RenderPass, PRenderPass)
EGE_DECLARE_SMART_CLASS(RenderComponent, PRenderComponent)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RenderSystemFixedOGL : public RenderSystemOGL
{
  public:

    RenderSystemFixedOGL(Application* app);
   ~RenderSystemFixedOGL();

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
    /*! @see RenderSystem::applyGeneralParams. */
    void applyGeneralParams() override;

    /*! @see RenderSystemOGL::setupVAO. */
    void setupVAO(PVertexArrayObject& vertexArrayObject, const PVertexBuffer& vertexBuffer, const PIndexBuffer& indexBuffer, const PRenderPass& pass) override;
    /*! @see RenderSystemOGL::applyVertexArrays. */
    void applyVertexArrays() override;

    /*! Enables/disables given client state. */
    void setClientStateEnabled(u32 state, bool set);
    /*! Sets current matrix mode. */
    void setMatrixMode(s32 mode);
    /*! Activates given texture unit at client side. 
     *  @param  unit  Texture unit to activate.
     */
    void activateClientTextureUnit(u32 unit);

  private:

    /*! Client side, currenty active texture unit. */
    u32 m_activeClientTextureUnit;
    /*! Current matrix mode. */
    s32 m_matrixMode;
    /*! Array of texture unit states. */
    DynamicArray<TextureUnitState> m_textureUnitStates;
    /*! Active client states. */
    DynamicArray<u32> m_activeClientStates;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_OPENGL_RENDERSYSTEMFIXEDOGL_H