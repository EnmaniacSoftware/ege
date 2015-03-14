#include "Core/Graphics/RenderObjectFactory.h"
#include "EGEDataBuffer.h"
#include "EGERenderComponent.h"
#include "EGESpline.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PRenderComponent RenderObjectFactory::CreateQuadXY(Engine& engine, const String& name, Vector4f position, Vector2f size, Alignment origin, bool flipU, 
                                                   bool flipV, VertexDeclarationSymbol vertexDeclaration, s32 priority, 
                                                   EGEGraphics::RenderPrimitiveType primitive, NVertexBuffer::UsageType vertexUsage)
{
  PRenderComponent component;

  // convert vertex semantics
  VertexDeclaration declaration = ConvertVertexDeclarationSymbol(vertexDeclaration);
  if (0 != declaration.vertexSize())
  {
    // create quad
    component = CreateQuadXY(engine, name, position, size, origin, flipU, flipV, declaration, priority, primitive, vertexUsage);
  }

  return component;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PRenderComponent RenderObjectFactory::CreateQuadXY(Engine& engine, const String& name, Vector4f position, Vector2f size, Alignment origin, bool flipU, 
                                                   bool flipV, const VertexDeclaration& vertexDeclaration, s32 priority, 
                                                   EGEGraphics::RenderPrimitiveType primitive, NVertexBuffer::UsageType vertexUsage)

{
  PRenderComponent object = ege_new RenderComponent(engine, name, vertexDeclaration, priority, primitive, vertexUsage);
  if (NULL != object)
  {
    // setup vertex buffer semantics
    if ( ! object->isValid())
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
PRenderComponent RenderObjectFactory::CreateQuatroQuadXY(Engine& engine, const String& name, Vector4f position, Vector2f size, Alignment origin, bool flipU, 
                                                         bool flipV, VertexDeclarationSymbol vertexDeclaration, s32 priority, 
                                                         EGEGraphics::RenderPrimitiveType primitive, NVertexBuffer::UsageType vertexUsage)
{
  PRenderComponent component;

  // convert vertex semantics
  VertexDeclaration declaration = ConvertVertexDeclarationSymbol(vertexDeclaration);
  if (0 != declaration.vertexSize())
  {
    // create object
    component = CreateQuatroQuadXY(engine, name, position, size, origin, flipU, flipV, declaration, priority, primitive, vertexUsage);
  }

  return component;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PRenderComponent RenderObjectFactory::CreateQuatroQuadXY(Engine& engine, const String& name, Vector4f position, Vector2f size, Alignment origin, bool flipU, 
                                                         bool flipV, const VertexDeclaration& vertexDeclaration, s32 priority, 
                                                         EGEGraphics::RenderPrimitiveType primitive, NVertexBuffer::UsageType vertexUsage)

{
  PRenderComponent object = ege_new RenderComponent(engine, name, vertexDeclaration, priority, primitive, vertexUsage);
  if (NULL != object)
  {
    // setup vertex buffer semantics
    if ( ! object->isValid())
    {
      // error!
      return NULL;
    }

    if ( ! DoCreateQuatroQuadXY(object, position, size, origin, primitive, flipU, flipV))
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

      vertexList.append(1);
      vertexList.append(2);
      vertexList.append(0);
      vertexList.append(3);
      break;

    case EGEGraphics::RPT_TRIANGLES:

      vertexList.append(0);
      vertexList.append(1);
      vertexList.append(2);
      vertexList.append(0);
      vertexList.append(2);
      vertexList.append(3);
      break;

    default:

      EGE_ASSERT_X(false, "Not supported!");
      return NULL;
  }

  // resize vertex buffer
  if ( ! component->vertexBuffer()->setSize(static_cast<u32>(vertexList.length())))
  {
    // error!
    return false;
  }

  // lock vertex buffer
  float32* data = reinterpret_cast<float32*>(component->vertexBuffer()->lock(0, static_cast<u32>(vertexList.length())));
  if (NULL == data)
  {
    // error!
    return false;
  }

  // go thru all vertices
  for (IntArray::ConstIterator it = vertexList.begin(); it != vertexList.end(); ++it)
  {
    const VertexData& vertex = vertices[*it];

    // go thru all vertex arrays one by one
    const VertexElementArray& vertexElements = component->vertexBuffer()->vertexDeclaration().vertexElements();
    for (VertexElementArray::ConstIterator it = vertexElements.begin(); it != vertexElements.end(); ++it)
    {
      switch (it->semantic())
      {
        case NVertexBuffer::VES_POSITION_XYZ:

          *data++ = vertex.pos.x;
          *data++ = vertex.pos.y;
          *data++ = vertex.pos.z;
          break;

        case NVertexBuffer::VES_POSITION_XY:

          *data++ = vertex.pos.x;
          *data++ = vertex.pos.y;
          break;

        case NVertexBuffer::VES_TEXTURE_UV:

          *data++ = vertex.uv.x;
          *data++ = vertex.uv.y;
          break;

        case NVertexBuffer::VES_COLOR_RGBA:

          *data++ = 1.0f;
          *data++ = 1.0f;
          *data++ = 1.0f;
          *data++ = 1.0f;
          break;

        default:

          EGE_ASSERT_X(false, "Not supported!");
          return NULL;
      }
    }
  }

  // unlock vertex buffer
  component->vertexBuffer()->unlock(data - 1);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool RenderObjectFactory::DoCreateQuatroQuadXY(PRenderComponent& component, Vector4f position, Vector2f size, Alignment origin, 
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
  //      (0)    (3)    (5)
  //       *------*------*
  //       |\     |\     |
  //       | \Tri2| \Tri4|
  //       |  \   |  \   |
  //       |   \  |   \  |
  //       |    \ |    \ |
  //       |Tri1 \|Tri3 \|
  //       *------*------*
  //      (1)    (2)    (4)
  //       |\     |\     |
  //       | \Tri6| \Tri8|
  //       |  \   |  \   |
  //       |   \  |   \  |
  //       |    \ |    \ |
  //       |Tri5 \|Tri7 \|
  //       *------*------*
  //      (6)    (7)    (8) 

  struct VertexData
  {
    Vector3f pos;
    Vector2f uv;
  };
  VertexData vertices[9];

  vertices[0].pos = Vector3f(position.x, position.y, position.z);
  vertices[0].uv  = Vector2f(flipU ? 1.0f : 0.0f, flipV ? 1.0f : 0.0f);
  vertices[1].pos = Vector3f(position.x, position.y + size.y * 0.5f, position.z);
  vertices[1].uv  = Vector2f(flipU ? 1.0f : 0.0f, flipV ? 0.0f : 1.0f);
  vertices[2].pos = Vector3f(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
  vertices[2].uv  = Vector2f(flipU ? 0.0f : 1.0f, flipV ? 0.0f : 1.0f);
  vertices[3].pos = Vector3f(position.x + size.x * 0.5f, position.y, position.z);
  vertices[3].uv  = Vector2f(flipU ? 0.0f : 1.0f, flipV ? 1.0f : 0.0f);
  vertices[4].pos = Vector3f(position.x + size.x, position.y + size.y * 0.5f, position.z);
  vertices[4].uv  = vertices[1].uv;
  vertices[5].pos = Vector3f(position.x + size.x, position.y, position.z);
  vertices[5].uv  = vertices[0].uv;
  vertices[6].pos = Vector3f(position.x, position.y + size.y, position.z);
  vertices[6].uv  = vertices[0].uv;
  vertices[7].pos = Vector3f(position.x + size.x * 0.5f, position.y + size.y, position.z);
  vertices[7].uv  = vertices[3].uv;
  vertices[8].pos = Vector3f(position.x + size.x, position.y + size.y, position.z);
  vertices[8].uv  = vertices[0].uv;

  // determine list of vertices
  IntArray vertexList;
  switch (primitive)
  {
    case EGEGraphics::RPT_TRIANGLE_STRIPS:

      vertexList.append(1);
      vertexList.append(2);
      vertexList.append(0);
      vertexList.append(3);
      vertexList.append(3);  // degenerated
      vertexList.append(2);  // degenerated
      vertexList.append(2);
      vertexList.append(4);
      vertexList.append(3);
      vertexList.append(5);
      vertexList.append(5);  // degenerated
      vertexList.append(6);  // degenerated
      vertexList.append(6);
      vertexList.append(7);
      vertexList.append(1);
      vertexList.append(2);
      vertexList.append(2);  // degenerated
      vertexList.append(7);  // degenerated
      vertexList.append(7);
      vertexList.append(8);
      vertexList.append(2);
      vertexList.append(4);
      break;

    case EGEGraphics::RPT_TRIANGLES:

      vertexList.append(0);
      vertexList.append(1);
      vertexList.append(2);
      vertexList.append(0);
      vertexList.append(2);
      vertexList.append(3);
      vertexList.append(3);
      vertexList.append(2);
      vertexList.append(4);
      vertexList.append(3);
      vertexList.append(4);
      vertexList.append(5);
      vertexList.append(1);
      vertexList.append(6);
      vertexList.append(7);
      vertexList.append(1);
      vertexList.append(7);
      vertexList.append(2);
      vertexList.append(2);
      vertexList.append(7);
      vertexList.append(8);
      vertexList.append(2);
      vertexList.append(8);
      vertexList.append(4);
      break;

    default:

      EGE_ASSERT_X(false, "Not supported!");
      return NULL;
  }

  // resize vertex buffer
  if ( ! component->vertexBuffer()->setSize(static_cast<u32>(vertexList.length())))
  {
    // error!
    return false;
  }

  // lock vertex buffer
  float32* data = reinterpret_cast<float32*>(component->vertexBuffer()->lock(0, static_cast<u32>(vertexList.length())));
  if (NULL == data)
  {
    // error!
    return false;
  }

  // go thru all vertices
  for (IntArray::ConstIterator it = vertexList.begin(); it != vertexList.end(); ++it)
  {
    const VertexData& vertex = vertices[*it];

    // go thru all vertex arrays one by one
    const VertexElementArray& vertexElements = component->vertexBuffer()->vertexDeclaration().vertexElements();
    for (VertexElementArray::ConstIterator it = vertexElements.begin(); it != vertexElements.end(); ++it)
    {
      switch (it->semantic())
      {
        case NVertexBuffer::VES_POSITION_XYZ:

          *data++ = vertex.pos.x;
          *data++ = vertex.pos.y;
          *data++ = vertex.pos.z;
          break;

        case NVertexBuffer::VES_POSITION_XY:

          *data++ = vertex.pos.x;
          *data++ = vertex.pos.y;
          break;

        case NVertexBuffer::VES_TEXTURE_UV:

          *data++ = vertex.uv.x;
          *data++ = vertex.uv.y;
          break;

        case NVertexBuffer::VES_COLOR_RGBA:

          *data++ = 1.0f;
          *data++ = 1.0f;
          *data++ = 1.0f;
          *data++ = 1.0f;
          break;

        default:

          EGE_ASSERT_X(false, "Not supported!");
          return NULL;
      }
    }
  }

  // unlock vertex buffer
  component->vertexBuffer()->unlock(data - 1);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PRenderComponent RenderObjectFactory::Create(const CubicSpline* spline, Engine& engine, const String& name, Vector3f offset,
                                             VertexDeclarationSymbol vertexDeclaration, s32 priority)
{
  PRenderComponent component;

  // convert vertex semantics
  VertexDeclaration declaration = ConvertVertexDeclarationSymbol(vertexDeclaration);
  if (0 != declaration.vertexSize())
  {
    // create quad
    component = Create(spline, engine, name, offset, declaration, priority);
  }

  return component;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PRenderComponent RenderObjectFactory::Create(const CubicSpline* spline, Engine& engine, const String& name, Vector3f offset, 
                                             const VertexDeclaration& vertexDeclaration, s32 priority)
{
  s32 vertexCount = 25;

  PRenderComponent component = ege_new RenderComponent(engine, name, vertexDeclaration, priority, EGEGraphics::RPT_LINES);
  if ((NULL != component) && component->isValid())
  {
    if (component->vertexBuffer()->setSize(vertexCount * 2))
    {
      float32* data = reinterpret_cast<float32*>(component->vertexBuffer()->lock(0, vertexCount * 2));
      if (NULL == data)
      {
        // error!
        return NULL;
      }

      Vector3f pos;

	    for (s32 i = 0; i < vertexCount; ++i)
	    {
        pos = spline->value(i / (1.0f * vertexCount));
        pos += offset;

        *data++ = pos.x;
        *data++ = pos.y;
        *data++ = pos.z;
        *data++ = 1.0f;
        *data++ = 1.0f;
        *data++ = 1.0f;
        *data++ = 1.0f;

        pos = spline->value((i + 1) / (1.0f * vertexCount));
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
VertexDeclaration RenderObjectFactory::ConvertVertexDeclarationSymbol(VertexDeclarationSymbol vertexDeclaration)
{
  VertexDeclaration declaration;

  bool result = true;

  // process according to type
  switch (vertexDeclaration)
  {
    case VS_V3:

      result |= declaration.addElement(NVertexBuffer::VES_POSITION_XYZ);
      break;

    case VS_V3_T2:

      result |= declaration.addElement(NVertexBuffer::VES_POSITION_XYZ);
      result |= declaration.addElement(NVertexBuffer::VES_TEXTURE_UV);
      break;

    case VS_V3_T2_C4:

      result |= declaration.addElement(NVertexBuffer::VES_POSITION_XYZ);
      result |= declaration.addElement(NVertexBuffer::VES_TEXTURE_UV);
      result |= declaration.addElement(NVertexBuffer::VES_COLOR_RGBA);
      break;

    case VS_V3_C4:

      result |= declaration.addElement(NVertexBuffer::VES_POSITION_XYZ);
      result |= declaration.addElement(NVertexBuffer::VES_COLOR_RGBA);
      break;

    case VS_V2:

      result |= declaration.addElement(NVertexBuffer::VES_POSITION_XY);
      break;

    case VS_V2_T2:

      result |= declaration.addElement(NVertexBuffer::VES_POSITION_XY);
      result |= declaration.addElement(NVertexBuffer::VES_TEXTURE_UV);
      break;

    case VS_V2_T2_C4:

      result |= declaration.addElement(NVertexBuffer::VES_POSITION_XY);
      result |= declaration.addElement(NVertexBuffer::VES_TEXTURE_UV);
      result |= declaration.addElement(NVertexBuffer::VES_COLOR_RGBA);
      break;

    case VS_V2_C4:

      result |= declaration.addElement(NVertexBuffer::VES_POSITION_XY);
      result |= declaration.addElement(NVertexBuffer::VES_COLOR_RGBA);
      break;

    default:

      EGE_ASSERT_X(false, "Implement?");
      break;
  }

  // check if error
  if ( ! result)
  {
    declaration.clear();
  }

  return declaration;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END