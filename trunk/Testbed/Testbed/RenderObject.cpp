#include "RenderObject.h"
#include "App.h"
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
    object->m_renderData = ege_new RenderComponent(app, name, EGEGraphics::RENDER_PRIORITY_MAIN);
    if (object->m_renderData)
    {
      // add render buffers
      if (!object->m_renderData->vertexBuffer()->addArray(EGEVertexBuffer::ARRAY_TYPE_POSITION_XYZ))
      {
        // error!
        EGE_DELETE(object);
        return NULL;
      }
    
      if (uv)
      {
        if (!object->m_renderData->vertexBuffer()->addArray(EGEVertexBuffer::ARRAY_TYPE_TEXTURE_UV))
        {
          // error!
          EGE_DELETE(object);
          return NULL;
        }
      }

      if (color)
      {
        if (!object->m_renderData->vertexBuffer()->addArray(EGEVertexBuffer::ARRAY_TYPE_COLOR_RGBA))
        {
          // error!
          EGE_DELETE(object);
          return NULL;
        }
      }
    }
  }

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance representing the triangle. */
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

    object->m_renderData->vertexBuffer()->unlock();
  }

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance representing the triangle. */
RenderObject* RenderObject::CreateRectangle(Application* app, const String& name, s32 width, s32 height, bool color, bool uv)
{
  RenderObject* object = CreateBase(app, name, color, uv);
  if (object)
  {
    float* data = (float*) object->m_renderData->vertexBuffer()->lock(0, 6);

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
    *data++ = - 0.5f * width;
    *data++ = - 0.5f * height;
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

    // 1
    *data++ = - 0.5f * width;
    *data++ = + 0.5f * height;
    *data++ = 0.0f;

    if (uv)
    {
      *data++ = 0.0f;
      *data++ = 1.0f;
    }

    if (color)
    {
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = 1.0f;
    }

    // 2
    *data++ = + 0.5f * width;
    *data++ = + 0.5f * height;
    *data++ = 0.0f;

    if (uv)
    {
      *data++ = 1.0f;
      *data++ = 1.0f;
    }

    if (color)
    {
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = 1.0f;
    }

    // 3
    *data++ = - 0.5f * width;
    *data++ = - 0.5f * height;
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

    // 4
    *data++ = + 0.5f * width;
    *data++ = + 0.5f * height;
    *data++ = 0.0f;

    if (uv)
    {
      *data++ = 1.0f;
      *data++ = 1.0f;
    }

    if (color)
    {
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = 1.0f;
    }
    
    // 5
    *data++ = + 0.5f * width;
    *data++ = - 0.5f * height;
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

    object->m_renderData->vertexBuffer()->unlock();
  }

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! SceneNodeObject override. Adds object render data for rendering with given renderer. */
bool RenderObject::addForRendering(EGE::Renderer* renderer)
{
  return renderer->addForRendering(parentNode()->worldMatrix(), m_renderData);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
