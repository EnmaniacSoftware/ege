#include "Core/Graphics/RenderObjectFactory.h"
#include "Core/Data/DataBuffer.h"
#include <EGERenderComponent.h>
#include <EGESpline.h>
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PRenderComponent RenderObjectFactory::CreateQuadXY(Application* app, const String& name, Vector4f position, Vector2f size, Alignment origin, bool flipU,
                                                   bool flipV, EGEVertexBuffer::SemanticType semantics, s32 priority, 
                                                   EGEGraphics::RenderPrimitiveType primitive, EGEVertexBuffer::UsageType vertexUsage)
{
  PRenderComponent object = ege_new RenderComponent(app, name, priority, primitive, vertexUsage);
  if (NULL != object)
  {
    // setup vertex buffer semantics
    if (!object->isValid() || !object->vertexBuffer()->setSemantics(semantics))
    {
      // error!
      return NULL;
    }

    if ( ! DoCreateQuadXY(object, position, size, origin, primitive, flipU, flipV))
    {
      // error!
      return NULL;
    }
  }

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PRenderComponent RenderObjectFactory::CreateQuadXY(Application* app, const String& name, Vector4f position, Vector2f size, Alignment origin, bool flipU, 
                                                   bool flipV, const List<EGEVertexBuffer::ArrayType>& semantics, s32 priority, 
                                                   EGEGraphics::RenderPrimitiveType primitive, EGEVertexBuffer::UsageType vertexUsage)

{
  PRenderComponent object = ege_new RenderComponent(app, name, priority, primitive, vertexUsage);
  if (NULL != object)
  {
    // setup vertex buffer semantics
    if (!object->isValid() || !object->vertexBuffer()->setSemantics(semantics))
    {
      // error!
      return NULL;
    }

    if ( ! DoCreateQuadXY(object, position, size, origin, primitive, flipU, flipV))
    {
      // error!
      return NULL;
    }
  }

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool RenderObjectFactory::DoCreateQuadXY(PRenderComponent& component, Vector4f position, Vector2f size, Alignment origin, 
                                         EGEGraphics::RenderPrimitiveType primitive, bool flipU, bool flipV)
{
  // apply alignment
  // NOTE: by default it is TOP-LEFT
  if (origin & ALIGN_RIGHT)
  {
    position.x -= size.x;
  }
  else if (origin & ALIGN_HCENTER)
  {
    position.x -= size.x / 2.0f;
  }

  if (origin & ALIGN_BOTTOM)
  {
    position.y -= size.y;
  }
  else if (origin & ALIGN_VCENTER)
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
  vertices[0].uv  = Vector2f(flipU ? 1.0f : 0.0f, flipV ? 1.0f : 0.0f);
  vertices[1].pos = Vector3f(position.x, position.y + size.y, position.z);
  vertices[1].uv  = Vector2f(flipU ? 1.0f : 0.0f, flipV ? 0.0f : 1.0f);
  vertices[2].pos = Vector3f(position.x + size.x, position.y + size.y, position.z);
  vertices[2].uv  = Vector2f(flipU ? 0.0f : 1.0f, flipV ? 0.0f : 1.0f);
  vertices[3].pos = Vector3f(position.x + size.x, position.y, position.z);
  vertices[3].uv  = Vector2f(flipU ? 0.0f : 1.0f, flipV ? 1.0f : 0.0f);

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
  float32* data = (float32*) component->vertexBuffer()->lock(0, static_cast<u32>(vertexList.size()));
  if (NULL == data)
  {
    // error!
    return false;
  }

  // go thru all vertices
  for (IntArray::const_iterator it = vertexList.begin(); it != vertexList.end(); ++it)
  {
    const VertexData& vertex = vertices[*it];

    // go thru all vertex arrays one by one
    const EGEVertexBuffer::SemanticArray& semantics = component->vertexBuffer()->semantics();
    for (EGEVertexBuffer::SemanticArray::const_iterator itSem = semantics.begin(); itSem != semantics.end(); ++itSem)
    {
      switch ((*itSem).type)
      {
        case EGEVertexBuffer::AT_POSITION_XYZ:

          *data++ = vertex.pos.x;
          *data++ = vertex.pos.y;
          *data++ = vertex.pos.z;
          break;

        case EGEVertexBuffer::AT_POSITION_XY:

          *data++ = vertex.pos.x;
          *data++ = vertex.pos.y;
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
  component->vertexBuffer()->unlock(data - 1);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PRenderComponent RenderObjectFactory::Create(const CubicSpline* spline, Application* app, const String& name, Vector4f offset, 
                                             EGEVertexBuffer::SemanticType semantics, s32 priority)
{
  s32 vertexCount = 25;

  RenderComponent* component = ege_new RenderComponent(app, name, priority, EGEGraphics::RPT_LINES);
  if (component && component->isValid())
  {
    if (component->vertexBuffer()->setSemantics(semantics))
    {
      float32* data = (float32*) component->vertexBuffer()->lock(0, vertexCount * 2);

      Vector4f pos;

	    for (s32 i = 0; i < vertexCount; ++i)
	    {
        spline->value(pos, i / (1.0f * vertexCount));
        pos += offset;

        *data++ = pos.x;
        *data++ = pos.y;
        *data++ = pos.z;
        *data++ = 1.0f;
        *data++ = 1.0f;
        *data++ = 1.0f;
        *data++ = 1.0f;

        spline->value(pos, (i + 1) / (1.0f * vertexCount));
        pos += offset;

        *data++ = pos.x;
        *data++ = pos.y;
        *data++ = pos.z;
        *data++ = 1.0f;
        *data++ = 1.0f;
        *data++ = 1.0f;
        *data++ = 1.0f;
      }

      // tangents
      //spline->value(pos, 0);
      //*data++ = pos.x;
      //*data++ = pos.y;
      //*data++ = pos.z;
      //*data++ = 1.0f;
      //*data++ = 0.0f;
      //*data++ = 0.0f;
      //*data++ = 1.0f;

      //pos = spline->segment(0)->beginTangent();

      //*data++ = pos.x;
      //*data++ = pos.y;
      //*data++ = pos.z;
      //*data++ = 1.0f;
      //*data++ = 0.0f;
      //*data++ = 0.0f;
      //*data++ = 1.0f;

      //spline->value(pos, 1);
      //*data++ = pos.x;
      //*data++ = pos.y;
      //*data++ = pos.z;
      //*data++ = 1.0f;
      //*data++ = 0.0f;
      //*data++ = 0.0f;
      //*data++ = 1.0f;

      //pos = spline->segment((2);

      //*data++ = pos.x;
      //*data++ = pos.y;
      //*data++ = pos.z;
      //*data++ = 1.0f;
      //*data++ = 0.0f;
      //*data++ = 0.0f;
      //*data++ = 1.0f;

      component->vertexBuffer()->unlock(data - 1);
    }
  }

  return component;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END