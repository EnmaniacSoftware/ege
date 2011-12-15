#include <EGEApplication.h>
#include "Core/Graphics/OpenGL/GL 2.0/RendererOGL2_p.h"
#include "Core/Graphics/OpenGL/ExtensionsOGL.h"
#include "Core/Components/Render/RenderComponent.h"
#include "Core/Graphics/Viewport.h"
#include "Core/Graphics/Camera.h"
#include "Core/Graphics/IndexBuffer.h"
#include "Core/Graphics/VertexBuffer.h"
#include "Core/Graphics/Material.h"
#include "Core/Graphics/OpenGL/Texture2DOGL.h"
#include "Core/Graphics/Render/RenderWindow.h"
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
    case EGEGraphics::RPT_POINTS:           return GL_POINTS;
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
static GLenum MapIndexSize(EGEIndexBuffer::IndexSize size)
{
  switch (size)
  {
    case EGEIndexBuffer::IS_8BIT:   return GL_UNSIGNED_BYTE;
    case EGEIndexBuffer::IS_16BIT:  return GL_UNSIGNED_SHORT;
    case EGEIndexBuffer::IS_32BIT:  return GL_UNSIGNED_INT;
  }

  // default
  return GL_UNSIGNED_INT;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RendererPrivate::RendererPrivate(Renderer* base) : m_d(base), m_activeTextureUnit(0xffffffff)
{
  detectCapabilities();

  //glEnable(GL_CULL_FACE);
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

  // set scissor
  glScissor((GLint) actualRect.x, (GLint) actualRect.y, (GLsizei) actualRect.width, (GLsizei) actualRect.height);
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

      PVertexBuffer& vertexBuffer = data.component->vertexBuffer();
      PIndexBuffer& indexBuffer   = data.component->indexBuffer();
      PMaterial& material         = data.component->material();

      // apply general params
      applyGeneralParams(data.component);

      if (data.component->name() == "lightning-effect-lines-1")
      {
        int a = 1;
      }

      // bind buffers
      //if (!vertexBuffer->bind())
      //{
      //  // error!
      //  EGE_PRINT("RendererPrivate::flush - could not bind vertex buffer");
      //  break;
      //}

      // TAGE - index buffer
      // ..

      // lock buffers
      // NOTE: Data is actually locked only for software buffers. 
      //       For hardware ones, we set it to 0 (for arithmetics) as VBOs require offsets to be used rather than pointers.
      void* vertexData = vertexBuffer->lock(0, vertexBuffer->vertexCount());
      void* indexData = indexBuffer->lock(0, indexBuffer->indexCount());

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

                for (s32 i = 0; i < textureCount; ++i)
                {
                  if (glClientActiveTexture)
                  {
                    glClientActiveTexture(GL_TEXTURE0 + i);
                  }

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
	        //  glClientActiveTexture(GL_TEXTURE0);

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
            if (Device::HasRenderCapability(EGEDevice::RENDER_CAPS_MULTITEXTURE))
            {
              glClientActiveTexture(GL_TEXTURE0 + *itTextureUnit);
            }

            // disable point sprites
            if (EGEGraphics::RPT_POINTS == data.component->primitiveType())
            {
              if (Device::HasRenderCapability(EGEDevice::RENDER_CAPS_POINT_SPRITE))
              {
                glDisable(GL_POINT_SPRITE_ARB);
              }
            }

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

      // unlock vertex buffers
      // NOTE: Data is actually unlocked only for software buffers. 
    //  if (EGE_OBJECT_UID_VERTEX_BUFFER == vertexBuffer->uid())
      {
        vertexBuffer->unlock();
      }

      indexBuffer->unlock();
    }

    // clear render queue
    queue->clear();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Applies parameters for given pass. */
void RendererPrivate::applyPassParams(const PRenderComponent& component, const PMaterial& material, const RenderPass* pass)
{
  // disable blending by default
  glDisable(GL_BLEND);

  if (NULL != pass)
  {
    // enable blending if necessary
    if ((EGEGraphics::BF_ONE != pass->srcBlendFactor()) || (EGEGraphics::BF_ZERO != pass->dstBlendFactor()))
    {
      glEnable(GL_BLEND);
      if (component->name() == "mode-selection-screen-chimney-smoke")
      {
        int a = 1;
      //  glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
      //  glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ONE);
      }
      //else
      glBlendFunc(MapBlendFactor(pass->srcBlendFactor()), MapBlendFactor(pass->dstBlendFactor()));
    }

    // set vertex color
    // NOTE: this will be overriden if color array is activated
    glColor4f(pass->diffuseColor().red, pass->diffuseColor().green, pass->diffuseColor().blue, pass->diffuseColor().alpha);

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

        //if (texImg->environmentMode() == EGETexture::EM_DECAL)
        //{
        //     glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
        //     glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);   //Interpolate RGB with RGB
        //     glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_PREVIOUS);
        //     glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_TEXTURE);
        //     //GL_CONSTANT refers to the call we make with glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, mycolor)
        //     glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB, GL_CONSTANT);
        //     glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
        //     glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
        //     glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_CONSTANT );
        //     //------------------------
        //     glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_INTERPOLATE);   //Interpolate ALPHA with ALPHA
        //     glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_PREVIOUS);
        //     glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA, GL_TEXTURE);
        //     //GL_CONSTANT refers to the call we make with glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, mycolor)
        //     glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_ALPHA, GL_CONSTANT);
        //     glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
        //     glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);
        //     glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_ALPHA, GL_SRC_ALPHA);

        //     static float a = 0.0f;
        //     a += 0.01f;
        //     if (a > 1)
        //     {
        //       a = 0;
        //     }

        //    float mycolor[4];
        //     mycolor[0]=mycolor[1]=mycolor[2]=0.0;    //RGB doesn't matter since we are not using it
        //     mycolor[3]=0.5;                         //Set the blend factor with this
        //     glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, mycolor);
        //}
        //else
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, MapPrimitiveType(texImg->environmentMode()));

        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
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
      if (Device::HasRenderCapability(EGEDevice::RENDER_CAPS_MULTITEXTURE))
      {
        glActiveTexture(GL_TEXTURE0 + unit);
        m_activeTextureUnit = unit;

        return GL_NO_ERROR == glGetError();
      }
      else if (0 == unit)
      {
        // unit 0 is the only valid option if no multitexturing is present. Thus, no special activation is required
        m_activeTextureUnit = unit;
        return true;
      }
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
    case GL_TEXTURE_1D: textureBinding = GL_TEXTURE_BINDING_1D; break;
    case GL_TEXTURE_2D: textureBinding = GL_TEXTURE_BINDING_2D; break;
    case GL_TEXTURE_3D: textureBinding = GL_TEXTURE_BINDING_3D; break;

    default:

      EGE_ASSERT(false && "Incorrect texture binding!");
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

  // check if multitexturing is supported
  if (isExtensionSupported("GL_ARB_multitexture"))
  {
    glClientActiveTexture = (PFNGLCLIENTACTIVETEXTUREARBPROC) wglGetProcAddress("glClientActiveTextureARB");
    glActiveTexture       = (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");

    if (glClientActiveTexture && glActiveTexture)
    {
      Device::SetRenderCapability(EGEDevice::RENDER_CAPS_MULTITEXTURE, true);
    }
  }

  // check if frame buffer object is supported
  if (isExtensionSupported("GL_EXT_framebuffer_object"))
  {
    glBindFramebuffer         = (PFNGLBINDFRAMEBUFFEREXTPROC) wglGetProcAddress("glBindFramebufferEXT");
    glDeleteFramebuffers      = (PFNGLDELETEFRAMEBUFFERSEXTPROC) wglGetProcAddress("glDeleteFramebuffersEXT");
    glGenFramebuffers         = (PFNGLGENFRAMEBUFFERSEXTPROC) wglGetProcAddress("glGenFramebuffersEXT");
    glCheckFramebufferStatus  = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC) wglGetProcAddress("glCheckFramebufferStatusEXT");
    glFramebufferTexture2D    = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC) wglGetProcAddress("glFramebufferTexture2DEXT");

    if (glBindFramebuffer && glDeleteFramebuffers && glGenFramebuffers && glCheckFramebufferStatus && glFramebufferTexture2D)
    {
      Device::SetRenderCapability(EGEDevice::RENDER_CAPS_FBO, true);
    }
  }

  // check if combine texture environment mode is supported
  if (isExtensionSupported("GL_ARB_texture_env_combine"))
  {
    Device::SetRenderCapability(EGEDevice::RENDER_CAPS_COMBINE_TEXTURE_ENV, true);
  }

  // check if vertex buffer object is supported
  if (isExtensionSupported("GL_ARB_vertex_buffer_object"))
  {
    glGenBuffers    = (PFNGLGENBUFFERSPROC) wglGetProcAddress("glGenBuffersARB");
    glBindBuffer    = (PFNGLBINDBUFFERPROC) wglGetProcAddress("glBindBufferARB");
    glBufferData    = (PFNGLBUFFERDATAPROC) wglGetProcAddress("glBufferDataARB");
    glBufferSubData = (PFNGLBUFFERSUBDATAPROC) wglGetProcAddress("glBufferSubDataARB");
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC) wglGetProcAddress("glDeleteBuffersARB");
    glMapBuffer     = (PFNGLMAPBUFFERPROC) wglGetProcAddress("glMapBufferARB");
    glUnmapBuffer   = (PFNGLUNMAPBUFFERPROC) wglGetProcAddress("glUnmapBufferARB");

    if (glGenBuffers && glBindBuffer && glBufferData && glBufferSubData && glDeleteBuffers && glMapBuffer && glUnmapBuffer)
    {
      Device::SetRenderCapability(EGEDevice::RENDER_CAPS_VBO, true);
    }
  }

  // check if point sprites are supported
  if (isExtensionSupported("GL_ARB_point_parameters") && isExtensionSupported("GL_ARB_point_sprite"))
  {
    glPointParameterf  = (PFNGLPOINTPARAMETERFARBPROC) wglGetProcAddress("glPointParameterfARB");
    glPointParameterfv = (PFNGLPOINTPARAMETERFVARBPROC) wglGetProcAddress("glPointParameterfvARB");

    if (glPointParameterf && glPointParameterfv)
    {
      Device::SetRenderCapability(EGEDevice::RENDER_CAPS_POINT_SPRITE, true);
    }
  }

  // check if min and max blending functions are supported
  if (isExtensionSupported("GL_ARB_imaging") || isExtensionSupported("GL_EXT_blend_minmax"))
  {
    glBlendEquation = (PFNGLBLENDEQUATIONPROC) wglGetProcAddress("glBlendEquationARB");
    if (!glBlendEquation)
    {
      glBlendEquation = (PFNGLBLENDEQUATIONPROC) wglGetProcAddress("glBlendEquationEXT");
    }

    if (glBlendEquation)
    {
      Device::SetRenderCapability(EGEDevice::RENDER_CAPS_BLEND_MINMAX, true);
    }

    glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC) wglGetProcAddress("glBlendFuncSeparateEXT");
  }

  // Point sprite size array is not supported by default
  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_POINT_SPRITE_SIZE, false);
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
/*! Applies general parameters. 
 *  @note General parameters are the ones that require only one setup before component is rendered. 
 */
void RendererPrivate::applyGeneralParams(const PRenderComponent& component)
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
    clipRect = d_func()->applyRotation(clipRect, -d_func()->m_renderTarget->orientationRotation());

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
