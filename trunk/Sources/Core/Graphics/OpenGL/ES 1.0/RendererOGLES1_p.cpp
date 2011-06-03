#include "Core/Application/Application.h"
#include "GLES/gl.h"
#include "GLES/egl.h"
#include "Core/Graphics/OpenGL/ES 1.0/RendererOGLES1_p.h"
#include "Core/Components/Render/RenderComponent.h"
#include "Core/Graphics/Viewport.h"
#include "Core/Graphics/Camera.h"
#include "Core/Graphics/IndexBuffer.h"
#include "Core/Graphics/VertexBuffer.h"
#include "Core/Graphics/Material.h"
#include "Core/Graphics/OpenGL/Texture2DOGL.h"
#include "Core/Graphics/OpenGL/MaterialOGL_p.h"

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
    case EGEGraphics::RENDER_PRIMITIVE_TYPE_TRIANGLES: return GL_TRIANGLES;
    case EGEGraphics::RENDER_PRIMITIVE_TYPE_LINES:     return GL_LINES;
  }

  // default
  return GL_TRIANGLES;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RendererPrivate::RendererPrivate(Renderer* base) : m_d(base)
{
  detectExtensions();
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
    glClearColor(clearColor.m_red, clearColor.m_green, clearColor.m_blue, clearColor.m_alpha);

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
  glMatrixMode(GL_MODELVIEW);

  for (std::map<s32, Renderer::SRENDERDATA>::iterator iter = d_func()->m_renderData.begin(); iter != d_func()->m_renderData.end();)
  {
    Renderer::SRENDERDATA& renderData = iter->second;

    PVertexBuffer vertexBuffer = renderData.renderComponent->vertexBuffer();
    PIndexBuffer indexBuffer   = renderData.renderComponent->indexBuffer();
    PMaterial material         = renderData.renderComponent->material();

    // apply material
		applyMaterial(material);

    // check if there is anything to be rendered
    if (0 != vertexBuffer->vertexCount())
    {
      const std::vector<VertexBuffer::SBUFFERSEMANTIC>& vsSemantics = vertexBuffer->semantics();

      // TAGE - if indexed geometry count indicies
      u32 value = vertexBuffer->vertexCount();

      switch (renderData.renderComponent->primitiveType())
      {
        case EGEGraphics::RENDER_PRIMITIVE_TYPE_TRIANGLES: d_func()->m_triangleCount += value / 3; break;
        case EGEGraphics::RENDER_PRIMITIVE_TYPE_LINES: 
          
          // nothing 
          break;
      }
      
      d_func()->m_batchCount++;

      // lock vertex data
      void* vertexData = vertexBuffer->lock(0, vertexBuffer->vertexCount());

      // go thru all buffers
      for (std::vector<VertexBuffer::SBUFFERSEMANTIC>::const_iterator iterSemantics = vsSemantics.begin(); iterSemantics != vsSemantics.end(); 
           ++iterSemantics)
      {
        // set according to buffer type
        switch (iterSemantics->type)
        {
          case VertexBuffer::ARRAY_TYPE_POSITION_XYZ:

            glVertexPointer(3, GL_FLOAT, vertexBuffer->vertexSize(), static_cast<s8*>(vertexData) + iterSemantics->offset);
            glEnableClientState(GL_VERTEX_ARRAY);
            break;

          case VertexBuffer::ARRAY_TYPE_NORMAL:

            glNormalPointer(GL_FLOAT, vertexBuffer->vertexSize(), static_cast<s8*>(vertexData) + iterSemantics->offset);
            glEnableClientState(GL_NORMAL_ARRAY);
            break;

          case VertexBuffer::ARRAY_TYPE_COLOR_RGBA:

            glColorPointer(4, GL_FLOAT, vertexBuffer->vertexSize(), static_cast<s8*>(vertexData) + iterSemantics->offset);
            glEnableClientState(GL_COLOR_ARRAY);
            break;
      
          case VertexBuffer::ARRAY_TYPE_TEXTURE_UV:

            glClientActiveTexture(GL_TEXTURE0 + iterSemantics->index);
            glTexCoordPointer(2, GL_FLOAT, vertexBuffer->vertexSize(), static_cast<s8*>(vertexData) + iterSemantics->offset);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            break;

          //case VertexBuffer::ARRAY_TYPE_TANGENT:

          //  // TANGENT is binded to GL_TEXTURE6
          //  glClientActiveTexture( GL_TEXTURE6 );
          //  glTexCoordPointer( 3, GL_FLOAT, cRenderOp.getVertexBuffer()->getVertexSize(), 
          //                     static_cast<char*>( vertexData )+iter->uiOffset );
          //  glEnableClientState( GL_TEXTURE_COORD_ARRAY );
          //  break;
        }
      }

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
        glLoadMatrixf(d_func()->m_viewMatrix.multiply(renderData.worldMatrix).data);

        // render only if there is anything to render
        glDrawArrays(MapPrimitiveType(renderData.renderComponent->primitiveType()), 0, vertexBuffer->vertexCount());
      }

      // unlock vertex buffer
      vertexBuffer->unlock();

      // clean up
      glDisableClientState(GL_VERTEX_ARRAY);
      glDisableClientState(GL_NORMAL_ARRAY);
      glDisableClientState(GL_COLOR_ARRAY);
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
      glColor4f(1, 1, 1, 1);
    }

    // remove rendered component and go to next one
    d_func()->m_renderData.erase(iter++);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Detects and initializes extensions. */
void RendererPrivate::detectExtensions()
{
  const GLubyte* extensions = glGetString(GL_EXTENSIONS);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Applies material. */
void RendererPrivate::applyMaterial(const PMaterial& material)
{
  glDisable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_BLEND);

  if (material)
  {
    if (material->isBlendingEnabled())
    {
      glEnable(GL_BLEND);
      glBlendFunc(material->p_func()->srcBlendFunc(), material->p_func()->dstBlendFunc());
    }

    // go thru all textures
    for (u32 i = 0; i < material->textureCount(); ++i)
    {
      Object* texture = material->texture(i).object();

      // check if 2D texture
      if (EGE_OBJECT_UID_TEXTURE_2D == texture->uid())
      {
        Texture2D* tex2d = (Texture2D*) texture;

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tex2d->id());
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------