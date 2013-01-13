#include <EGEApplication.h>
#include <EGEOpenGL.h>
#include "Core/Graphics/OpenGL/GL 2.0/RenderSystemOGL2_p.h"
#include "Core/Graphics/OpenGL/ExtensionsOGL.h"
#include "Core/Components/Render/RenderComponent.h"
#include "Core/Graphics/Viewport.h"
#include "Core/Graphics/Camera.h"
#include "Core/Graphics/Graphics.h"
#include "Core/Graphics/OpenGL/IndexBufferVAOGL.h"
#include "Core/Graphics/OpenGL/IndexBufferVBOOGL.h"
#include "Core/Graphics/OpenGL/VertexBufferVAOGL.h"
#include "Core/Graphics/OpenGL/VertexBufferVBOOGL.h"
#include "Core/Graphics/OpenGL/RenderTextureCopyOGL.h"
#include "Core/Graphics/OpenGL/RenderTextureFBOOGL.h"
#include "Core/Graphics/Material.h"
#include "Core/Graphics/OpenGL/Texture2DOGL.h"
#include "Core/Graphics/Render/RenderWindow.h"
#include "Core/Graphics/TextureImage.h"
#include "Core/Graphics/Render/RenderQueue.h"
#include <EGEDevice.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(RenderSystemPrivate)
EGE_DEFINE_DELETE_OPERATORS(RenderSystemPrivate)
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
/*! Maps texture filtering mode to OpenGL compilant value. */
static GLint MapTextureFilter(EGETexture::Filter filter)
{
  switch (filter)
  {
    case EGETexture::BILINEAR:          return GL_NEAREST;
    case EGETexture::TRILINEAR:         return GL_LINEAR;
    case EGETexture::MIPMAP_BILINEAR:   return GL_LINEAR_MIPMAP_NEAREST;
    case EGETexture::MIPMAP_TRILINEAR:  return GL_LINEAR_MIPMAP_LINEAR;
  }

  return GL_NEAREST;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Maps texture addressing mode to OpenGL compilant value. */
// TAGE - when moved to OGLES1X remove #defines
static GLint MapTextureAddressingMode(EGETexture::AddressingMode mode)
{
  switch (mode)
  {
#ifndef EGE_RENDERING_OPENGLES_1
    case EGETexture::AM_CLAMP:  return GL_CLAMP;
#else
    case EGETexture::AM_CLAMP:  return GL_CLAMP_TO_EDGE;
#endif
    case EGETexture::AM_REPEAT: return GL_REPEAT;
  }

  return GL_REPEAT;
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
  glViewport((GLint) actualRect.x, (GLint) actualRect.y, (GLsizei) actualRect.width, (GLsizei) actualRect.height);

  // set scissor
  glScissor((GLint) actualRect.x, (GLint) actualRect.y, (GLsizei) actualRect.width, (GLsizei) actualRect.height);
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

 // EGE_LOG("---------------------- Render Flush begin ----------------------");

  // go thru all render queues
  for (Map<s32, PRenderQueue>::const_iterator itQueue = d_func()->m_renderQueues.begin(); itQueue != d_func()->m_renderQueues.end(); ++itQueue)
  {
    RenderQueue* queue = itQueue->second;

    // go thru all render data within current queue
    const MultiMap<u32, RenderQueue::SRENDERDATA>& renderData = queue->renderData();
    for (MultiMap<u32, RenderQueue::SRENDERDATA>::const_iterator it = renderData.begin(); it != renderData.end(); ++it)
    {
      const RenderQueue::SRENDERDATA& data = it->second;
      
    //  EGE_LOG("Q: %d, name: %s", itQueue->first, data.component->name().toAscii());

      //if (data.component->name() == "achievement-frame")
      //{
      //  int a = 1;
      //}

      PVertexBuffer& vertexBuffer = data.component->vertexBuffer();
      PIndexBuffer& indexBuffer   = data.component->indexBuffer();
      PMaterial& material         = data.component->material();

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
                  if (glClientActiveTexture)
                  {
                    glClientActiveTexture(GL_TEXTURE0 + textureUnitsActivated);
                  }

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
                    if (glClientActiveTexture)
                    {
                      glClientActiveTexture(GL_TEXTURE0 + textureUnitsActivated);
                    }

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
                      if (glClientActiveTexture)
                      {
                        glClientActiveTexture(GL_TEXTURE0 + textureUnitsActivated);
                      }

                      glTexCoordPointer(2, GL_FLOAT, vertexBuffer->vertexSize(), static_cast<s8*>(vertexData) + itSemantic->offset);
                      glEnableClientState(GL_TEXTURE_COORD_ARRAY);

                      ++textureUnitsActivated;
                    }
                  }
                  else
                  {
                    // set current texture array to corresponding texture unit
                    if (glClientActiveTexture)
                    {
                      glClientActiveTexture(GL_TEXTURE0 + textureUnitsActivated);
                    }

                    glTexCoordPointer(2, GL_FLOAT, vertexBuffer->vertexSize(), static_cast<s8*>(vertexData) + itSemantic->offset);
                    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

                    ++textureUnitsActivated;
                  }
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
          Matrix4f out = d_func()->m_viewMatrix.multiply(data.worldMatrix);
          glLoadMatrixf(d_func()->m_viewMatrix.data);
          glMultMatrixf(data.worldMatrix.data);

          //glLoadMatrixf(d_func()->m_viewMatrix.multiply(data.worldMatrix).data);

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
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);

            // disable point sprites
            if (EGEGraphics::RPT_POINTS == data.component->primitiveType())
            {
              if (Device::HasRenderCapability(EGEDevice::RENDER_CAPS_POINT_SPRITE))
              {
                glDisable(GL_POINT_SPRITE_ARB);
              }
            }
          }
          m_activeTextureUnits.clear();

          // clean up
          glDisableClientState(GL_VERTEX_ARRAY);
          glDisableClientState(GL_NORMAL_ARRAY);
          glDisableClientState(GL_COLOR_ARRAY);
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

  //EGE_LOG("---------------------- Render Flush end ----------------------");
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
      if (Device::HasRenderCapability(EGEDevice::RENDER_CAPS_MULTITEXTURE))
      {
        glActiveTexture(GL_TEXTURE0 + unit);
        m_activeTextureUnit = unit;

        OGL_CHECK();
      }
      else if (0 == unit)
      {
        // unit 0 is the only valid option if no multitexturing is present. Thus, no special activation is required
        m_activeTextureUnit = unit;
      }
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
  // NOTE: this implies that VBO mapping extension is present too
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
      Device::SetRenderCapability(EGEDevice::RENDER_CAPS_MAP_BUFFER, true);
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

  // texture compression
  glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC) wglGetProcAddress("glCompressedTexImage2D");

  // check for texture compressions support
  if (isExtensionSupported("GL_IMG_texture_compression_pvrtc"))
  {
    Device::SetRenderCapability(EGEDevice::RENDER_CAPS_TEXTURE_COMPRESSION_PVRTC, true);
  }

  if (isExtensionSupported("GL_EXT_texture_compression_s3tc"))
  {
    Device::SetRenderCapability(EGEDevice::RENDER_CAPS_TEXTURE_COMPRESSION_S3TC, true);
  }

  // Point sprite size array is not supported by default
  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_POINT_SPRITE_SIZE, false);
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
        // TAGE - why casting ?? bind not generic ?? why ??
        ((VertexBufferVBO*) buffer.object())->bind();

        // set vertex data base to 0 as for VBO we use offsets
        data = 0;
        break;

      default:

        EGE_ASSERT(false && "Invalid vertex buffer type");
        data = NULL;
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
      // TAGE - why casting ? unbind not generic ? why ?
      ((VertexBufferVBO*) buffer.object())->unbind();
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

  // process if any indicies exist
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
        // TAGE - why casting ? bind not generic ? why ?
        ((IndexBufferVBO*) buffer.object())->bind();

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
      // TAGE - why casting ? unbind not generic ? why ?
      ((IndexBufferVBO*) buffer.object())->unbind();
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

  if ( ! Device::HasRenderCapability(EGEDevice::RENDER_CAPS_VBO))
  {
    buffer = ege_new VertexBufferVBO(d_func()->app(), usage);
  }
  else
  {
    buffer = ege_new VertexBufferVA(d_func()->app(), usage);
  }

  return buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PIndexBuffer RenderSystemPrivate::createIndexBuffer(EGEIndexBuffer::UsageType usage) const
{
  PIndexBuffer buffer;

  if ( ! Device::HasRenderCapability(EGEDevice::RENDER_CAPS_VBO))
  {
    buffer = ege_new IndexBufferVBO(d_func()->app(), usage);
  }
  else
  {
    buffer = ege_new IndexBufferVA(d_func()->app(), usage);
  }

  return buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PTexture2D RenderSystemPrivate::createTexture2D(const String& name, const PImage& image)
{
  PTexture2D texture = ege_new Texture2D(d_func()->app(), name);
  if ((NULL == texture) || ! texture->isValid())
  {
    // error!
    return NULL;
  }

  activateTextureUnit(0);
  bindTexture(GL_TEXTURE_2D, texture->p_func()->id());

  // set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MapTextureFilter(d_func()->m_textureMinFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MapTextureFilter(d_func()->m_textureMagFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, MapTextureAddressingMode(d_func()->m_textureAddressingModeS));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, MapTextureAddressingMode(d_func()->m_textureAddressingModeT));

  // create it
  if (EGE_SUCCESS != texture->create(image))
  {
    // error!
    return NULL;
  }

  bindTexture(GL_TEXTURE_2D, 0);

  return texture;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PTexture2D RenderSystemPrivate::createTexture2D(const String& name, const PDataBuffer& data)
{
  PTexture2D texture = ege_new Texture2D(d_func()->app(), name);
  if ((NULL == texture) || ! texture->isValid())
  {
    // error!
    return NULL;
  }

  activateTextureUnit(0);
  bindTexture(GL_TEXTURE_2D, texture->p_func()->id());

  // set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MapTextureFilter(d_func()->m_textureMinFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MapTextureFilter(d_func()->m_textureMagFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, MapTextureAddressingMode(d_func()->m_textureAddressingModeS));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, MapTextureAddressingMode(d_func()->m_textureAddressingModeT));

  // create it
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

  // create empty texture of given size and format
  PTexture2D texture = ege_new Texture2D(d_func()->app(), name);
  if ((NULL == texture) || ! texture->isValid())
  {
    // error!
    return NULL;
  }

  activateTextureUnit(0);
  bindTexture(GL_TEXTURE_2D, texture->p_func()->id());

  // set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MapTextureFilter(d_func()->m_textureMinFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MapTextureFilter(d_func()->m_textureMagFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, MapTextureAddressingMode(d_func()->m_textureAddressingModeS));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, MapTextureAddressingMode(d_func()->m_textureAddressingModeT));

  // create texture from image
  if (EGE_SUCCESS != texture->p_func()->create(image))
  {
    // error!
    return NULL;
  }

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
