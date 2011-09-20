#include <EGEApplication.h>
#include "Core/Graphics/OpenGL/ES 1.0/RendererOGLES1_p.h"
#include "Core/Graphics/OpenGL/ExtensionsOGLES.h"
#include "Core/Components/Render/RenderComponent.h"
#include "Core/Graphics/Viewport.h"
#include "Core/Graphics/Camera.h"
#include "Core/Graphics/IndexBuffer.h"
#include "Core/Graphics/VertexBuffer.h"
#include "Core/Graphics/Material.h"
#include "Core/Graphics/OpenGL/Texture2DOGL.h"
#include "Core/Graphics/TextureImage.h"
#include "Core/Graphics/Render/RenderQueue.h"
#include <EGEDevice.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(RendererPrivate)
EGE_DEFINE_DELETE_OPERATORS(RendererPrivate)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Maps primitive type to OpenGL compilant one. */
static GLenum MapPrimitiveType(EGEGraphics::RenderPrimitiveType type)
{
  switch (type)
  {
    case EGEGraphics::RPT_TRIANGLES:        return GL_TRIANGLES;
    case EGEGraphics::RPT_TRIANGLE_STRIPS:  return GL_TRIANGLE_STRIP;
    case EGEGraphics::RPT_TRIANGLE_FAN:     return GL_TRIANGLE_FAN;
    case EGEGraphics::RPT_LINES:            return GL_LINES;
    case EGEGraphics::RPT_LINE_LOOP:        return GL_LINE_LOOP;
  }

  // default
  return GL_TRIANGLES;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Maps blend factors to OpenGL compilant values. */
static GLenum MapBlendFactor(EGEGraphics::BlendFactor factor)
{
  switch (factor)
  {
    case EGEGraphics::BF_ZERO:                return GL_ZERO;
    case EGEGraphics::BF_ONE:                 return GL_ONE;
    case EGEGraphics::BF_SRC_COLOR:           return GL_SRC_COLOR;
    case EGEGraphics::BF_DST_COLOR:           return GL_DST_COLOR;
    case EGEGraphics::BF_ONE_MINUS_SRC_COLOR: return GL_ONE_MINUS_SRC_COLOR;
    case EGEGraphics::BF_ONE_MINUS_DST_COLOR: return GL_ONE_MINUS_DST_COLOR;
    case EGEGraphics::BF_SRC_ALPHA:           return GL_SRC_ALPHA;
    case EGEGraphics::BF_DST_ALPHA:           return GL_DST_ALPHA;
    case EGEGraphics::BF_ONE_MINUS_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
    case EGEGraphics::BF_ONE_MINUS_DST_ALPHA: return GL_ONE_MINUS_DST_ALPHA;
  }

  // default
  return GL_ONE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Maps texture environment to OpenGL compilant one. */
static GLint MapPrimitiveType(EGETexture::EnvironmentMode mode)
{
  switch (mode)
  {
    case EGETexture::EM_ADD:      return GL_ADD;
    case EGETexture::EM_BLEND:    return GL_BLEND;
    case EGETexture::EM_COMBINE:  return GL_COMBINE;
    case EGETexture::EM_DECAL:    return GL_DECAL;
    case EGETexture::EM_MODULATE: return GL_MODULATE;
    case EGETexture::EM_REPLACE:  return GL_REPLACE;
  }

  // default
  return GL_MODULATE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Maps incides size to OpenGL compilant value. */
static GLenum MapIndexSize(IndexBuffer::Size size)
{
  switch (size)
  {
    case IndexBuffer::SIZE_8BIT:   return GL_UNSIGNED_BYTE;
    case IndexBuffer::SIZE_16BIT:  return GL_UNSIGNED_SHORT;
    case IndexBuffer::SIZE_32BIT:  return GL_UNSIGNED_INT;
  }

  // default
  return GL_UNSIGNED_INT;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RendererPrivate::RendererPrivate(Renderer* base) : m_d(base), m_activeTextureUnit(0xffffffff)
{
  detectCapabilities();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RendererPrivate::~RendererPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool RendererPrivate::isValid() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Clears given viewport. */
void RendererPrivate::clearViewport(const PViewport& viewport)
{
  GLbitfield bits = 0;

  u32 uiClearBuffersMask = viewport->clearBufferTypes();

  if (uiClearBuffersMask & Viewport::BUFFER_TYPE_COLOR)
  {
    const Color& clearColor = viewport->clearColor();
    glClearColor(clearColor.red, clearColor.green, clearColor.blue, clearColor.alpha);

    bits |= GL_COLOR_BUFFER_BIT;
  }

  if (uiClearBuffersMask & Viewport::BUFFER_TYPE_DEPTH)
  {
    bits |= GL_DEPTH_BUFFER_BIT;
  }

  glClear(bits);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets given viewport. */
void RendererPrivate::setViewport(const PViewport& viewport)
{
  // set render target
  RenderTarget* target = viewport->renderTarget();
  setRenderTarget(target);

  // set viewport area
  Rectf actualRect = viewport->physicalRect();

  if (!target->requiresTextureFlipping())
  {
    // convert "upper-left" corner to "lower-left"
    actualRect.y = target->physicalHeight() - actualRect.height - actualRect.y;
  }

  // set viewport to physical region occupied by render target
  glViewport((GLint) actualRect.x, (GLint) actualRect.y, (GLsizei) actualRect.width, (GLsizei) actualRect.height);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets render target. */
void RendererPrivate::setRenderTarget(const PRenderTarget& renderTarget)
{
  // unbind current render target
  if (d_func()->m_renderTarget)
  {
    d_func()->m_renderTarget->unbind();
  }

  d_func()->m_renderTarget = renderTarget;

  // switch context
  EGE_ASSERT(EGE_SUCCESS == renderTarget->makeCurrentContext());

  // bind new target
  d_func()->m_renderTarget->bind();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sends all geometry through the geometry pipeline to hardware. */
void RendererPrivate::flush()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glRotatef(d_func()->m_renderTarget->orientationRotation().degrees(), 0, 0, 1);
  glMultMatrixf(d_func()->m_projectionMatrix.data);

  // go thru all render queues
  for (Map<s32, PRenderQueue>::const_iterator itQueue = d_func()->m_renderQueues.begin(); itQueue != d_func()->m_renderQueues.end(); ++itQueue)
  {
    RenderQueue* queue = itQueue->second;
    
    // go thru all render data within current queue
    const MultiMap<u32, RenderQueue::SRENDERDATA>& renderData = queue->renderData();
    for (MultiMap<u32, RenderQueue::SRENDERDATA>::const_iterator it = renderData.begin(); it != renderData.end(); ++it)
    {
      const RenderQueue::SRENDERDATA& data = it->second;

      PVertexBuffer vertexBuffer = data.component->vertexBuffer();
      PIndexBuffer indexBuffer   = data.component->indexBuffer();
      PMaterial material         = data.component->material();

      // apply material
      applyMaterial(material);

      // NOTE: change to modelview after material is applied as it may change current matrix mode
      glMatrixMode(GL_MODELVIEW);

      // determine texture count
      s32 textureCount = material ? material->textureCount() : 0;

      // check if there is anything to be rendered
      if (0 != vertexBuffer->vertexCount())
      {
        const VertexBuffer::SemanticsList& semantics = vertexBuffer->semantics();

        u32 value = (0 < indexBuffer->indexCount()) ? indexBuffer->indexCount() : vertexBuffer->vertexCount();

        d_func()->m_vertexCount += value;
        d_func()->m_batchCount++;

        // lock vertex data
        void* vertexData = vertexBuffer->lock(0, vertexBuffer->vertexCount());

        // go thru all buffers
        for (VertexBuffer::SemanticsList::const_iterator itSemantic = semantics.begin(); itSemantic != semantics.end(); ++itSemantic)
        {
          // set according to buffer type
          switch (itSemantic->type)
          {
            case EGEVertexBuffer::AT_POSITION_XYZ:

              glVertexPointer(3, GL_FLOAT, vertexBuffer->vertexSize(), static_cast<s8*>(vertexData) + itSemantic->offset);
              glEnableClientState(GL_VERTEX_ARRAY);
              break;

            case EGEVertexBuffer::AT_NORMAL:

              glNormalPointer(GL_FLOAT, vertexBuffer->vertexSize(), static_cast<s8*>(vertexData) + itSemantic->offset);
              glEnableClientState(GL_NORMAL_ARRAY);
              break;

            case EGEVertexBuffer::AT_COLOR_RGBA:

              glColorPointer(4, GL_FLOAT, vertexBuffer->vertexSize(), static_cast<s8*>(vertexData) + itSemantic->offset);
              glEnableClientState(GL_COLOR_ARRAY);
              break;
      
            case EGEVertexBuffer::AT_TEXTURE_UV:

              for (s32 i = 0; i < textureCount; ++i)
              {
                glClientActiveTexture(GL_TEXTURE0 + i);

                glTexCoordPointer(2, GL_FLOAT, vertexBuffer->vertexSize(), static_cast<s8*>(vertexData) + itSemantic->offset);
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
              }
              break;

            //case EGEVertexBuffer::ARRAY_TYPE_TANGENT:

            //  // TANGENT is binded to GL_TEXTURE6
            //  glClientActiveTexture( GL_TEXTURE6 );
            //  glTexCoordPointer( 3, GL_FLOAT, cRenderOp.getVertexBuffer()->getVertexSize(), 
            //                     static_cast<char*>( vertexData )+iter->uiOffset );
            //  glEnableClientState( GL_TEXTURE_COORD_ARRAY );
            //  break;
          }
        }

        // TAGE - might be necessary
	      //if (multitexturing)
	      //glClientActiveTextureARB(GL_TEXTURE0);

        // set model-view matrix
        glLoadMatrixf(d_func()->m_viewMatrix.multiply(data.worldMatrix).data);

        // check if INDICIES are to be used
        if (0 < indexBuffer->indexCount())
        {
          // lock INDEX buffer
          void* indexData = indexBuffer->lock(0, indexBuffer->indexCount());

          // render only if there is anything to render
          glDrawElements(MapPrimitiveType(data.component->primitiveType()), indexBuffer->indexCount(), MapIndexSize(indexBuffer->size()), indexData);

          // unlock INDEX buffer
          indexBuffer->unlock();
        }
        else
        {
          // render only if there is anything to render
          glDrawArrays(MapPrimitiveType(data.component->primitiveType()), 0, vertexBuffer->vertexCount());
        }

        // unlock vertex buffer
        vertexBuffer->unlock();

        // disable all actived texture units
        for (DynamicArray<u32>::const_iterator itTextureUnit = m_activeTextureUnits.begin(); itTextureUnit != m_activeTextureUnits.end(); ++itTextureUnit)
        {
          // disable texturing on server side
          activateTextureUnit(*itTextureUnit);
          glDisable(GL_TEXTURE_2D);

          // disable texturing data on client side
          glClientActiveTexture(GL_TEXTURE0 + *itTextureUnit);
          glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
        m_activeTextureUnits.clear();

        // clean up
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisable(GL_BLEND);
        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
      }
    }

    // clear render queue
    queue->clear();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Applies material. */
void RendererPrivate::applyMaterial(const PMaterial& material)
{
  // disable blending by default
  glDisable(GL_BLEND);

  if (material)
  {
    // enable blending if necessary
    if ((EGEGraphics::BF_ONE != material->srcBlendFactor()) || (EGEGraphics::BF_ZERO != material->dstBlendFactor()))
    {
      glEnable(GL_BLEND);
      glBlendFunc(MapBlendFactor(material->srcBlendFactor()), MapBlendFactor(material->dstBlendFactor()));
    }

    // set vertex color
    // NOTE: this will be overriden if color array is activated
    glColor4f(material->diffuseColor().red, material->diffuseColor().green, material->diffuseColor().blue, material->diffuseColor().alpha);

    // go thru all textures
    for (u32 i = 0; i < material->textureCount(); ++i)
    {
      Object* texture = material->texture(i).object();

      // check if 2D texture
      if (EGE_OBJECT_UID_TEXTURE_2D == texture->uid())
      {
        Texture2D* tex2d = (Texture2D*) texture;

        activateTextureUnit(i);
        bindTexture(GL_TEXTURE_2D, tex2d->p_func()->id());

        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
      }
      // check if texture image
      else if (EGE_OBJECT_UID_TEXTURE_IMAGE == texture->uid())
      {
        TextureImage* texImg = (TextureImage*) texture;
        Texture2D* tex2d = (Texture2D*) texImg->texture().object();

        activateTextureUnit(i);
        bindTexture(GL_TEXTURE_2D, tex2d->p_func()->id());

        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, MapPrimitiveType(texImg->environmentMode()));

        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
        glTranslatef(texImg->rect().x, texImg->rect().y, 0.0f);
        glScalef(texImg->rect().width, texImg->rect().height, 1.0f);
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Activates given texture unit. */
bool RendererPrivate::activateTextureUnit(u32 unit)
{
  // check if unit available
  if (unit < Device::TextureUnitsCount())
  {
    // add to active texture units pool if not present there yet
    // NOTE: we add it here and not on GL call cause if we are about to activate same texture unit as current one we dont want GL code to be executed
    //       Though, we want to know it was requested to be activated for further use
    if (!m_activeTextureUnits.contains(unit))
    {
      m_activeTextureUnits.push_back(unit);
    }

    // check if currently selected other texture unit
    if (m_activeTextureUnit != unit)
    {
      // check if multitexture available
      glActiveTexture(GL_TEXTURE0 + unit);
      m_activeTextureUnit = unit;

      return GL_NO_ERROR == glGetError();
    }

    // same as current one
    return true;
  }

  // out of range
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Binds texture to target. */
bool RendererPrivate::bindTexture(GLenum target, GLuint textureId)
{
  // enable target first
  glEnable(target);
  if (GL_NO_ERROR != glGetError())
  {
    // error!
    return false;
  }

  // map texture target into texture binding query value
  GLenum textureBinding;
  switch (target)
  {
    case GL_TEXTURE_2D: textureBinding = GL_TEXTURE_BINDING_2D; break;

    default:

      EGE_ASSERT("Incorrect texture binding!");
  }

  // query texture Id bound to given target
  GLint boundTextureId;
	glGetIntegerv(textureBinding, &boundTextureId);

  // check if different texture bound currently
  if (static_cast<GLuint>(boundTextureId) != textureId)
  {
    // bind new texture to target
    glBindTexture(target, textureId);

    return GL_NO_ERROR == glGetError();
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Detects rendering capabilities. */
void RendererPrivate::detectCapabilities()
{
	GLint value;

  // get number of texture units
	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &value);
  Device::SetTextureUnitsCount(static_cast<u32>(value));

  // detect maximal texture size
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);
  Device::SetTextureMaxSize(static_cast<u32>(value));

  if (isExtensionSupported("GL_APPLE_texture_2D_limited_npot"))
  {
    Device::SetRenderCapability(EGEDevice::RENDER_CAPS_APPLE_LIMITED_NPOT_TEXTURE, true);
  }

  // multitexturing is supported by default
  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_MULTITEXTURE, true);

  // check if frame buffer object is supported
  if (isExtensionSupported("GL_OES_framebuffer_object"))
  {
    glBindFramebuffer         = (PFNGLBINDFRAMEBUFFEROESPROC) eglGetProcAddress("glBindFramebufferOES");
    glDeleteFramebuffers      = (PFNGLDELETEFRAMEBUFFERSOESPROC) eglGetProcAddress("glDeleteFramebuffersOES");
    glGenFramebuffers         = (PFNGLGENFRAMEBUFFERSOESPROC) eglGetProcAddress("glGenFramebuffersOES");
    glCheckFramebufferStatus  = (PFNGLCHECKFRAMEBUFFERSTATUSOESPROC) eglGetProcAddress("glCheckFramebufferStatusOES");
    glFramebufferTexture2D    = (PFNGLFRAMEBUFFERTEXTURE2DOESPROC) eglGetProcAddress("glFramebufferTexture2DOES");

    if (glBindFramebuffer && glDeleteFramebuffers && glGenFramebuffers && glCheckFramebufferStatus && glFramebufferTexture2D)
    {
      Device::SetRenderCapability(EGEDevice::RENDER_CAPS_FBO, true);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Checks if given extension is supported. */
bool RendererPrivate::isExtensionSupported(const char* extension) const
{
  const GLubyte* extensions = NULL;
  const GLubyte* start;

  GLubyte* where;
  GLubyte* terminator;

  // extension names should not have spaces
  where = (GLubyte*) strchr(extension, ' ');
  if (where || '\0' == *extension)
  {
    return false;
  }

  // get extensions string
  extensions = glGetString(GL_EXTENSIONS);

  // it takes a bit of care to be fool-proof about parsing the OpenGL extensions string. Don't be fooled by sub-strings, etc.
  start = extensions;
  for (;;) 
  {
    where = (GLubyte*) strstr((const char *) start, extension);
    if (!where)
    {
      break;
    }

    terminator = where + strlen(extension);

    if (where == start || *(where - 1) == ' ')
    {
      if (*terminator == ' ' || *terminator == '\0')
      {
        // found
        return true;
      }
    }

    start = terminator;
  }

  // not found
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
