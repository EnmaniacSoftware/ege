#include "Core/Graphics/RenderObjectFactory.h"
#include "Core/Data/DataBuffer.h"
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates render component with 2D quad on XY plane. 
 * @param app       Pointer to application.
 * @param name      Name of render component.
 * @param position  Quad position (local-space).
 * @param size      Size of quad.
 * @param origin    Quad's origin alignment.
 * @param semantics Render component's vertex buffer semantics.
 * @param priority  Render priority.
 * @param primitive Render primitive type.
 * @return Returns render component. NULL if failed.
 */
PRenderComponent RenderObjectFactory::CreateQuadXY(Application* app, const String& name, Vector4f position, Vector2f size, Alignment origin, 
                                                   EGEVertexBuffer::SemanticType semantics, s32 priority, EGEGraphics::RenderPrimitiveType primitive)
{
  PRenderComponent object = ege_new RenderComponent(app, name, priority, primitive);
  if (object)
  {
    // setup vertex buffer semantics
    if (!object->isValid() || !object->vertexBuffer()->setSemantics(semantics))
    {
      // error!
      return NULL;
    }

    // apply alignment
    // NOTE: by default it is TOP-LEFT
    if (origin & EGEAlignment::ALIGN_RIGHT)
    {
      position.x -= size.x;
    }
    else if (origin & EGEAlignment::ALIGN_HCENTER)
    {
      position.x -= size.x / 2.0f;
    }

    if (origin & EGEAlignment::ALIGN_BOTTOM)
    {
      position.y -= size.y;
    }
    else if (origin & EGEAlignment::ALIGN_VCENTER)
    {
      position.y -= size.y / 2.0f;
    }

    // List of all quad vertices
    // NOTE: vertices are in the following order:
    //   (0)    (3)
    //    *------*
    //    |\     |
    //    | \Tri2|
    //    |  \   |
    //    |   \  |
    //    |    \ |
    //    |Tri1 \|
    //    |      |
    //    *------*
    //   (1)    (2)
    struct VertexData
    {
      Vector3f pos;
      Vector2f uv;
    };
    VertexData vertices[4];
    vertices[0].pos = Vector3f(position.x, position.y, position.z);
    vertices[0].uv  = Vector2f(0, 0);
    vertices[1].pos = Vector3f(position.x, position.y + size.y, position.z);
    vertices[1].uv  = Vector2f(0, 1);
    vertices[2].pos = Vector3f(position.x + size.x, position.y + size.y, position.z);
    vertices[2].uv  = Vector2f(1, 1);
    vertices[3].pos = Vector3f(position.x + size.x, position.y, position.z);
    vertices[3].uv  = Vector2f(1, 0);

    // determine list of vertices
    IntArray vertexList;
    switch (primitive)
    {
      case EGEGraphics::RPT_TRIANGLE_STRIPS:

        vertexList.push_back(1);
        vertexList.push_back(2);
        vertexList.push_back(0);
        vertexList.push_back(3);
        break;

      case EGEGraphics::RPT_TRIANGLES:

        vertexList.push_back(0);
        vertexList.push_back(1);
        vertexList.push_back(2);
        vertexList.push_back(0);
        vertexList.push_back(2);
        vertexList.push_back(3);
        break;

      default:

        EGE_ASSERT(false && "Not supported!");
        return NULL;
    }

    // lock vertex buffer
    float32* data = (float32*) object->vertexBuffer()->lock(0, static_cast<u32>(vertexList.size()));
    if (NULL == data)
    {
      // error!
      return NULL;
    }

    // go thru all vertices
    for (IntArray::const_iterator it = vertexList.begin(); it != vertexList.end(); ++it)
    {
      const VertexData& vertex = vertices[*it];

      // go thru all vertex arrays one by one
      const VertexBuffer::SemanticsList& semantics = object->vertexBuffer()->semantics();
      for (VertexBuffer::SemanticsList::const_iterator itSem = semantics.begin(); itSem != semantics.end(); ++itSem)
      {
        switch ((*itSem).type)
        {
          case EGEVertexBuffer::AT_POSITION_XYZ:

            *data++ = vertex.pos.x;
            *data++ = vertex.pos.y;
            *data++ = vertex.pos.z;
            break;

          case EGEVertexBuffer::AT_TEXTURE_UV:

            *data++ = vertex.uv.x;
            *data++ = vertex.uv.y;
            break;

          case EGEVertexBuffer::AT_COLOR_RGBA:

            *data++ = 1.0f;
            *data++ = 1.0f;
            *data++ = 1.0f;
            *data++ = 1.0f;
            break;

          default:

            EGE_ASSERT(false && "Not supported!");
            return NULL;
        }
      }
    }

    // unlock vertex buffer
    object->vertexBuffer()->unlock();
  }

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
