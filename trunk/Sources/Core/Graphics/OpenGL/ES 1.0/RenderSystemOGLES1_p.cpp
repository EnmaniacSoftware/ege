#include <EGEApplication.h>
#include "Core/Graphics/OpenGL/ES 1.0/RenderSystemOGLES1_p.h"
#include "Core/Graphics/OpenGL/ExtensionsOGLES.h"
#include "Core/Components/Render/RenderComponent.h"
#include "Core/Graphics/Viewport.h"
#include "Core/Graphics/Camera.h"
#include "Core/Graphics/Graphics.h"
#include "Core/Graphics/Program.h"
#include "Core/Graphics/Shader.h"
#include "Core/Graphics/OpenGL/IndexBufferVAOGL.h"
#include "Core/Graphics/OpenGL/IndexBufferVBOOGL.h"
#include "Core/Graphics/OpenGL/VertexBufferVAOGL.h"
#include "Core/Graphics/OpenGL/VertexBufferVBOOGL.h"
#include "Core/Graphics/OpenGL/RenderTextureCopyOGL.h"
#include "Core/Graphics/OpenGL/RenderTextureFBOOGL.h"
#include "Core/Graphics/Material.h"
#include "Core/Graphics/OpenGL/Texture2DOGL.h"
#include "Core/Graphics/TextureImage.h"
#include "Core/Graphics/Render/RenderQueue.h"
#include <EGETimer.h>
#include <EGEDevice.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(RenderSystemPrivate)
EGE_DEFINE_DELETE_OPERATORS(RenderSystemPrivate)
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
    case EGETexture::EM_COMBINE:  result = GL_COMBINE; break;
    case EGETexture::EM_DECAL:    result = GL_DECAL; break;
    case EGETexture::EM_MODULATE: result = GL_MODULATE; break;
    case EGETexture::EM_REPLACE:  result = GL_REPLACE; break;
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
    case EGEIndexBuffer::IS_32BIT:  result = GL_UNSIGNED_INT; break;
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
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderSystemPrivate::RenderSystemPrivate(RenderSystem* base) : m_d(base), 
                                                               m_activeTextureUnit(0xffffffff)
{
  detectCapabilities();
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

  glClear(bits);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemPrivate::setViewport(const PViewport& viewport)
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
  glViewport(static_cast<GLint>(actualRect.x), static_cast<GLint>(actualRect.y), 
             static_cast<GLsizei>(actualRect.width), static_cast<GLsizei>(actualRect.height));

  // set scissor
  glScissor(static_cast<GLint>(actualRect.x), static_cast<GLint>(actualRect.y), 
            static_cast<GLsizei>(actualRect.width), static_cast<GLsizei>(actualRect.height));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemPrivate::setRenderTarget(const PRenderTarget& renderTarget)
{
  // unbind current render target
  if (d_func()->m_renderTarget)
  {
    d_func()->m_renderTarget->unbind();
  }

  d_func()->m_renderTarget = renderTarget;

  // bind new target
  d_func()->m_renderTarget->bind();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemPrivate::flush()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // NOTE: actual OGLES rotation should be opposite
  glRotatef(-d_func()->m_renderTarget->orientationRotation().degrees(), 0, 0, 1);
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

      //if (data.component->name() == "mode-selection-screen-chimney-smoke")
      //{
      //  int a = 1;
      //}

      PVertexBuffer vertexBuffer = data.component->vertexBuffer();
      PIndexBuffer indexBuffer   = data.component->indexBuffer();
      PMaterial material         = data.component->material();

      // determine number of texture arrays
      s32 textureArraysCount = vertexBuffer->arrayCount(EGEVertexBuffer::AT_TEXTURE_UV);

      // bind vertex and index buffers
      void* vertexData = bindVertexBuffer(vertexBuffer);
      void* indexData  = bindIndexBuffer(indexBuffer);

      // apply general params
      applyGeneralParams(data.component);

      // go thru all passes
      // NOTE: if there is no material, we consider it 1 pass
      u32 passes = material ? material->passCount() : 1;
      for (u32 pass = 0; pass < passes; ++pass)
      {
        const RenderPass* renderPass = material ? material->pass(pass) : NULL;

        // check if there is anything to be rendered
        // TAGE - is it overhead setting up whole geometry for each pass ? or should it be done only once ? what with texturing ?
        if (0 != vertexBuffer->vertexCount())
        {
          const EGEVertexBuffer::SemanticArray& semantics = vertexBuffer->semantics();

          // apply pass related params
          applyPassParams(data.component, material, renderPass);

          // NOTE: change to modelview after material is applied as it may change current matrix mode
          glMatrixMode(GL_MODELVIEW);

          // determine texture count
          s32 textureCount = renderPass ? renderPass->textureCount() : 0;

          u32 value = (0 < indexBuffer->indexCount()) ? indexBuffer->indexCount() : vertexBuffer->vertexCount();

          d_func()->m_vertexCount += value;
          d_func()->m_batchCount++;

          // go thru all buffers
          s32 textureUnitsActivated = 0;
          for (EGEVertexBuffer::SemanticArray::const_iterator itSemantic = semantics.begin(); itSemantic != semantics.end(); ++itSemantic)
          {
            // set according to buffer type
            switch (itSemantic->type)
            {
              case EGEVertexBuffer::AT_POSITION_XYZ:

                glVertexPointer(3, GL_FLOAT, vertexBuffer->vertexSize(), static_cast<s8*>(vertexData) + itSemantic->offset);
                glEnableClientState(GL_VERTEX_ARRAY);
                break;

              case EGEVertexBuffer::AT_POSITION_XY:

                glVertexPointer(2, GL_FLOAT, vertexBuffer->vertexSize(), static_cast<s8*>(vertexData) + itSemantic->offset);
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

                // check if number of texture arrays is exactly the same as texture units in a current pass
                if (textureArraysCount == textureCount)
                {
                  glClientActiveTexture(GL_TEXTURE0 + textureUnitsActivated);

                  glTexCoordPointer(2, GL_FLOAT, vertexBuffer->vertexSize(), static_cast<s8*>(vertexData) + itSemantic->offset);
                  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

                  ++textureUnitsActivated;
                }
                // check if there is more texture arrays than texture units
                else if (textureArraysCount > textureCount)
                {
                  // check if still some texture unit is to be set
                  if (textureUnitsActivated < textureCount)
                  {
                    glClientActiveTexture(GL_TEXTURE0 + textureUnitsActivated);

                    glTexCoordPointer(2, GL_FLOAT, vertexBuffer->vertexSize(), static_cast<s8*>(vertexData) + itSemantic->offset);
                    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

                    ++textureUnitsActivated;
                  }
                }
                // check if there are more texture units than texture arrays
                else if (textureArraysCount < textureCount)
                {
                  // check if this is last texture array
                  if ((textureUnitsActivated + 1) == textureArraysCount)
                  {
                    // set current texture array to all remaining texture units
                    while (textureUnitsActivated != textureCount)
                    {
                      glClientActiveTexture(GL_TEXTURE0 + textureUnitsActivated);

                      glTexCoordPointer(2, GL_FLOAT, vertexBuffer->vertexSize(), static_cast<s8*>(vertexData) + itSemantic->offset);
                      glEnableClientState(GL_TEXTURE_COORD_ARRAY);

                      ++textureUnitsActivated;
                    }
                  }
                  else
                  {
                    // set current texture array to corresponding texture unit
                    glClientActiveTexture(GL_TEXTURE0 + textureUnitsActivated);

                    glTexCoordPointer(2, GL_FLOAT, vertexBuffer->vertexSize(), static_cast<s8*>(vertexData) + itSemantic->offset);
                    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

                    ++textureUnitsActivated;
                  }
                }
                break;

              case EGEVertexBuffer::AT_POINT_SPRITE_SIZE:

                glPointSizePointerOES(GL_FLOAT, vertexBuffer->vertexSize(), static_cast<s8*>(vertexData) + itSemantic->offset);
                glEnableClientState(GL_POINT_SIZE_ARRAY_ARB);                
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
            // render only if there is anything to render
            glDrawElements(MapPrimitiveType(data.component->primitiveType()), indexBuffer->indexCount(), MapIndexSize(indexBuffer->size()), indexData);
          }
          else
          {
            // render only if there is anything to render
            glDrawArrays(MapPrimitiveType(data.component->primitiveType()), 0, vertexBuffer->vertexCount());
          }

          // disable all actived texture units
          for (DynamicArray<u32>::const_iterator itTextureUnit = m_activeTextureUnits.begin(); itTextureUnit != m_activeTextureUnits.end(); ++itTextureUnit)
          {
            // disable texturing on server side
            activateTextureUnit(*itTextureUnit);
            glDisable(GL_TEXTURE_2D);

            // disable texturing data on client side
            glClientActiveTexture(GL_TEXTURE0 + *itTextureUnit);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);

            // disable point sprites
            if (EGEGraphics::RPT_POINTS == data.component->primitiveType())
            {
              glDisable(GL_POINT_SPRITE_ARB);
            }
          }
          m_activeTextureUnits.clear();

          // clean up
          glDisableClientState(GL_VERTEX_ARRAY);
          glDisableClientState(GL_NORMAL_ARRAY);
          glDisableClientState(GL_COLOR_ARRAY);
          //glDisableClientState(GL_POINT_SIZE_ARRAY_ARB);                
          glDisable(GL_BLEND);
          glMatrixMode(GL_TEXTURE);
        }
      }

      // unbind vertex and index buffers
      unbindVertexBuffer(vertexBuffer);
      unbindIndexBuffer(indexBuffer);
    }

    // clear render queue
    queue->clear();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemPrivate::applyPassParams(const PRenderComponent& component, const PMaterial& material, const RenderPass* pass)
{
  // disable blending by default
  glDisable(GL_BLEND);

  if (NULL != pass)
  {
    // enable blending if necessary
    if ((EGEGraphics::BF_ONE != pass->srcBlendFactor()) || (EGEGraphics::BF_ZERO != pass->dstBlendFactor()))
    {
      glEnable(GL_BLEND);
     
      //if (pass->srcBlendFactor() == EGEGraphics::BF_SRC_ALPHA && pass->dstBlendFactor() == EGEGraphics::BF_ONE_MINUS_SRC_ALPHA)
      //{
      //  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
      //}
      //else
      glBlendFunc(MapBlendFactor(pass->srcBlendFactor()), MapBlendFactor(pass->dstBlendFactor()));
    }

    // set vertex color
    // NOTE: this will be overriden if color array is activated
    Color color = pass->diffuseColorTransformation().transform(pass->diffuseColor());
    glColor4f(color.red, color.green, color.blue, color.alpha);

    // go thru all textures
    for (u32 i = 0; i < pass->textureCount(); ++i)
    {
      Object* texture = pass->texture(i).object();

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
        
        // NOTE: it is possible texure object might be not present ie when it is manual and hasnt been set yet
        if (tex2d)
        {
          bindTexture(GL_TEXTURE_2D, tex2d->p_func()->id());
        }

        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, MapPrimitiveType(texImg->environmentMode()));

        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();

        float32 degrees = texImg->rotationAngle().degrees();

        if (0.0f != degrees)
        {
          glTranslatef(texImg->rect().x + texImg->rect().width * 0.5f, texImg->rect().y + texImg->rect().height * 0.5f, 0.0f);
          glRotatef(degrees, 0.0f, 0.0f, 1.0f);
          glTranslatef(-(texImg->rect().x + texImg->rect().width * 0.5f), -(texImg->rect().y + texImg->rect().height * 0.5f), 0.0f);
        }

        glTranslatef(texImg->rect().x, texImg->rect().y, 0.0f);
        glScalef(texImg->rect().width, texImg->rect().height, 1.0f);
      }

      // check if points are be rendered
      if (EGEGraphics::RPT_POINTS == component->primitiveType())
      {
        // check if point sprites are supported
        if (Device::HasRenderCapability(EGEDevice::RENDER_CAPS_POINT_SPRITE))
        {
          glEnable(GL_POINT_SPRITE_ARB);

          glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
        }
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemPrivate::activateTextureUnit(u32 unit)
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

      OGL_CHECK();
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemPrivate::bindTexture(GLenum target, GLuint textureId)
{
  // enable target first
  glEnable(target);
  OGL_CHECK();

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
    OGL_CHECK();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemPrivate::detectCapabilities()
{
  // get list of all extensions
  String extensionString(reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS)));
  m_extensionArray = extensionString.split(" ");

  for (StringArray::const_iterator it = m_extensionArray.begin(); it != m_extensionArray.end(); ++it)
  {
    egeDebug() << "Available OGL extension:" << *it;
  }

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
    glBindFramebuffer         = reinterpret_cast<PFNGLBINDFRAMEBUFFEROESPROC>(eglGetProcAddress("glBindFramebufferOES"));
    glDeleteFramebuffers      = reinterpret_cast<PFNGLDELETEFRAMEBUFFERSOESPROC>(eglGetProcAddress("glDeleteFramebuffersOES"));
    glGenFramebuffers         = reinterpret_cast<PFNGLGENFRAMEBUFFERSOESPROC>(eglGetProcAddress("glGenFramebuffersOES"));
    glCheckFramebufferStatus  = reinterpret_cast<PFNGLCHECKFRAMEBUFFERSTATUSOESPROC>(eglGetProcAddress("glCheckFramebufferStatusOES"));
    glFramebufferTexture2D    = reinterpret_cast<PFNGLFRAMEBUFFERTEXTURE2DOESPROC>(eglGetProcAddress("glFramebufferTexture2DOES"));

    if ((NULL != glBindFramebuffer) && (NULL != glDeleteFramebuffers) && (NULL != glGenFramebuffers) && 
        (NULL != glCheckFramebufferStatus) && (NULL != glFramebufferTexture2D))
    {
      Device::SetRenderCapability(EGEDevice::RENDER_CAPS_FBO, true);
    }
  }

  // check if VBO mapping is supported
  if (isExtensionSupported("GL_OES_mapbuffer"))
  {
    glMapBuffer   = reinterpret_cast<PFNGLMAPBUFFEROESPROC>(eglGetProcAddress("glMapBufferOES"));
    glUnmapBuffer = reinterpret_cast<PFNGLUNMAPBUFFEROESPROC>(eglGetProcAddress("glUnmapBufferOES"));

    if ((NULL != glMapBuffer) && (NULL != glUnmapBuffer))
    {
      Device::SetRenderCapability(EGEDevice::RENDER_CAPS_MAP_BUFFER, true);
    }
  }

  // check if min and max blending functions are supported
  //if (isExtensionSupported("GL_ARB_imaging") || isExtensionSupported("GL_EXT_blend_minmax"))
  //{
  //  glBlendEquation = (PFNGLBLENDEQUATIONPROC) wglGetProcAddress("glBlendEquationARB");
  //  if (!glBlendEquation)
  //  {
  //    glBlendEquation = (PFNGLBLENDEQUATIONPROC) wglGetProcAddress("glBlendEquationEXT");
  //  }

  //  if (glBlendEquation)
  //  {
  //    Device::SetRenderCapability(EGEDevice::RENDER_CAPS_BLEND_MINMAX, true);
  //  }
  //}
  
  // Combine texture environment mode is supported by default
  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_COMBINE_TEXTURE_ENV, true);

  // Point sprite is supported by defult
  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_POINT_SPRITE, true);

  // Point sprite size array is supported by default
  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_POINT_SPRITE_SIZE, true);

  // Vertex Buffer Objects is supported by default
  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_VBO, true);

  // check for texture compressions support
  if (isExtensionSupported("GL_IMG_texture_compression_pvrtc"))
  {
    Device::SetRenderCapability(EGEDevice::RENDER_CAPS_TEXTURE_COMPRESSION_PVRTC, true);
  }

  if (isExtensionSupported("GL_EXT_texture_compression_s3tc"))
  {
    Device::SetRenderCapability(EGEDevice::RENDER_CAPS_TEXTURE_COMPRESSION_S3TC, true);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool RenderSystemPrivate::isExtensionSupported(const char* extension) const
{
  return m_extensionArray.contains(extension);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemPrivate::applyGeneralParams(const PRenderComponent& component)
{
  // apply scissor test
  if (component->clipRect().isNull())
  {
    glDisable(GL_SCISSOR_TEST);
  }
  else
  {
    glEnable(GL_SCISSOR_TEST);

    Rectf clipRect = component->clipRect();

    // check if conversion from "upper-left" corner to "lower-left" for current orientation is necessary
    if (!d_func()->m_renderTarget->requiresTextureFlipping())
    {
      // convert "upper-left" corner to "lower-left"
      clipRect.y = d_func()->m_renderTarget->height() - clipRect.height - clipRect.y;
    }

    // apply opposite rotation to rectangle to convert it into native (non-transformed) coordinate
    //EGE_PRINT("ORIENTATION: %f %f", d_func()->m_renderTarget->orientationRotation().radians(), d_func()->m_renderTarget->orientationRotation().degrees());

    clipRect = d_func()->applyRotation(clipRect, d_func()->m_renderTarget->orientationRotation());

    // apply zoom
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
void* RenderSystemPrivate::bindVertexBuffer(PVertexBuffer& buffer) const
{
  void* data = NULL;

  // process if any vertices exist
  if (0 < buffer->vertexCount())
  {
    // process according to buffer type
    switch (buffer->uid())
    {
      case EGE_OBJECT_UID_VERTEX_BUFFER_VA:

        // lock buffer to get pointer to first element
        data = buffer->lock(0, buffer->vertexCount());
        break;

      case EGE_OBJECT_UID_VERTEX_BUFFER_VBO:

        // bind VBO
        static_cast<VertexBufferVBO*>(buffer.object())->bind();

        // set vertex data base to 0 as for VBO we use offsets
        data = 0;
        break;

      default:

        EGE_ASSERT(false && "Invalid vertex buffer type");
        break;
    }
  }

  return data;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemPrivate::unbindVertexBuffer(PVertexBuffer& buffer) const
{
  // process according to buffer type
  switch (buffer->uid())
  {
    case EGE_OBJECT_UID_VERTEX_BUFFER_VA:

      // unlock buffer
      buffer->unlock(NULL);
      break;

    case EGE_OBJECT_UID_VERTEX_BUFFER_VBO:

      // unbind VBO
      static_cast<VertexBufferVBO*>(buffer.object())->unbind();
      break;

    default:

      EGE_ASSERT(false && "Invalid vertex buffer type");
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* RenderSystemPrivate::bindIndexBuffer(PIndexBuffer& buffer) const
{
  void* data = NULL;

  // process if any indicies exists
  if (0 < buffer->indexCount())
  {
    // process according to buffer type
    switch (buffer->uid())
    {
      case EGE_OBJECT_UID_INDEX_BUFFER_VA:

        // lock buffer to get pointer to first element
        data = buffer->lock(0, buffer->indexCount());
        break;

      case EGE_OBJECT_UID_INDEX_BUFFER_VBO:

        // bind VBO
        static_cast<IndexBufferVBO*>(buffer.object())->bind();

        // set index data base to 0 as for VBO we use offsets
        data = 0;
        break;

      default:

        EGE_ASSERT(false && "Invalid index buffer type");
        break;
    }
  }

  return data;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemPrivate::unbindIndexBuffer(PIndexBuffer& buffer) const
{
  // process according to buffer type
  switch (buffer->uid())
  {
    case EGE_OBJECT_UID_INDEX_BUFFER_VA:

      // unlock buffer
      buffer->unlock(NULL);
      break;

    case EGE_OBJECT_UID_INDEX_BUFFER_VBO:

      // unbind VBO
      static_cast<IndexBufferVBO*>(buffer.object())->unbind();
      break;

    default:

      EGE_ASSERT(false && "Invalid index buffer type");
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PVertexBuffer RenderSystemPrivate::createVertexBuffer(EGEVertexBuffer::UsageType usage) const
{
  PVertexBuffer buffer;

  if (!Device::HasRenderCapability(EGEDevice::RENDER_CAPS_VBO))
  {
    buffer = ege_new VertexBufferVBO(d_func()->app(), usage);
    if (NULL != buffer)
    {
      VertexBufferVBO* bufferVBO = ege_cast<EGE::VertexBufferVBO*>(buffer);

      // generate OGL buffer
      glGenBuffers(1, &bufferVBO->m_id);
      OGL_CHECK();

      // check if failed
      if (0 == bufferVBO->m_id)
      {
        // error!
        buffer = NULL;
      }
    }
  }
  else
  {
    buffer = ege_new VertexBufferVA(d_func()->app());
  }

  return buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemPrivate::destroyVertexBuffer(PVertexBuffer object) const
{
  if (EGE_OBJECT_UID_VERTEX_BUFFER_VBO == object->uid())
  {
    VertexBufferVBO* bufferVBO = ege_cast<EGE::VertexBufferVBO*>(object);
 
    glDeleteBuffers(1, &bufferVBO->m_id);
    OGL_CHECK();
    bufferVBO->m_id = 0;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PIndexBuffer RenderSystemPrivate::createIndexBuffer(EGEIndexBuffer::UsageType usage) const
{
  PIndexBuffer buffer;

  if (!Device::HasRenderCapability(EGEDevice::RENDER_CAPS_VBO))
  {
    buffer = ege_new IndexBufferVBO(d_func()->app(), usage);
    if (NULL != buffer)
    {
      IndexBufferVBO* bufferVBO = ege_cast<EGE::IndexBufferVBO*>(buffer);

      // generate OGL buffer
      glGenBuffers(1, &bufferVBO->m_id);
      OGL_CHECK();

      // check if failed
      if (0 == bufferVBO->m_id)
      {
        // error!
        buffer = NULL;
      }
    }
  }
  else
  {
    buffer = ege_new IndexBufferVA(d_func()->app());
  }

  return buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemPrivate::destroyIndexBuffer(PIndexBuffer object) const
{
  if (EGE_OBJECT_UID_INDEX_BUFFER_VBO == object->uid())
  {
    IndexBufferVBO* bufferVBO = ege_cast<EGE::IndexBufferVBO*>(object);
 
    glDeleteBuffers(1, &bufferVBO->m_id);
    OGL_CHECK();
    bufferVBO->m_id = 0;
  }
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
  egeWarning() << "Texture" << name << "uploaded:" << endTime - startTime << "microseconds";

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

  egeWarning() << "Creating render target done" << texture->m_target;

  // add into render targets
  d_func()->app()->graphics()->registerRenderTarget(texture->m_target);

  return texture;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemPrivate::destroyTexture2D(PTexture2D texture)
{
  if (0 != texture->p_func()->m_id)
  {
    egeDebug() << "Destroying texture" << texture->p_func()->m_id << texture->name();
  
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

  return texture;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PShader RenderSystemPrivate::createShader(EGEGraphics::ShaderType type, const String& name, const PDataBuffer& data)
{
  EGE_UNUSED(type);
  EGE_UNUSED(name);
  EGE_UNUSED(data);

  // not available
  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemPrivate::destroyShader(PShader shader)
{
  EGE_UNUSED(shader);

  // not available
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PProgram RenderSystemPrivate::createProgram(const String& name, const List<PShader>& shaders)
{
  EGE_UNUSED(name);
  EGE_UNUSED(shaders);

  // not available
  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemPrivate::destroyProgram(PProgram program)
{
  EGE_UNUSED(program);

  // not available
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END