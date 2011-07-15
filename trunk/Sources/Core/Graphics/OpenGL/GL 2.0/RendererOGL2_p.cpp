#include "Core/Graphics/OpenGL/GL 2.0/RendererOGL2_p.h"
#include <gl/gl.h>
#include "glext.h"
#include "Core/Components/Render/RenderComponent.h"
#include "Core/Graphics/Viewport.h"
#include "Core/Graphics/Camera.h"
#include "Core/Graphics/IndexBuffer.h"
#include "Core/Graphics/VertexBuffer.h"
#include "Core/Graphics/Material.h"
#include "Core/Graphics/OpenGL/Texture2DOGL.h"
#include "Core/Graphics/Graphics.h"
#include "Core/Graphics/Render/RenderWindow.h"
#include "Core/Graphics/TextureImage.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(RendererPrivate)
EGE_DEFINE_DELETE_OPERATORS(RendererPrivate)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB = NULL;
PFNGLACTIVETEXTUREARBPROC glActiveTextureARB = NULL;

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
/*! Sends all geometry through the geometry pipeline to hardware. */
void RendererPrivate::flush()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glRotatef(d_func()->orientationRotation().degrees(), 0, 0, 1);
  glMultMatrixf(d_func()->m_projectionMatrix.data);

  // go thru all render data
  for (MultiMap<s32, Renderer::SRENDERDATA>::iterator iter = d_func()->m_renderData.begin(); iter != d_func()->m_renderData.end();)
  {
    Renderer::SRENDERDATA& renderData = iter->second;

    PVertexBuffer& vertexBuffer = renderData.renderComponent->vertexBuffer();
    PIndexBuffer& indexBuffer   = renderData.renderComponent->indexBuffer();
    PMaterial& material         = renderData.renderComponent->material();

    // apply material
    applyMaterial(material);

    // NOTE: change to modelview after material is applied as it may change current matrix mode
    glMatrixMode(GL_MODELVIEW);

    // determine texture count
    s32 textureCount = material ? material->textureCount() : 0;

    // check if there is anything to be rendered
    if (0 != vertexBuffer->vertexCount())
    {
      const DynamicArray<EGEVertexBuffer::SARRAYSEMANTIC>& vsSemantics = vertexBuffer->semantics();

      // TAGE - if indexed geometry count indicies
      u32 value = vertexBuffer->vertexCount();

      d_func()->m_vertexCount += value;
      d_func()->m_batchCount++;

      // lock vertex data
      void* vertexData = vertexBuffer->lock(0, vertexBuffer->vertexCount());

      // go thru all buffers
      for (DynamicArray<EGEVertexBuffer::SARRAYSEMANTIC>::const_iterator iterSemantics = vsSemantics.begin(); iterSemantics != vsSemantics.end(); 
           ++iterSemantics)
      {
        // set according to buffer type
        switch (iterSemantics->type)
        {
          case EGEVertexBuffer::ARRAY_TYPE_POSITION_XYZ:

            glVertexPointer(3, GL_FLOAT, vertexBuffer->vertexSize(), static_cast<s8*>(vertexData) + iterSemantics->offset);
            glEnableClientState(GL_VERTEX_ARRAY);
            break;

          case EGEVertexBuffer::ARRAY_TYPE_NORMAL:

            glNormalPointer(GL_FLOAT, vertexBuffer->vertexSize(), static_cast<s8*>(vertexData) + iterSemantics->offset);
            glEnableClientState(GL_NORMAL_ARRAY);
            break;

          case EGEVertexBuffer::ARRAY_TYPE_COLOR_RGBA:

            glColorPointer(4, GL_FLOAT, vertexBuffer->vertexSize(), static_cast<s8*>(vertexData) + iterSemantics->offset);
            glEnableClientState(GL_COLOR_ARRAY);
            break;
      
          case EGEVertexBuffer::ARRAY_TYPE_TEXTURE_UV:

            for (s32 i = 0; i < textureCount; ++i)
            {
              if (glClientActiveTextureARB)
              {
                glClientActiveTextureARB(GL_TEXTURE0_ARB + i);
              }

              glTexCoordPointer(2, GL_FLOAT, vertexBuffer->vertexSize(), static_cast<s8*>(vertexData) + iterSemantics->offset);
              glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            }
            // TAGE - we assume that each texture unit uses THE SAME texture coords!
            //for ( s32 i = 0; i < iTexturesEnabled; i++ )
            //{
              //glEnable(GL_TEXTURE_2D);
              //glBindTexture(GL_TEXTURE_2D, 2);
              //glClientActiveTexture(GL_TEXTURE0 + 0);
//              glTexCoordPointer(2, GL_FLOAT, vertexBuffer->vertexSize(), static_cast<s8*>(vertexData) + iterSemantics->offset);
  //          }
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
    iter = d_func()->m_renderData.erase(iter);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Detects and initializes extensions. */
void RendererPrivate::detectExtensions()
{
  const GLubyte* extensions = glGetString(GL_EXTENSIONS);

  glClientActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC) wglGetProcAddress("glClientActiveTextureARB");
  glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Applies material. */
void RendererPrivate::applyMaterial(const PMaterial& material)
{
  //for (s32 i = 0; i < 2; i++)
  //{
  //  if (glActiveTextureARB)
  //  {
  //    glActiveTextureARB(GL_TEXTURE0_ARB + i);
  //    glDisable(GL_TEXTURE_2D);
  //  }
  //}
  glDisable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_BLEND);

  if (material)
  {
    if ((EGEGraphics::BLEND_FACTOR_ONE != material->srcBlendFactor()) || (EGEGraphics::BLEND_FACTOR_ZERO != material->dstBlendFactor()))
    {
      glEnable(GL_BLEND);
      glBlendFunc(MapBlendFactor(material->srcBlendFactor()), MapBlendFactor(material->dstBlendFactor()));
    }

    // go thru all textures
    for (u32 i = 0; i < material->textureCount(); ++i)
    {
      Object* texture = material->texture(i).object();

      // check if 2D texture
      if (EGE_OBJECT_UID_TEXTURE_2D == texture->uid())
      {
        Texture2D* tex2d = (Texture2D*) texture;

        if (glActiveTextureARB)
        {
          glActiveTextureARB(GL_TEXTURE0_ARB + i);
        }

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tex2d->id());

        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
      }
      // check if texture image
      else if (EGE_OBJECT_UID_TEXTURE_IMAGE == texture->uid())
      {
        TextureImage* texImg = (TextureImage*) texture;
        Texture2D* tex2d = (Texture2D*) texImg->texture().object();

        if (glActiveTextureARB)
        {
          glActiveTextureARB(GL_TEXTURE0_ARB + i);
        }

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tex2d->id());

        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
        glScalef(texImg->rectangle().width, texImg->rectangle().height, 1.0f);
        glTranslatef(texImg->rectangle().x, texImg->rectangle().y, 0.0f);
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
