#include "Core/Graphics/OpenGL/Implementation/RenderSystemOGL.h"
#include "Core/Graphics/OpenGL/Implementation/VertexArrayObject.h"
#include "Core/Graphics/OpenGL/ShaderOGL.h"
#include "Core/Graphics/OpenGL/ProgramOGL.h"
#include "Core/Graphics/OpenGL/IndexBufferVAOGL.h"
#include "Core/Graphics/OpenGL/IndexBufferVBOOGL.h"
#include "Core/Graphics/OpenGL/VertexBufferVAOGL.h"
#include "Core/Graphics/OpenGL/VertexBufferVBOOGL.h"
#include "Core/Graphics/OpenGL/RenderTextureCopyOGL.h"
#include "Core/Graphics/OpenGL/RenderTextureFBOOGL.h"
#include "Core/Graphics/OpenGL/Texture2DOGL.h"
#include "Core/Graphics/Render/Implementation/RenderSystemStatistics.h"
#include "EGEEngine.h"
#include "EGEGraphics.h"
#include "EGETimer.h"
#include "EGERenderQueues.h"
#include "EGEDevice.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderSystemOGL::RenderSystemOGL(Engine& engine) : RenderSystem(engine)
                                                 , m_activeTextureUnit(0)
                                                 , m_activeTextureUnitsCount(0)
{
  m_blendEnabled = (GL_TRUE == glIsEnabled(GL_BLEND));
  OGL_CHECK()
  
  m_scissorTestEnabled = (GL_TRUE == glIsEnabled(GL_SCISSOR_TEST));
  OGL_CHECK()
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderSystemOGL::~RenderSystemOGL()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemOGL::clearViewport(const PViewport& viewport)
{
  // determine which buffers to clear
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

  // check if anything to clear
  if (0 != bits)
  {
    // do clear
    glClear(bits);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemOGL::setViewport(const PViewport& viewport)
{
  // call base class
  RenderSystem::setViewport(viewport);

  // NOTE: auto-rotation does not affect viewports. They always works in portrait mode.

  Rectf actualRect = viewport->physicalRect();

  // NOTE: arguments of viewport should be in physical window-coordinate space ie not affected by zoom
  glViewport(static_cast<GLint>(actualRect.x), static_cast<GLint>(actualRect.y),
             static_cast<GLsizei>(actualRect.width), static_cast<GLsizei>(actualRect.height));
  OGL_CHECK()
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemOGL::flush()
{
  RenderSystemFrameStatisticData& statisticsData = ege_cast<RenderSystemStatistics*>(this->component(EGE_OBJECT_UID_RENDER_SYSTEM_STATISTICS))->currentRecord();

  // go thru all render queues
  for (Map<s32, List<PRenderQueue> >::const_iterator itQueue = m_renderQueues.begin(); itQueue != m_renderQueues.end(); ++itQueue)
  {
    const List<PRenderQueue>& queueList = itQueue->second;

    if (5 < queueList.length())
    {
      egeWarning(KOpenGLDebugName) << "Possible batch optimization. Hash:" << itQueue->first;
    }

    // update statistics
    // NOTE: this appends new render queue data object to the pool. This object will be used while processing all render queues below to aggregate all 
    //       individual information.
    RenderSystemRenderQueueData queueData;
    queueData.hash              = itQueue->first;
    queueData.primitiveType     = queueData.hash & 0xff;
    queueData.priority          = queueData.hash >> 8;
    queueData.batchCount        = 0;
    queueData.vertexCount       = 0;
    queueData.indexedBatchCount = 0;
    statisticsData.queues.push_back(queueData);

    // render queue list
    for (List<PRenderQueue>::ConstIterator it = queueList.begin(); it != queueList.end(); ++it)
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
void RenderSystemOGL::activateTextureUnit(u32 unit)
{
  if (unit != m_activeTextureUnit)
  {
    EGE_ASSERT_X(Device::TextureUnitsCount() > unit, "Activating texture unit outside of available range!");

    // check if unit available
    if (unit < Device::TextureUnitsCount())
    {
      // check if multitexture available
      if (Device::HasRenderCapability(ERenderCapabilityMultitexturing))
      {
        glActiveTexture(GL_TEXTURE0 + unit);
        OGL_CHECK();

        m_activeTextureUnit = unit;
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemOGL::bindTexture(GLenum target, GLuint textureId)
{
  // enable target first
  glEnable(target);
  OGL_CHECK()

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
  OGL_CHECK()

  // check if different texture bound currently
  if (static_cast<GLuint>(boundTextureId) != textureId)
  {
    // bind new texture to target
    glBindTexture(target, textureId);
    OGL_CHECK()
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PVertexBuffer RenderSystemOGL::createVertexBuffer(const String& name, const VertexDeclaration& vertexDeclaration, NVertexBuffer::UsageType usage) const
{
  PVertexBuffer buffer;

  // check if VBO is available
  if (Device::HasRenderCapability(ERenderCapabilityVertexBufferObjects))
  {
    VertexBufferVBO* vertexBuffer = ege_new VertexBufferVBO(name, vertexDeclaration, usage);
    if ((NULL == vertexBuffer) || (0 == vertexBuffer->id()))
    {
      // error!
      EGE_DELETE(vertexBuffer);
    }

    buffer = vertexBuffer;
  }
  else
  {
    buffer = ege_new VertexBufferVA(name, vertexDeclaration);
  }

  return buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PIndexBuffer RenderSystemOGL::createIndexBuffer(const String& name, EGEIndexBuffer::UsageType usage) const
{
  PIndexBuffer buffer;

  if (Device::HasRenderCapability(ERenderCapabilityVertexBufferObjects))
  {
    IndexBufferVBO* indexBuffer = ege_new IndexBufferVBO(name, usage);
    if ((NULL == indexBuffer) || (0 == indexBuffer->id()))
    {
      // error!
      EGE_DELETE(indexBuffer);
    }

    buffer = indexBuffer;
  }
  else
  {
    buffer = ege_new IndexBufferVA(name);
  }

  return buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PTexture2D RenderSystemOGL::createTexture2D(const String& name, const PImage& image)
{
  s64 startTime = Timer::GetMicroseconds();

  // create empty texture
  PTexture2D texture = createEmptyTexture(name);
  if (NULL == texture)
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
PTexture2D RenderSystemOGL::createTexture2D(const String& name, const PDataBuffer& data)
{
  // create empty texture
  PTexture2D texture = createEmptyTexture(name);
  if (NULL == texture)
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
PTexture2D RenderSystemOGL::createRenderTexture(const String& name, s32 width, s32 height, PixelFormat format)
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
  if (NULL == texture)
  {
    // error!
    return NULL;
  }

  // create texture from image
  if (EGE_SUCCESS != texture->create(image))
  {
    // error!
    return NULL;
  }

  Texture2DOGL* textureOGL = ege_cast<Texture2DOGL*>(texture);

  // detach any texture
  bindTexture(GL_TEXTURE_2D, 0);

  Dictionary params;
  params[EGE_RENDER_TARGET_PARAM_NAME]    = name;
  params[EGE_RENDER_TARGET_PARAM_WIDTH]   = String::FromNumber(width);
  params[EGE_RENDER_TARGET_PARAM_HEIGHT]  = String::FromNumber(height);

  // check if FBO is supported
  PRenderTarget target;
  if (Device::HasRenderCapability(ERenderCapabilityFrameBufferObjects))
  {
    target = ege_new RenderTextureFBOOGL(params, GL_TEXTURE_2D, GL_TEXTURE_2D, textureOGL->id());
  }
  else
  {
    target = ege_new RenderTextureCopyOGL(params, GL_TEXTURE_2D, GL_TEXTURE_2D, textureOGL->id());
  }

  // check if could not be allocated
  if (NULL == target)
  {
    // error!
    return NULL;
  }

  // set render target
  texture->setRenderTarget(target);

  // add into render targets
  engine().graphics()->registerRenderTarget(texture->renderTarget());

  egeWarning(KOpenGLDebugName) << "Creating render target done" << texture;

  return texture;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PTexture2D RenderSystemOGL::createEmptyTexture(const String& name)
{
  Texture2DOGL* texture = ege_new Texture2DOGL(engine(), name, this);
  if (NULL != texture)
  {
    EGE_ASSERT(0 <= texture->id());

    // setup 1 byte alignment
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // bind it
    activateTextureUnit(0);
    bindTexture(GL_TEXTURE_2D, texture->id());

    // set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mapTextureFilter(m_textureMinFilter, m_textureMipMapping));
    OGL_CHECK()
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mapTextureFilter(m_textureMagFilter, false));
    OGL_CHECK()
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mapTextureAddressingMode(m_textureAddressingModeS));
    OGL_CHECK()
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mapTextureAddressingMode(m_textureAddressingModeT));
    OGL_CHECK()
  }

  return texture;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemOGL::setBlendEnabled(bool set)
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
void RenderSystemOGL::setScissorTestEnabled(bool set)
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
bool RenderSystemOGL::registerComponent(PRenderComponent& component, NVertexBuffer::UsageType vertexUsage, const VertexDeclaration& vertexDeclaration, 
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
GLint RenderSystemOGL::mapTextureFilter(TextureFilter filter, bool mipmapping) const
{
  GLint result = GL_NEAREST;

  switch (filter)
  {
    case TF_NEAREST:    result = mipmapping ? GL_NEAREST_MIPMAP_NEAREST: GL_NEAREST; break;
    case TF_BILINEAR:   result = mipmapping ? GL_NEAREST_MIPMAP_LINEAR : GL_LINEAR; break;
    case TF_TRILINEAR:  result = GL_LINEAR_MIPMAP_LINEAR; break;

    default:
      break;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
GLint RenderSystemOGL::mapTextureAddressingMode(TextureAddressingMode mode) const
{
  GLint result = GL_REPEAT;

  switch (mode)
  {
    case AM_CLAMP:  result = GL_CLAMP_TO_EDGE; break;
    case AM_REPEAT: result = GL_REPEAT; break;

    default:
      break;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
GLenum RenderSystemOGL::mapBlendFactor(EGEGraphics::BlendFactor factor) const
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
GLenum RenderSystemOGL::mapPrimitiveType(EGEGraphics::RenderPrimitiveType type) const
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
GLenum RenderSystemOGL::mapIndexSize(EGEIndexBuffer::IndexSize size) const
{
  GLenum result = GL_UNSIGNED_INT;

  switch (size)
  {
    case EGEIndexBuffer::IS_8BIT:   result = GL_UNSIGNED_BYTE; break;
    case EGEIndexBuffer::IS_16BIT:  result = GL_UNSIGNED_SHORT; break;
    case EGEIndexBuffer::IS_32BIT:  
      
      EGE_ASSERT(Device::HasRenderCapability(ERenderCapabilityElementIndexUnsignedInt));

      result = GL_UNSIGNED_INT; 
      break;

    default:
      break;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemOGL::applyGeneralParams()
{
  const PRenderComponent& component = activeRenderComponent();
  
  PRenderTarget renderTarget = currentRenderTarget();
  EGE_ASSERT(NULL != renderTarget);

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
    if ( ! renderTarget->requiresTextureFlipping())
    {
      // convert "upper-left" corner to "lower-left"
      clipRect.y = renderTarget->height() - clipRect.height - clipRect.y;
    }

    // check if not auto-rotated
    if ( ! renderTarget->isAutoRotated())
    {
      // apply opposite rotation to rectangle to convert it into native (non-transformed) coordinate
      clipRect = applyRotation(clipRect, renderTarget->orientationRotation());
    }

    // apply zoom
    // NOTE: this is necessary as logical content may be ie bigger than physical window and because glScissor works in physical window-coordinate space
    float32 zoom = renderTarget->zoom();

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
void RenderSystemOGL::createAndSetupVAOs()
{
  const PRenderComponent& component = activeRenderComponent();

  bool result = true;

  PMaterial material    = component->material();
  List<PComponent> vaos = component->components(EGE_OBJECT_UID_VERTEX_ARRAY_OBJECT);

  // check if VAO is supported and can be used (ie VBO is supported)
  if (vaos.isEmpty() && 
      Device::HasRenderCapability(ERenderCapabilityVertexArrayObjects) && 
      Device::HasRenderCapability(ERenderCapabilityVertexBufferObjects))
  {
    for (u32 pass = 0; (pass < material->passCount()) && result; ++pass)
    {
      // create vao
      PVertexArrayObject vao = ege_new VertexArrayObject(component->name() + String::Format("-vao-%1", pass));
      if ((NULL == vao) || (EGE_SUCCESS != component->addComponent(vao)))
      {
        // error!
        result = false;
      }
      else
      {
        // add to local pool
        vaos.append(vao);

        // set it up
        setupVAO(vao, component->vertexBuffer(), component->indexBuffer(), material->pass(pass));
      }
    }
  }

  // check if failed
  if ( ! result)
  {
    // remove all VAOs
    for (List<PComponent> ::ConstIterator it = vaos.begin(); it != vaos.end(); ++it)
    {
      component->removeComponent(*it);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END