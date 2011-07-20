#include "Core/Graphics/OpenGL/ES 1.0/RendererOGLES1_p.h"
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
static GLenum MapPrimitiveType(EGEGraphics::ERenderPrimitiveType type)
{
  switch (type)
  {
    case EGEGraphics::RENDER_PRIMITIVE_TYPE_TRIANGLES:        return GL_TRIANGLES;
    case EGEGraphics::RENDER_PRIMITIVE_TYPE_TRIANGLE_STRIPS:  return GL_TRIANGLE_STRIP;
    case EGEGraphics::RENDER_PRIMITIVE_TYPE_TRIANGLE_FAN:     return GL_TRIANGLE_FAN;
    case EGEGraphics::RENDER_PRIMITIVE_TYPE_LINES:            return GL_LINES;
    case EGEGraphics::RENDER_PRIMITIVE_TYPE_LINE_LOOP:        return GL_LINE_LOOP;
  }

  // default
  return GL_TRIANGLES;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Maps blend factors to OpenGL compilant values. */
static GLenum MapBlendFactor(EGEGraphics::EBlendFactor factor)
{
  switch (factor)
  {
    case EGEGraphics::BLEND_FACTOR_ZERO:                return GL_ZERO;
    case EGEGraphics::BLEND_FACTOR_ONE:                 return GL_ONE;
    case EGEGraphics::BLEND_FACTOR_SRC_COLOR:           return GL_SRC_COLOR;
    case EGEGraphics::BLEND_FACTOR_DST_COLOR:           return GL_DST_COLOR;
    case EGEGraphics::BLEND_FACTOR_ONE_MINUS_SRC_COLOR: return GL_ONE_MINUS_SRC_COLOR;
    case EGEGraphics::BLEND_FACTOR_ONE_MINUS_DST_COLOR: return GL_ONE_MINUS_DST_COLOR;
    case EGEGraphics::BLEND_FACTOR_SRC_ALPHA:           return GL_SRC_ALPHA;
    case EGEGraphics::BLEND_FACTOR_DST_ALPHA:           return GL_DST_ALPHA;
    case EGEGraphics::BLEND_FACTOR_ONE_MINUS_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
    case EGEGraphics::BLEND_FACTOR_ONE_MINUS_DST_ALPHA: return GL_ONE_MINUS_DST_ALPHA;
  }

  // default
  return GL_ONE;
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
/*! Sends all geometry through the geometry pipeline to hardware. */
void RendererPrivate::flush()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glRotatef(d_func()->orientationRotation().degrees(), 0, 0, 1);
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
        const DynamicArray<EGEVertexBuffer::SARRAYSEMANTIC>& semantics = vertexBuffer->semantics();

        // TAGE - if indexed geometry count indicies
        u32 value = vertexBuffer->vertexCount();

        d_func()->m_vertexCount += value;
        d_func()->m_batchCount++;

        // lock vertex data
        void* vertexData = vertexBuffer->lock(0, vertexBuffer->vertexCount());

        // go thru all buffers
        for (DynamicArray<EGEVertexBuffer::SARRAYSEMANTIC>::const_iterator itSemantic = semantics.begin(); itSemantic != semantics.end(); ++itSemantic)
        {
          // set according to buffer type
          switch (itSemantic->type)
          {
            case EGEVertexBuffer::ARRAY_TYPE_POSITION_XYZ:

              glVertexPointer(3, GL_FLOAT, vertexBuffer->vertexSize(), static_cast<s8*>(vertexData) + itSemantic->offset);
              glEnableClientState(GL_VERTEX_ARRAY);
              break;

            case EGEVertexBuffer::ARRAY_TYPE_NORMAL:

              glNormalPointer(GL_FLOAT, vertexBuffer->vertexSize(), static_cast<s8*>(vertexData) + itSemantic->offset);
              glEnableClientState(GL_NORMAL_ARRAY);
              break;

            case EGEVertexBuffer::ARRAY_TYPE_COLOR_RGBA:

              glColorPointer(4, GL_FLOAT, vertexBuffer->vertexSize(), static_cast<s8*>(vertexData) + itSemantic->offset);
              glEnableClientState(GL_COLOR_ARRAY);
              break;
      
            case EGEVertexBuffer::ARRAY_TYPE_TEXTURE_UV:

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

        // check if INDICIES are to be used
        //if ( cRenderOp.isIndexBufferInUse() == true )
        //{
        //  // lock INDEX buffer
        //  void* pIndexData = cRenderOp.getIndexBuffer()->lock( 0, cRenderOp.getIndexBuffer()->getIndexCountInUse() );

        //  // render only if there is anything to render
        //  if ( cRenderOp.getIndexBuffer()->getIndexCountInUse() != 0 )
        //  {
        //    glDrawElements( cRenderOp.getType(), cRenderOp.getIndexBuffer()->getIndexCountInUse(), 
        //                    cRenderOp.getIndexBuffer()->getIndexSizeType(), pIndexData );
        //  }

        //  // unlock INDEX buffer
        //  cRenderOp.getIndexBuffer()->unlock();
        //}
        //else
        {
          // set model-view matrix
          glLoadMatrixf(d_func()->m_viewMatrix.multiply(data.worldMatrix).data);

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
        glColor4f(1, 1, 1, 1);
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
    if ((EGEGraphics::BLEND_FACTOR_ONE != material->srcBlendFactor()) || (EGEGraphics::BLEND_FACTOR_ZERO != material->dstBlendFactor()))
    {
      glEnable(GL_BLEND);
      glBlendFunc(MapBlendFactor(material->srcBlendFactor()), MapBlendFactor(material->dstBlendFactor()));
    }

    // go thru all textures
    for (u32 i = 0; i < material->textureCount(); ++i)
    {
      Object* texture = material->texture(i).object();

#if 0
      // check if 2D texture
      if (EGE_OBJECT_UID_TEXTURE_2D == texture->uid())
      {
        Texture2D* tex2d = (Texture2D*) texture;

        activateTextureUnit(i);
        bindTexture(GL_TEXTURE_2D, tex2d->p_func()->id());

        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
      }
      // check if texture image
      else 
#endif
      if (EGE_OBJECT_UID_TEXTURE_IMAGE == texture->uid())
      {
        TextureImage* texImg = (TextureImage*) texture;
        Texture2D* tex2d = (Texture2D*) texImg->texture().object();

        activateTextureUnit(i);
        bindTexture(GL_TEXTURE_2D, tex2d->p_func()->id());

        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
        glScalef(texImg->rectangle().width, texImg->rectangle().height, 1.0f);
        glTranslatef(texImg->rectangle().x, texImg->rectangle().y, 0.0f);
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Activates given texture unit. */
bool RendererPrivate::activateTextureUnit(u32 unit)
{
  // check if unit available
  if (unit < Device::GetTextureUnitsCount())
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
  if (boundTextureId != textureId)
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
  // there is at least 1 texture unit available
  Device::SetTextureUnitsCount(1);

  // check if multitexturing is supported
  if (isExtensionSupported("GL_ARB_multitexture"))
  {
    // get number of texture units
		GLint units;
		glGetIntegerv(GL_MAX_TEXTURE_UNITS, &units);
    Device::SetTextureUnitsCount(static_cast<u32>(units));
  }

  // detect maximal texture size
  GLint size;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);
  Device::SetTextureMaxSize(static_cast<u32>(size));
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
