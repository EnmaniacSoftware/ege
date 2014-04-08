#include "Core/Graphics/OpenGL/Implementation/Programmable/RenderSystemProgrammableOGL.h"
#include "Core/Graphics/OpenGL/Implementation/VertexArrayObject.h"
#include "Core/Graphics/OpenGL/Texture2DOGL.h"
#include "Core/Graphics/OpenGL/ShaderOGL.h"
#include "Core/Graphics/OpenGL/ProgramOGL.h"
#include "EGEDevice.h"
#include "EGEDebug.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const String KUniformModelViewPerspectiveName = "u_mvpMatrix";
static const String KUniformTexture2DXName           = "u_texture2D%1";
static const String KUniformTextureMatrixXName       = "u_texMatrix%1";

static const String KAttributeVertexName        = "a_vertex";
static const String KAttributeNormalName        = "a_normal";
static const String KAttributeColorName         = "a_color";
static const String KAttributeTextureCoordXName = "a_texCoord%1";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderSystemProgrammableOGL::RenderSystemProgrammableOGL(Application* app) : RenderSystemOGL(app)
                                                                           , m_activeProgram(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderSystemProgrammableOGL::~RenderSystemProgrammableOGL()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemProgrammableOGL::flush()
{
  PRenderTarget renderTarget = currentRenderTarget();
  EGE_ASSERT(NULL != renderTarget);

  // prepare projection matrix
  m_finalProjectionMatrix = m_projectionMatrix;

  // check if not auto-rotated
  if ( ! renderTarget->isAutoRotated())
  {
    // apply rotation
    // NOTE: actual OGLES rotation should be opposite
    Quaternionf rotation = Math::CreateQuaternion(Vector3f(0, 0, 1), -renderTarget->orientationRotation());

    Matrix4f matrix;
    Math::Convert(matrix, rotation);

    m_finalProjectionMatrix *= matrix;
  }

  // call base class
  RenderSystemOGL::flush();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemProgrammableOGL::applyPassParams(const PRenderPass& pass)
{
  // TAGE - port
  //Color color = pass.diffuseColorTransformation().transform(pass.diffuseColor());
  //glColor4f(color.red, color.green, color.blue, color.alpha);
  //OGL_CHECK()

  // go thru all textures
  for (u32 i = 0; i < pass->textureCount(); ++i)
  {
    PTextureImage textureImage = pass->texture(i);
    EGE_ASSERT(EGE_OBJECT_UID_TEXTURE_IMAGE == textureImage->uid());

    // check if texture image
    if (EGE_OBJECT_UID_TEXTURE_IMAGE == textureImage->uid())
    {
      PTexture2D tex2d = textureImage->texture();

      activateTextureUnit(i);

      // NOTE: it is possible texure object might be not present ie when it is manual and hasnt been set yet
      if (NULL != tex2d)
      {
        bindTexture(GL_TEXTURE_2D, tex2d->p_func()->id());
      }

      // assign appropriate texture sampler to texture unit
      GLint location = m_activeProgram->uniformLocation(KUniformTexture2DXName.arg(i));
      EGE_ASSERT(0 <= location);

      glUniform1i(location, i);
      OGL_CHECK()

      // supply texture matrix if needed
      location = m_activeProgram->uniformLocation(KUniformTextureMatrixXName.arg(i));
      EGE_ASSERT((0 <= location) || (0 > location) && (0 == textureImage->rotationAngle().degrees()) || (0 > location) && (Rectf::UNIT != textureImage->rect()));

      if (0 <= location)
      {
        Matrix4f matrix = Matrix4f::IDENTITY;

        // check if any rotation
        if (0.0f != textureImage->rotationAngle().degrees())
        {
          matrix.setTranslation(0.5f, 0.5f, 0);
          Math::Convert(matrix, Math::CreateQuaternion(Vector3f(0, 0, 1), textureImage->rotationAngle()));
          matrix *= Matrix4f(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -0.5f, -0.5f, 0, 1);
        }

        // apply texture position and scaling
        matrix *= Matrix4f(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, textureImage->rect().x, textureImage->rect().y, 0, 1);
        matrix *= Matrix4f(textureImage->rect().width, 0, 0, 0, 0, textureImage->rect().height, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

        // supply to shader
        glUniformMatrix4fv(location, 1, GL_FALSE, matrix.data);
        OGL_CHECK()

        // TAGE - TESTING
        //glMatrixMode(GL_TEXTURE);
        //glLoadIdentity();

        //float32 degrees = textureImage->rotationAngle().degrees();

        //if (0.0f != degrees)
        //{
        //  glTranslatef(textureImage->rect().x + textureImage->rect().width * 0.5f, textureImage->rect().y + textureImage->rect().height * 0.5f, 0.0f);
        //  glRotatef(degrees, 0.0f, 0.0f, 1.0f);
        //  glTranslatef(-(textureImage->rect().x + textureImage->rect().width * 0.5f), -(textureImage->rect().y + textureImage->rect().height * 0.5f), 0.0f);
        //}

        //float32 mat[16];
        //glGetFloatv(GL_TEXTURE_MATRIX, mat);

        //int a = 1;
      }
    }
  }
  
  // disable rest texture units
  m_activeTextureUnitsCount = pass->textureCount();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PShader RenderSystemProgrammableOGL::createShader(EGEGraphics::ShaderType type, const String& name, const PDataBuffer& data)
{
  // check if NOT supported
  if (((EGEGraphics::VERTEX_SHADER == type) && ! Device::HasRenderCapability(EGEDevice::RENDER_CAPS_VERTEX_SHADER)) ||
      ((EGEGraphics::FRAGMENT_SHADER == type) && ! Device::HasRenderCapability(EGEDevice::RENDER_CAPS_FRAGMENT_SHADER)) ||
      (EGEGraphics::UNKNOWN_SHADER == type))
  {
    // error!
    return NULL;
  }

  // create shader
  ShaderOGL* shader = ege_new ShaderOGL(app(), name, type, this);
  if ((NULL == shader) || ! shader->isValid())
  {
    // error!
    EGE_DELETE(shader);
  }
  else
  {
    // create it from data source
    if (EGE_SUCCESS != shader->create(data))
    {
      // error!
      EGE_DELETE(shader);
    }
  }

  return shader;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PProgram RenderSystemProgrammableOGL::createProgram(const String& name, const List<PShader>& shaders)
{
  // create shader program
  ProgramOGL* program = ege_new ProgramOGL(app(), name, this);
  if ((NULL == program) || ! program->isValid())
  {
    // error!
    EGE_DELETE(program);
  }
  else
  {
    // attach shaders
    for (List<PShader>::const_iterator it = shaders.begin(); it != shaders.end(); ++it)
    {
      const PShader& shader = *it;

      if ( ! program->attach(shader))
      {
        // error!
        EGE_DELETE(program);
        break;
      }
    }

    // link
    if ((NULL != program) && ! program->link())
    {
      // error!
      EGE_DELETE(program);
    }
  }

  return program;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemProgrammableOGL::renderComponent(const PRenderComponent& component, const Matrix4f& modelMatrix)
{
  // set component being rendered
  setActiveRenderComponent(component);

  PVertexBuffer vertexBuffer = component->vertexBuffer();
  PIndexBuffer indexBuffer   = component->indexBuffer();
  PMaterial material         = component->material();

  EGE_ASSERT_X(NULL != material, "Component must have material attached.");
  EGE_ASSERT_X(0 < vertexBuffer->vertexCount(), "No vertices to render!");

  // make sure VAOs are created and set up
  createAndSetupVAOs();

  // get list of vaos
  // NOTE: each VAO in the list corresponds to single pass
  List<PComponent> vaos = component->components(EGE_OBJECT_UID_VERTEX_ARRAY_OBJECT);

  // check if no VAOs present
  if (vaos.empty())
  {
    // bind vertex and index buffers
    // NOTE: this is valid for all passes
    vertexBuffer->bind();
    indexBuffer->bind();
  }

  // apply general params
  applyGeneralParams();

  // go thru all passes
  for (u32 pass = 0; pass < material->passCount(); ++pass)
  {
    const PRenderPass renderPass = material->pass(pass);

    // TAGE - move it to apply pass params!
    m_activeProgram = ege_cast<ProgramOGL*>(renderPass->program());
    EGE_ASSERT(NULL != m_activeProgram);

    // get first vao from the list and shrink the list
    PVertexArrayObject vao = vaos.first(NULL);
    if ( ! vaos.empty())
    {
      vaos.pop_front();
    }

    // bind
    m_activeProgram->bind();

    // set model-view matrix
    GLint location = m_activeProgram->uniformLocation(KUniformModelViewPerspectiveName);
    EGE_ASSERT(0 <= location);

    glUniformMatrix4fv(location, 1, GL_FALSE, m_viewMatrix.multiply(modelMatrix).multiply(m_finalProjectionMatrix).data);
    OGL_CHECK()

    // apply pass related params
    applyPassParams(renderPass);

    // apply vertex arrays
    if (NULL == vao)
    {
      applyVertexArrays();
    }
    else
    {
      // bind vao
      vao->bind();
    }

    u32 value = (0 < indexBuffer->indexCount()) ? indexBuffer->indexCount() : vertexBuffer->vertexCount();

    m_vertexCount += value;
    m_batchCount++;

    // check if INDICIES are to be used
    if (0 < indexBuffer->indexCount())
    {
      // render only if there is anything to render
      glDrawElements(mapPrimitiveType(component->primitiveType()), indexBuffer->indexCount(), mapIndexSize(indexBuffer->size()), indexBuffer->offset());
      OGL_CHECK();

      // update engine info
      //ENGINE_INFO(drawElementsCalls++);
    }
    else
    {
      // render only if there is anything to render
      glDrawArrays(mapPrimitiveType(component->primitiveType()), 0, vertexBuffer->vertexCount());
      OGL_CHECK()

      // update engine info
      //ENGINE_INFO(drawArraysCalls++);
    }

    if (NULL != vao)
    {
      // unbind vao
      vao->unbind();
    }

    // unbind program
    m_activeProgram->unbind();

    // clean up
    for (s32 i = m_activeTextureUnitsCount - 1; 0 <= i; --i)
    {
      // disable texturing on server side
      activateTextureUnit(i);
      glDisable(GL_TEXTURE_2D);
      OGL_CHECK()
    }
  }

  if (vaos.empty())
  {
    // unbind vertex and index buffers
    vertexBuffer->unbind();
    indexBuffer->unbind();
  }

  // reset component being rendered
  setActiveRenderComponent(NULL);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemProgrammableOGL::applyVertexArrays()
{
  const PRenderComponent& component = activeRenderComponent();

  EGE_ASSERT(NULL != component);
  EGE_ASSERT(NULL != component->vertexBuffer());

  const PVertexBuffer& vertexBuffer = component->vertexBuffer();

  const VertexDeclaration& vertexDeclaration = vertexBuffer->vertexDeclaration();
  s8* vertexData = reinterpret_cast<s8*>(vertexBuffer->offset());

  // next texture coordinate index
  s32 textureCoordIndex = 0;

  // go thru all arrays
  const VertexElementArray& vertexElements = vertexDeclaration.vertexElements();
  for (VertexElementArray::const_iterator itElement = vertexElements.begin(); itElement != vertexElements.end(); ++itElement)
  {
    GLint location;

    // set according to buffer type
    switch (itElement->semantic())
    {
      case NVertexBuffer::VES_POSITION_XYZ:

        location = m_activeProgram->attributeLocation(KAttributeVertexName);
        EGE_ASSERT(0 <= location);

        glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, vertexDeclaration.vertexSize(), static_cast<s8*>(vertexData) + itElement->offset());
        OGL_CHECK()
        break;

      case NVertexBuffer::VES_POSITION_XY:

        location = m_activeProgram->attributeLocation(KAttributeVertexName);
        EGE_ASSERT(0 <= location);

        glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, vertexDeclaration.vertexSize(), static_cast<s8*>(vertexData) + itElement->offset());
        OGL_CHECK()
        break;

      case NVertexBuffer::VES_NORMAL:

        location = m_activeProgram->attributeLocation(KAttributeNormalName);
        EGE_ASSERT(0 <= location);

        glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, vertexDeclaration.vertexSize(), static_cast<s8*>(vertexData) + itElement->offset());
        OGL_CHECK()
        break;

      case NVertexBuffer::VES_COLOR_RGBA:

        location = m_activeProgram->attributeLocation(KAttributeColorName);
        EGE_ASSERT(0 <= location);

        glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, vertexDeclaration.vertexSize(), static_cast<s8*>(vertexData) + itElement->offset());
        OGL_CHECK()
        break;

      case NVertexBuffer::VES_TEXTURE_UV:

        location = m_activeProgram->attributeLocation(KAttributeTextureCoordXName.arg(textureCoordIndex));
        EGE_ASSERT(0 <= location);

        glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, vertexDeclaration.vertexSize(), static_cast<s8*>(vertexData) + itElement->offset());
        OGL_CHECK()
        break;

      default:

        // nothing to enable
        location = -1;
        break;
    }

    // check if any vertex attribute needs enabling
    if (0 <= location)
    {
      // enable
      glEnableVertexAttribArray(location);  
      OGL_CHECK()

      // store for further use
      m_enabledVertexAttributeIndices << location;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemProgrammableOGL::setupVAO(PVertexArrayObject& vertexArrayObject, const PVertexBuffer& vertexBuffer, const PIndexBuffer& indexBuffer,
                                           const PRenderPass& pass)
{
  // bind VAO
  // NOTE: Currently, all vertex states are stored within VAO rather than in OpenGL state machine, so there is no need to unbind anything 
  vertexArrayObject->bind();

  // bind VBO
  vertexBuffer->bind();

  // next texture coordinate index
  s32 textureCoordIndex = 0;

  // go thru all arrays
  const VertexElementArray& vertexElements = vertexBuffer->vertexDeclaration().vertexElements();
  for (VertexElementArray::const_iterator itElement = vertexElements.begin(); itElement != vertexElements.end(); ++itElement)
  {
    GLint location;

    // set according to buffer type
    switch (itElement->semantic())
    {
      case NVertexBuffer::VES_POSITION_XYZ:

        location = m_activeProgram->attributeLocation(KAttributeVertexName);
        EGE_ASSERT(0 <= location);

        glEnableVertexAttribArray(location);
        OGL_CHECK()
        glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, vertexBuffer->vertexDeclaration().vertexSize(), 
                              static_cast<s8*>(vertexBuffer->offset()) + itElement->offset());
        OGL_CHECK()
        break;

      case NVertexBuffer::VES_POSITION_XY:

        location = m_activeProgram->attributeLocation(KAttributeVertexName);
        EGE_ASSERT(0 <= location);

        glEnableVertexAttribArray(location);
        OGL_CHECK()
        glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, vertexBuffer->vertexDeclaration().vertexSize(), 
                              static_cast<s8*>(vertexBuffer->offset()) + itElement->offset());
        OGL_CHECK()
        break;

      case NVertexBuffer::VES_NORMAL:

        location = m_activeProgram->attributeLocation(KAttributeNormalName);
        EGE_ASSERT(0 <= location);

        glEnableVertexAttribArray(location);
        OGL_CHECK()
        glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, vertexBuffer->vertexDeclaration().vertexSize(), 
                              static_cast<s8*>(vertexBuffer->offset()) + itElement->offset());
        OGL_CHECK()
        break;

      case NVertexBuffer::VES_COLOR_RGBA:

        location = m_activeProgram->attributeLocation(KAttributeColorName);
        EGE_ASSERT(0 <= location);

        glEnableVertexAttribArray(location);
        OGL_CHECK();
        glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, vertexBuffer->vertexDeclaration().vertexSize(), 
                              static_cast<s8*>(vertexBuffer->offset()) + itElement->offset());
        OGL_CHECK();
        break;

      case NVertexBuffer::VES_TEXTURE_UV:

        location = m_activeProgram->attributeLocation(KAttributeTextureCoordXName.arg(textureCoordIndex++));
        EGE_ASSERT(0 <= location);

        glEnableVertexAttribArray(location);
        OGL_CHECK()
        glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, vertexBuffer->vertexDeclaration().vertexSize(), 
                              static_cast<s8*>(vertexBuffer->offset()) + itElement->offset());
        OGL_CHECK()
        break;

      default:
        break;
    }
  }

  // bind IBO
  indexBuffer->bind();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
