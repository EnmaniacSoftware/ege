#include "RenderObject.h"
#include "App.h"
#include <EGERenderComponent.h>
#include <EGEVertexBuffer.h>
#include <EGERenderer.h>
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderObject::RenderObject(Application* app, const EGE::String& name) : SceneNodeObject(name)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderObject::~RenderObject()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates base object. */
RenderObject* RenderObject::CreateBase(Application* app, const String& name, bool color, bool uv)
{
  RenderObject* object = ege_new RenderObject(app, name);
  if (object)
  {
    // setup vertex declaration
    VertexDeclaration vertexDeclaration;
    if ( ! vertexDeclaration.addElement(NVertexBuffer::VES_POSITION_XYZ))
    {
      // error!
      EGE_DELETE(object);
      return NULL;
    }

    if (uv && ! vertexDeclaration.addElement(NVertexBuffer::VES_TEXTURE_UV))
    {
      // error!
      EGE_DELETE(object);
      return NULL;
    }

    if (color && ! vertexDeclaration.addElement(NVertexBuffer::VES_COLOR_RGBA))
    {
      // error!
      EGE_DELETE(object);
      return NULL;
    }

    // create render data
    object->m_renderData = ege_new RenderComponent(app, name, vertexDeclaration, EGEGraphics::RP_MAIN, EGEGraphics::RPT_TRIANGLES, 
                                                   NVertexBuffer::UT_DYNAMIC_WRITE);
    if (NULL == object->m_renderData)
    {
      // error!
      EGE_DELETE(object);
      return NULL;
    }
  }

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderObject* RenderObject::CreateTriangle(Application* app, const String& name, s32 size, bool color, bool uv)
{
  RenderObject* object = CreateBase(app, name, color, uv);
  if (object)
  {
    float* data = (float*) object->m_renderData->vertexBuffer()->lock(0, 3);

    // top vertex
    *data++ = + 0.0f * size;
    *data++ = - 0.5f * size;
    *data++ = 0.0f;

    if (uv)
    {
      *data++ = 0.5f;
      *data++ = 1.0f;
    }

    if (color)
    {
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = 1.0f;
    }

    // bottom left vertex
    *data++ = - 0.5f * size;
    *data++ = + 0.5f * size;
    *data++ = 0.0f;

    if (uv)
    {
      *data++ = 0.0f;
      *data++ = 0.0f;
    }

    if (color)
    {
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = 1.0f;
    }

    // bottom right vertex
    *data++ = + 0.5f * size;
    *data++ = + 0.5f * size;
    *data++ = 0.0f;

    if (uv)
    {
      *data++ = 1.0f;
      *data++ = 0.0f;
    }

    if (color)
    {
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = 1.0f;
    }

    object->m_renderData->vertexBuffer()->unlock(data - 1);
  }

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderObject* RenderObject::CreateRectangle(Application* app, const String& name, s32 width, s32 height, bool color, bool uv, const EGE::Vector2i& segments)
{
  RenderObject* object = CreateBase(app, name, color, uv);
  if (object)
  {
    float* data = (float*) object->m_renderData->vertexBuffer()->lock(0, 6 * (segments.x + segments.y));
    
    Vector2f vertexPos(- 0.5f * width, - 0.5f * height);
    Vector2f vertexPosIncrement(width / static_cast<float32>(segments.x), height / static_cast<float32>(segments.y));

    Vector2f uvPos(0, 0);
    Vector2f uvPosIncrement(1.0f / segments.x, 1.0f / segments.y);

    for (s32 y = 0; y < segments.y; ++y)
    {
      // reset data in U direction
      vertexPos.x = - 0.5f * width;
      uvPos.x = 0.0f;

      for (s32 x = 0; x < segments.x; ++x)
      {
        // Glyph quad looks like follows:
        //
        //   (0,3)  (5)
        //    *------*
        //    |\     |
        //    | \Tri2|
        //    |  \   |
        //    |   \  |
        //    |    \ |
        //    |Tri1 \|
        //    |      |
        //    *------*
        //   (1)   (2,4)

        // 0
        *data++ = vertexPos.x;
        *data++ = vertexPos.y;
        *data++ = 0.0f;

        if (uv)
        {
          *data++ = uvPos.x;
          *data++ = uvPos.y;
        }

        if (color)
        {
          *data++ = 1.0f;
          *data++ = 1.0f;
          *data++ = 1.0f;
          *data++ = 1.0f;
        }

        // 1
        *data++ = vertexPos.x;
        *data++ = vertexPos.y + vertexPosIncrement.y;
        *data++ = 0.0f;

        if (uv)
        {
          *data++ = uvPos.x;
          *data++ = uvPos.y + uvPosIncrement.y;
        }

        if (color)
        {
          *data++ = 1.0f;
          *data++ = 1.0f;
          *data++ = 1.0f;
          *data++ = 1.0f;
        }

        // 2
        *data++ = vertexPos.x + vertexPosIncrement.x;
        *data++ = vertexPos.y + vertexPosIncrement.y;
        *data++ = 0.0f;

        if (uv)
        {
          *data++ = uvPos.x + uvPosIncrement.x;
          *data++ = uvPos.y + uvPosIncrement.y;
        }

        if (color)
        {
          *data++ = 1.0f;
          *data++ = 1.0f;
          *data++ = 1.0f;
          *data++ = 1.0f;
        }

        // 3
        *data++ = vertexPos.x;
        *data++ = vertexPos.y;
        *data++ = 0.0f;

        if (uv)
        {
          *data++ = uvPos.x;
          *data++ = uvPos.y;
        }

        if (color)
        {
          *data++ = 1.0f;
          *data++ = 1.0f;
          *data++ = 1.0f;
          *data++ = 1.0f;
        }

        // 4
        *data++ = vertexPos.x + vertexPosIncrement.x;
        *data++ = vertexPos.y + vertexPosIncrement.y;
        *data++ = 0.0f;

        if (uv)
        {
          *data++ = uvPos.x + uvPosIncrement.x;
          *data++ = uvPos.y + uvPosIncrement.y;
        }

        if (color)
        {
          *data++ = 1.0f;
          *data++ = 1.0f;
          *data++ = 1.0f;
          *data++ = 1.0f;
        }
    
        // 5
        *data++ = vertexPos.x + vertexPosIncrement.x;
        *data++ = vertexPos.y;
        *data++ = 0.0f;

        if (uv)
        {
          *data++ = uvPos.x + uvPosIncrement.x;
          *data++ = uvPos.y;
        }

        if (color)
        {
          *data++ = 1.0f;
          *data++ = 1.0f;
          *data++ = 1.0f;
          *data++ = 1.0f;
        }

        // update data in U direction
        vertexPos.x += vertexPosIncrement.x;
        uvPos.x += uvPosIncrement.x;
      }

      // update data in V direction
      vertexPos.y += vertexPosIncrement.y;
      uvPos.y += uvPosIncrement.y;
    }

    object->m_renderData->vertexBuffer()->unlock(data - 1);
  }

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! SceneNodeObject override. Adds object render data for rendering with given renderer. */
bool RenderObject::addForRendering(EGE::IRenderer* renderer, const Matrix4f& transform)
{
  return renderer->addForRendering(m_renderData, parentNode()->worldMatrix().multiply(transform));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
