#include "EGEApplication.h"
#include "Core/Graphics/OpenGL/GL 3.x/RenderSystemOGL3_p.h"
#include "Core/Graphics/Viewport.h"
#include "Core/Graphics/Camera.h"
#include "Core/Graphics/Graphics.h"
#include "Core/Graphics/Program.h"
#include "Core/Graphics/Shader.h"
#include "Core/Graphics/OpenGL/ShaderOGL.h"
#include "Core/Graphics/OpenGL/ProgramOGL.h"
#include "Core/Graphics/OpenGL/IndexBufferVAOGL.h"
#include "Core/Graphics/OpenGL/IndexBufferVBOOGL.h"
#include "Core/Graphics/OpenGL/VertexBufferVAOGL.h"
#include "Core/Graphics/OpenGL/VertexBufferVBOOGL.h"
#include "Core/Graphics/OpenGL/RenderTextureCopyOGL.h"
#include "Core/Graphics/OpenGL/RenderTextureFBOOGL.h"
#include "Core/Graphics/OpenGL/Implementation/VertexArrayObject.h"
#include "Core/Graphics/Material.h"
#include "Core/Graphics/OpenGL/Texture2DOGL.h"
#include "Core/Graphics/Render/RenderWindow.h"
#include "Core/Graphics/TextureImage.h"
#include "Core/Debug/Interface/EngineInfo.h"
#include "EGERenderQueues.h"
#include "EGEOpenGL.h"
#include "EGETimer.h"
#include "EGEDevice.h"
#include "EGERenderComponent.h"
#include "EGEDebug.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(RenderSystemPrivate)
EGE_DEFINE_DELETE_OPERATORS(RenderSystemPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const String KUniformModelViewPerspectiveName = "u_mvpMatrix";
static const String KUniformTexture2DXName           = "u_texture2D%1";
static const String KUniformTextureMatrixXName       = "u_texMatrix%1";

static const String KAttributeVertexName        = "a_vertex";
static const String KAttributeNormalName        = "a_normal";
static const String KAttributeColorName         = "a_color";
static const String KAttributeTextureCoordXName = "a_texCoord%1";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Maps primitive type to OpenGL compilant one. */
static GLenum MapPrimitiveType(EGEGraphics::RenderPrimitiveType type)
{
  GLenum result = GL_TRIANGLES;

  switch (type)
  {
    case EGEGraphics::RPT_TRIANGLES:        result = GL_TRIANGLES; break;
    case EGEGraphics::RPT_TRIANGLE_STRIPS:  result = GL_TRIANGLE_STRIP; break;
    case EGEGraphics::RPT_TRIANGLE_FAN:     result = GL_TRIANGLE_FAN; break;
    case EGEGraphics::RPT_LINES:            result = GL_LINES; break;
    case EGEGraphics::RPT_LINE_LOOP:        result = GL_LINE_LOOP; break;
    case EGEGraphics::RPT_POINTS:           result = GL_POINTS; break;

    default:
      break;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Maps blend factors to OpenGL compilant values. */
static GLenum MapBlendFactor(EGEGraphics::BlendFactor factor)
{
  GLenum result = GL_ONE;

  switch (factor)
  {
    case EGEGraphics::BF_ZERO:                result = GL_ZERO; break;
    case EGEGraphics::BF_ONE:                 result = GL_ONE; break;
    case EGEGraphics::BF_SRC_COLOR:           result = GL_SRC_COLOR; break;
    case EGEGraphics::BF_DST_COLOR:           result = GL_DST_COLOR; break;
    case EGEGraphics::BF_ONE_MINUS_SRC_COLOR: result = GL_ONE_MINUS_SRC_COLOR; break;
    case EGEGraphics::BF_ONE_MINUS_DST_COLOR: result = GL_ONE_MINUS_DST_COLOR; break;
    case EGEGraphics::BF_SRC_ALPHA:           result = GL_SRC_ALPHA; break;
    case EGEGraphics::BF_DST_ALPHA:           result = GL_DST_ALPHA; break;
    case EGEGraphics::BF_ONE_MINUS_SRC_ALPHA: result = GL_ONE_MINUS_SRC_ALPHA; break;
    case EGEGraphics::BF_ONE_MINUS_DST_ALPHA: result = GL_ONE_MINUS_DST_ALPHA; break;

    default:
      break;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Maps texture environment to OpenGL compilant one. */
static GLint MapPrimitiveType(EGETexture::EnvironmentMode mode)
{
  GLint result = GL_MODULATE;

  switch (mode)
  {
    case EGETexture::EM_ADD:      result = GL_ADD; break;
    case EGETexture::EM_BLEND:    result = GL_BLEND; break;
    //case EGETexture::EM_COMBINE:  result = GL_COMBINE; break;
    case EGETexture::EM_DECAL:    result = GL_DECAL; break;
    case EGETexture::EM_MODULATE: result = GL_MODULATE; break;
    case EGETexture::EM_REPLACE:  result = GL_REPLACE; break;

    default:
      break;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Maps incides size to OpenGL compilant value. */
static GLenum MapIndexSize(EGEIndexBuffer::IndexSize size)
{
  GLenum result = GL_UNSIGNED_INT;

  switch (size)
  {
    case EGEIndexBuffer::IS_8BIT:   result = GL_UNSIGNED_BYTE; break;
    case EGEIndexBuffer::IS_16BIT:  result = GL_UNSIGNED_SHORT; break;
    case EGEIndexBuffer::IS_32BIT:  
      
      EGE_ASSERT(Device::HasRenderCapability(EGEDevice::RENDER_CAPS_ELEMENT_INDEX_UINT));

      result = GL_UNSIGNED_INT; 
      break;

    default:
      break;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Maps texture filtering mode to OpenGL compilant value. */
static GLint MapTextureFilter(EGETexture::Filter filter)
{
  GLint result = GL_NEAREST;

  switch (filter)
  {
    case EGETexture::BILINEAR:          result = GL_NEAREST; break;
    case EGETexture::TRILINEAR:         result = GL_LINEAR; break;
    case EGETexture::MIPMAP_BILINEAR:   result = GL_LINEAR_MIPMAP_NEAREST; break;
    case EGETexture::MIPMAP_TRILINEAR:  result = GL_LINEAR_MIPMAP_LINEAR; break;

    default:
      break;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Maps texture addressing mode to OpenGL compilant value. */
static GLint MapTextureAddressingMode(EGETexture::AddressingMode mode)
{
  GLint result = GL_REPEAT;

  switch (mode)
  {
    case EGETexture::AM_CLAMP:  result = GL_CLAMP_TO_EDGE; break;
    case EGETexture::AM_REPEAT: result = GL_REPEAT; break;

    default:
      break;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderSystemPrivate::RenderSystemPrivate(RenderSystem* base) : m_d(base), 
                                                               m_activeTextureUnit(0),
                                                               m_activeTextureUnitsCount(0)
{
  m_blendEnabled = (GL_TRUE == glIsEnabled(GL_BLEND));
  OGL_CHECK();
  
  m_scissorTestEnabled = (GL_TRUE == glIsEnabled(GL_SCISSOR_TEST));
  OGL_CHECK();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderSystemPrivate::~RenderSystemPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemPrivate::clearViewport(const PViewport& viewport)
{
  GLbitfield bits = 0;

  if (viewport->clearBufferTypes() & Viewport::BUFFER_TYPE_COLOR)
  {
    const Color& clearColor = viewport->clearColor();
    glClearColor(clearColor.red, clearColor.green, clearColor.blue, clearColor.alpha);

    bits |= GL_COLOR_BUFFER_BIT;
  }

  if (viewport->clearBufferTypes() & Viewport::BUFFER_TYPE_DEPTH)
  {
    bits |= GL_DEPTH_BUFFER_BIT;
  }

  if (0 != bits)
  {
    glClear(bits);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemPrivate::setViewport(const PViewport& viewport)
{
  // NOTE: auto-rotation does not affect viewports. They always works in portrait mode.

  Rectf actualRect = viewport->physicalRect();

  // NOTE: arguments of viewport should be in physical window-coordinate space ie not affected by zoom
  glViewport(static_cast<GLint>(actualRect.x), static_cast<GLint>(actualRect.y),
             static_cast<GLsizei>(actualRect.width), static_cast<GLsizei>(actualRect.height));
  OGL_CHECK()
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemPrivate::flush()
{
  // prepare projection matrix
  m_projectionMatrix = d_func()->m_projectionMatrix;

  // check if not auto-rotated
  if ( ! d_func()->m_renderTarget->isAutoRotated())
  {
    // apply rotation
    // NOTE: actual OGLES rotation should be opposite
    Quaternionf rotation = Math::CreateQuaternion(Vector3f(0, 0, 1), -d_func()->m_renderTarget->orientationRotation());

    Matrix4f matrix;
    Math::Convert(matrix, rotation);

    m_projectionMatrix *= matrix;
  }

  // go thru all render queues
  for (Map<s32, List<PRenderQueue> >::const_iterator itQueue = d_func()->m_renderQueues.begin(); itQueue != d_func()->m_renderQueues.end(); ++itQueue)
  {
    const List<PRenderQueue>& queueList = itQueue->second;

    if (5 < queueList.size())
    {
      egeWarning(KOpenGLDebugName) << "Possible batch optimization. Hash:" << itQueue->first;
    }

    // render queue list
    for (List<PRenderQueue>::const_iterator it = queueList.begin(); it != queueList.end(); ++it)
    {
      PRenderQueue queue = *it;

      // render queue
      queue->render(*this);

      // clear queue
      queue->clear();
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemPrivate::applyPassParams(const PRenderComponent& component, const PMaterial& material, const RenderPass& pass)
{
  ProgramOGL* program = ege_cast<ProgramOGL*>(pass.program());
  EGE_ASSERT(NULL != program);

  // TAGE - port
  //Color color = pass.diffuseColorTransformation().transform(pass.diffuseColor());
  //glColor4f(color.red, color.green, color.blue, color.alpha);
  //OGL_CHECK();

  // go thru all textures
  for (u32 i = 0; i < pass.textureCount(); ++i)
  {
    PTextureImage textureImage = pass.texture(i);

    EGE_ASSERT(EGE_OBJECT_UID_TEXTURE_2D != textureImage->uid());

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

      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, MapPrimitiveType(textureImage->environmentMode()));

      // assign appropriate texture sampler to texture unit
      GLint location = program->uniformLocation(KUniformTexture2DXName.arg(i));
      EGE_ASSERT(0 <= location);

      glUniform1i(location, i);
      OGL_CHECK();

      // supply texture matrix if needed
      location = program->uniformLocation(KUniformTextureMatrixXName.arg(i));
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
        OGL_CHECK();

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
  m_activeTextureUnitsCount = pass.textureCount();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemPrivate::activateTextureUnit(u32 unit)
{
  if (unit != m_activeTextureUnit)
  {
    EGE_ASSERT_X(Device::TextureUnitsCount() > unit, "Activating texture unit outside of available range!");

    // check if unit available
    if (unit < Device::TextureUnitsCount())
    {
      // check if multitexture available
      if (Device::HasRenderCapability(EGEDevice::RENDER_CAPS_MULTITEXTURE))
      {
        glActiveTexture(GL_TEXTURE0 + unit);
        OGL_CHECK();

        m_activeTextureUnit = unit;
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemPrivate::bindTexture(GLenum target, GLuint textureId)
{
  // map texture target into texture binding query value
  GLenum textureBinding;
  switch (target)
  {
    case GL_TEXTURE_1D: textureBinding = GL_TEXTURE_BINDING_1D; break;
    case GL_TEXTURE_2D: textureBinding = GL_TEXTURE_BINDING_2D; break;

    default:

      EGE_ASSERT_X(false, "Incorrect texture binding!");
      return;
  }

  // query texture Id bound to given target
  GLint boundTextureId;
	glGetIntegerv(textureBinding, &boundTextureId);

  // check if different texture bound currently
  if (static_cast<GLuint>(boundTextureId) != textureId)
  {
    // bind new texture to target
    glBindTexture(target, textureId);
    OGL_CHECK();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemPrivate::applyGeneralParams(const PRenderComponent& component)
{
  // apply scissor test
  if (component->clipRect().isNull())
  {
    setScissorTestEnabled(false);
  }
  else
  {
    setScissorTestEnabled(true);

    Rectf clipRect = component->clipRect();
    
    // check if conversion from "upper-left" corner to "lower-left" for current orientation is necessary
    if ( ! d_func()->m_renderTarget->requiresTextureFlipping())
    {
      // convert "upper-left" corner to "lower-left"
      clipRect.y = d_func()->m_renderTarget->height() - clipRect.height - clipRect.y;
    }

    // check if not auto-rotated
    if ( ! d_func()->m_renderTarget->isAutoRotated())
    {
      // apply opposite rotation to rectangle to convert it into native (non-transformed) coordinate
      clipRect = d_func()->applyRotation(clipRect, d_func()->m_renderTarget->orientationRotation());
    }

    // apply zoom
    // NOTE: this is necessary as logical content may be ie bigger than physical window and because glScissor works in physical window-coordinate space
    float32 zoom = d_func()->m_renderTarget->zoom();

    clipRect.x      *= zoom;
    clipRect.y      *= zoom;
    clipRect.width  *= zoom;
    clipRect.height *= zoom;

    glScissor(static_cast<GLint>(clipRect.x), static_cast<GLint>(clipRect.y), static_cast<GLsizei>(clipRect.width), static_cast<GLsizei>(clipRect.height));
  }

  // check if points are to be rendered
  if (EGEGraphics::RPT_POINTS == component->primitiveType())
  {
    // set point size
    glPointSize(component->pointSize());
  }
  // check if lines are to be rendered
  else if ((EGEGraphics::RPT_LINES == component->primitiveType()) || (EGEGraphics::RPT_LINES == component->primitiveType()))
  {
    glLineWidth(component->lineWidth());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PVertexBuffer RenderSystemPrivate::createVertexBuffer(const String& name, const VertexDeclaration& vertexDeclaration, NVertexBuffer::UsageType usage) const
{
  PVertexBuffer buffer;

  if (Device::HasRenderCapability(EGEDevice::RENDER_CAPS_VERTEX_BUFFER_OBJECT))
  {
    VertexBufferVBO* vertexBuffer = ege_new VertexBufferVBO(d_func()->app(), name, vertexDeclaration, usage);
    if ((NULL == vertexBuffer) || (0 == vertexBuffer->m_id))
    {
      // error!
      EGE_DELETE(vertexBuffer);
    }

    buffer = vertexBuffer;
  }
  else
  {
    buffer = ege_new VertexBufferVA(d_func()->app(), name, vertexDeclaration);
  }

  return buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PIndexBuffer RenderSystemPrivate::createIndexBuffer(const String& name, EGEIndexBuffer::UsageType usage) const
{
  PIndexBuffer buffer;

  if (Device::HasRenderCapability(EGEDevice::RENDER_CAPS_VERTEX_BUFFER_OBJECT))
  {
    IndexBufferVBO* indexBuffer = ege_new IndexBufferVBO(d_func()->app(), name, usage);
    if ((NULL == indexBuffer) || (0 == indexBuffer->m_id))
    {
      // error!
      EGE_DELETE(indexBuffer);
    }

    buffer = indexBuffer;
  }
  else
  {
    buffer = ege_new IndexBufferVA(d_func()->app(), name);
  }

  return buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PTexture2D RenderSystemPrivate::createTexture2D(const String& name, const PImage& image)
{
  s64 startTime = Timer::GetMicroseconds();

  // create empty texture
  PTexture2D texture = createEmptyTexture(name);
  if ((NULL == texture) || ! texture->isValid())
  {
    // error!
    return NULL;
  }

  // create it
  if (EGE_SUCCESS != texture->create(image))
  {
    // error!
    return NULL;
  }

  bindTexture(GL_TEXTURE_2D, 0);

  s64 endTime = Timer::GetMicroseconds();
  egeDebug(KOpenGLDebugName) << "Texture" << name << "uploaded:" << endTime - startTime << "microseconds";

  return texture;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PTexture2D RenderSystemPrivate::createTexture2D(const String& name, const PDataBuffer& data)
{
  // create empty texture
  PTexture2D texture = createEmptyTexture(name);
  if ((NULL == texture) || ! texture->isValid())
  {
    // error!
    return NULL;
  }

  // upload data
  if (EGE_SUCCESS != texture->create(data))
  {
    // error!
    return NULL;
  }

  bindTexture(GL_TEXTURE_2D, 0);

  return texture;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PTexture2D RenderSystemPrivate::createRenderTexture(const String& name, s32 width, s32 height, PixelFormat format)
{
  // create empty image from which empty texture is to be created
  PImage image = ImageUtils::CreateImage(width, height, format, false, 0, NULL);
  if (NULL == image)
  {
    // error!
    return NULL;
  }

  // create empty texture
  PTexture2D texture = createEmptyTexture(name);
  if ((NULL == texture) || ! texture->isValid())
  {
    // error!
    return NULL;
  }

  // create texture from image
  if (EGE_SUCCESS != texture->p_func()->create(image))
  {
    // error!
    return NULL;
  }

  bindTexture(GL_TEXTURE_2D, 0);

  Dictionary params;
  params[EGE_RENDER_TARGET_PARAM_NAME]    = name;
  params[EGE_RENDER_TARGET_PARAM_WIDTH]   = String::FromNumber(width);
  params[EGE_RENDER_TARGET_PARAM_HEIGHT]  = String::FromNumber(height);

  // check if Frame Buffer Object is supported
  if (Device::HasRenderCapability(EGEDevice::RENDER_CAPS_FBO))
  {
    texture->m_target = ege_new RenderTextureFBOOGL(d_func()->app(), params, GL_TEXTURE_2D, GL_TEXTURE_2D, texture->p_func()->id());
  }
  else
  {
    texture->m_target = ege_new RenderTextureCopyOGL(d_func()->app(), params, GL_TEXTURE_2D, GL_TEXTURE_2D, texture->p_func()->id());
  }

  // check if could not be allocated
  if (NULL == texture->m_target)
  {
    // error!
    return NULL;
  }

  egeWarning(KOpenGLDebugName) << "Creating render target done" << texture->m_target;

  // add into render targets
  d_func()->app()->graphics()->registerRenderTarget(texture->m_target);

  return texture;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemPrivate::destroyTexture2D(PTexture2D texture)
{
  if (0 != texture->p_func()->m_id)
  {
    egeDebug(KOpenGLDebugName) << "Destroying texture" << texture->p_func()->m_id << texture->name();
  
    glDeleteTextures(1, &texture->p_func()->m_id);
    OGL_CHECK();
    texture->p_func()->m_id = 0;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PTexture2D RenderSystemPrivate::createEmptyTexture(const String& name)
{
  PTexture2D texture = ege_new Texture2D(d_func()->app(), name, d_func());
  if ((NULL == texture) || (NULL == texture->p_func()))
  {
    // error!
    return NULL;
  }

  // generate OGL texture
  glGenTextures(1, &texture->p_func()->m_id);
  OGL_CHECK();

  // setup 1 byte alignment
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  // bind it
  activateTextureUnit(0);
  bindTexture(GL_TEXTURE_2D, texture->p_func()->id());

  // set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MapTextureFilter(d_func()->m_textureMinFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MapTextureFilter(d_func()->m_textureMagFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, MapTextureAddressingMode(d_func()->m_textureAddressingModeS));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, MapTextureAddressingMode(d_func()->m_textureAddressingModeT));

  // store parameters
  // NOTE: mostly for debugging purposes
  texture->m_minFilter        = d_func()->m_textureMinFilter;
  texture->m_magFilter        = d_func()->m_textureMagFilter;
  texture->m_addressingModeS  = d_func()->m_textureAddressingModeS;
  texture->m_addressingModeT  = d_func()->m_textureAddressingModeT;

  return texture;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PShader RenderSystemPrivate::createShader(EGEGraphics::ShaderType type, const String& name, const PDataBuffer& data)
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
  ShaderOGL* shader = ege_new ShaderOGL(d_func()->app(), name, type, d_func());
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
PProgram RenderSystemPrivate::createProgram(const String& name, const List<PShader>& shaders)
{
  // create shader program
  ProgramOGL* program = ege_new ProgramOGL(d_func()->app(), name, d_func());
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
void RenderSystemPrivate::renderComponent(const PRenderComponent& component, const Matrix4f& modelMatrix)
{
  PVertexBuffer vertexBuffer  = component->vertexBuffer();
  PIndexBuffer indexBuffer    = component->indexBuffer();
  PMaterial material          = component->material();

  EGE_ASSERT_X(NULL != material, "Component must have material attached.");
  EGE_ASSERT_X(0 < vertexBuffer->vertexCount(), "No vertices to render!");

  // make sure VAOs are created and set up
  createAndSetupVAOs(component);
  
  List<PComponent> vaos = component->components(EGE_OBJECT_UID_VERTEX_ARRAY_OBJECT);

  //if (component->name() == "level-meter-classic")
  //{
  //    int a = 1;
  //}

  if (vaos.empty())
  {
    // bind vertex and index buffers
    vertexBuffer->bind();
    indexBuffer->bind();
  }

  // apply general params
  applyGeneralParams(component);

  // go thru all passes
  for (u32 pass = 0; pass < material->passCount(); ++pass)
  {
    const PRenderPass renderPass = material->pass(pass);

    ProgramOGL* program = ege_cast<ProgramOGL*>(renderPass->program());
    EGE_ASSERT(NULL != program);

    // get first vao from the list and shrink the list
    PVertexArrayObject vao = vaos.first(NULL);
    if ( ! vaos.empty())
    {
      vaos.pop_front();
    }

    // bind
    program->bind();

    // set model-view matrix
    glUniformMatrix4fv(program->uniformLocation(KUniformModelViewPerspectiveName), 1, GL_FALSE, 
                        d_func()->m_viewMatrix.multiply(modelMatrix).multiply(m_projectionMatrix).data);
    OGL_CHECK();

    // apply pass related params
    applyPassParams(component, material, *renderPass);

    // apply vertex arrays
    List<u32> appliedVertexIndices;
    if (NULL == vao)
    {
      appliedVertexIndices = applyVertexArrays(program, vertexBuffer->vertexDeclaration(), vertexBuffer->offset());
    }
    else
    {
      // bind vao
      vao->bind();
    }

    u32 value = (0 < indexBuffer->indexCount()) ? indexBuffer->indexCount() : vertexBuffer->vertexCount();

    d_func()->m_vertexCount += value;
    d_func()->m_batchCount++;

    // check if INDICIES are to be used
    if (0 < indexBuffer->indexCount())
    {
      // render only if there is anything to render
      glDrawElements(MapPrimitiveType(component->primitiveType()), indexBuffer->indexCount(), MapIndexSize(indexBuffer->size()), indexBuffer->offset());
      OGL_CHECK();

      // update engine info
      ENGINE_INFO(drawElementsCalls++);
    }
    else
    {
      // render only if there is anything to render
      glDrawArrays(MapPrimitiveType(component->primitiveType()), 0, vertexBuffer->vertexCount());
      OGL_CHECK();

      // update engine info
      ENGINE_INFO(drawArraysCalls++);
    }

    if (NULL == vao)
    {
      for (List<u32>::const_iterator it = appliedVertexIndices.begin(); it != appliedVertexIndices.end(); ++it)
      {
        glDisableVertexAttribArray(*it);
      }
    }
    else
    {
      // unbind vao
      vao->unbind();
    }

    // unbind program
    program->unbind();

    // clean up
    for (s32 i = m_activeTextureUnitsCount - 1; 0 <= i; --i)
    {
      // disable texturing on server side
      activateTextureUnit(i);
      glDisable(GL_TEXTURE_2D);
      OGL_CHECK();
    }
  }

  if (vaos.empty())
  {
    // unbind vertex and index buffers
    vertexBuffer->unbind();
    indexBuffer->unbind();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemPrivate::setBlendEnabled(bool set)
{
  if (set != m_blendEnabled)
  {
    if (set)
    {
      glEnable(GL_BLEND);
    }
    else
    {
      glDisable(GL_BLEND);
    }

    m_blendEnabled = set;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemPrivate::setScissorTestEnabled(bool set)
{
  if (set != m_scissorTestEnabled)
  {
    if (set)
    {
      glEnable(GL_SCISSOR_TEST);
    }
    else
    {
      glDisable(GL_SCISSOR_TEST);
    }
    
    m_scissorTestEnabled = set;
  }  
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
List<u32> RenderSystemPrivate::applyVertexArrays(const ProgramOGL* program, const VertexDeclaration& vertexDeclaration, void* vertexData)
{
  EGE_ASSERT(NULL != program);

  List<u32> enabledIndices;

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

        location = program->attributeLocation(KAttributeVertexName);
        EGE_ASSERT(0 <= location);

        glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, vertexDeclaration.vertexSize(), static_cast<s8*>(vertexData) + itElement->offset());
        OGL_CHECK();
        glEnableVertexAttribArray(location);
        OGL_CHECK();

        enabledIndices << location;
        break;

      case NVertexBuffer::VES_POSITION_XY:

        location = program->attributeLocation(KAttributeVertexName);
        EGE_ASSERT(0 <= location);

        glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, vertexDeclaration.vertexSize(), static_cast<s8*>(vertexData) + itElement->offset());
        OGL_CHECK();
        glEnableVertexAttribArray(location);
        OGL_CHECK();

        enabledIndices << location;
        break;

      case NVertexBuffer::VES_NORMAL:

        location = program->attributeLocation(KAttributeNormalName);
        EGE_ASSERT(0 <= location);

        glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, vertexDeclaration.vertexSize(), static_cast<s8*>(vertexData) + itElement->offset());
        OGL_CHECK();
        glEnableVertexAttribArray(location);
        OGL_CHECK();

        enabledIndices << location;
        break;

      case NVertexBuffer::VES_COLOR_RGBA:

        location = program->attributeLocation(KAttributeColorName);
        EGE_ASSERT(0 <= location);

        glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, vertexDeclaration.vertexSize(), static_cast<s8*>(vertexData) + itElement->offset());
        OGL_CHECK();
        glEnableVertexAttribArray(location);
        OGL_CHECK();

        enabledIndices << location;
        break;

      case NVertexBuffer::VES_TEXTURE_UV:

        location = program->attributeLocation(KAttributeTextureCoordXName.arg(textureCoordIndex));
        EGE_ASSERT(0 <= location);

        glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, vertexDeclaration.vertexSize(), static_cast<s8*>(vertexData) + itElement->offset());
        OGL_CHECK();
        glEnableVertexAttribArray(location);  
        OGL_CHECK();

        enabledIndices << location;
        break;

      default:
        break;
    }
  }

  return enabledIndices;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool RenderSystemPrivate::registerComponent(PRenderComponent& component, NVertexBuffer::UsageType vertexUsage, const VertexDeclaration& vertexDeclaration, 
                                            EGEIndexBuffer::UsageType indexUsage)
{
  bool result = false;

  // add index buffer component
  if (EGE_SUCCESS == component->addComponent(createIndexBuffer(component->name() + "-ib", indexUsage)))
  {
    // add vertex buffer component
    if (EGE_SUCCESS == component->addComponent(createVertexBuffer(component->name() + "-vb", vertexDeclaration, vertexUsage)))
    {
      // done
      result = true;
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemPrivate::setupVAO(PVertexArrayObject& vertexArrayObject, const PVertexBuffer& vertexBuffer, const PIndexBuffer& indexBuffer, 
                                   const PProgram& program)
{
  EGE_ASSERT(NULL != program);

  ProgramOGL* programOGL = ege_cast<ProgramOGL*>(program);

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

        location = programOGL->attributeLocation(KAttributeVertexName);
        EGE_ASSERT(0 <= location);

        glEnableVertexAttribArray(location);
        OGL_CHECK();
        glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, vertexBuffer->vertexDeclaration().vertexSize(), static_cast<s8*>(vertexBuffer->offset()) + itElement->offset());
        OGL_CHECK();
        break;

      case NVertexBuffer::VES_POSITION_XY:

        location = programOGL->attributeLocation(KAttributeVertexName);
        EGE_ASSERT(0 <= location);

        glEnableVertexAttribArray(location);
        OGL_CHECK();
        glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, vertexBuffer->vertexDeclaration().vertexSize(), static_cast<s8*>(vertexBuffer->offset()) + itElement->offset());
        OGL_CHECK();
        break;

      case NVertexBuffer::VES_NORMAL:

        location = programOGL->attributeLocation(KAttributeNormalName);
        EGE_ASSERT(0 <= location);

        glEnableVertexAttribArray(location);
        OGL_CHECK();
        glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, vertexBuffer->vertexDeclaration().vertexSize(), static_cast<s8*>(vertexBuffer->offset()) + itElement->offset());
        OGL_CHECK();
        break;

      case NVertexBuffer::VES_COLOR_RGBA:

        location = programOGL->attributeLocation(KAttributeColorName);
        EGE_ASSERT(0 <= location);

        glEnableVertexAttribArray(location);
        OGL_CHECK();
        glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, vertexBuffer->vertexDeclaration().vertexSize(), static_cast<s8*>(vertexBuffer->offset()) + itElement->offset());
        OGL_CHECK();
        break;

      case NVertexBuffer::VES_TEXTURE_UV:

        location = programOGL->attributeLocation(KAttributeTextureCoordXName.arg(textureCoordIndex++));
        EGE_ASSERT(0 <= location);

        glEnableVertexAttribArray(location);
        OGL_CHECK();
        glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, vertexBuffer->vertexDeclaration().vertexSize(), static_cast<s8*>(vertexBuffer->offset()) + itElement->offset());
        OGL_CHECK();
        break;

      default:
        break;
    }
  }

  // bind IBO
  indexBuffer->bind();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemPrivate::createAndSetupVAOs(PRenderComponent component)
{
  bool result = true;

  PMaterial material    = component->material();
  List<PComponent> vaos = component->components(EGE_OBJECT_UID_VERTEX_ARRAY_OBJECT);

  // check if VAO is supported and can be used ie VBO is supported
  if (vaos.empty() && 
      Device::HasRenderCapability(EGEDevice::RENDER_CAPS_VERTEX_ARRAY_OBJECT) && 
      Device::HasRenderCapability(EGEDevice::RENDER_CAPS_VERTEX_BUFFER_OBJECT))
  {
    for (u32 pass = 0; (pass < material->passCount()) && result; ++pass)
    {
      // create vao
      PVertexArrayObject vao = new VertexArrayObject(d_func()->app(), component->name() + String::Format("-vao-%1", pass));
      if ((NULL == vao) || (EGE_SUCCESS != component->addComponent(vao)))
      {
        // error!
        result = false;
      }
      else
      {
        // add to local pool
        vaos.push_back(vao);

        // set it up
        setupVAO(vao, component->vertexBuffer(), component->indexBuffer(), material->pass(pass)->program());
      }
    }
  }

  // check if failed
  if ( ! result)
  {
    // remove all VAOs
    for (List<PComponent> ::const_iterator it = vaos.begin(); it != vaos.end(); ++it)
    {
      component->removeComponent(*it);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
