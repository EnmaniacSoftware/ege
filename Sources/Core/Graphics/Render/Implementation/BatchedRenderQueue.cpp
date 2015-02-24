#include "Core/Graphics/Render/Implementation/BatchedRenderQueue.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(BatchedRenderQueue)
EGE_DEFINE_DELETE_OPERATORS(BatchedRenderQueue)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BatchedRenderQueue::BatchedRenderQueue(u32 priority, EGEGraphics::RenderPrimitiveType primitiveType) 
: RenderQueue(EGE_OBJECT_UID_BACTHED_RENDER_QUEUE, priority, primitiveType)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BatchedRenderQueue::~BatchedRenderQueue()
{
  m_renderData = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool BatchedRenderQueue::IsSuitable(const PRenderComponent& component)
{
  bool result = true;

  // check if more than one pass
  // TAGE - need to think about it, perhaps it will be possible to cache these as well
  if (1 < component->material()->passCount())
  {
    result = false;
  }

  // only components with the same amount of texture as UV components can be batched
  // NOTE: this is will be major thing to refactor for OGLES 2.0 due to texture matrices
  const u32 textureUVElements = component->vertexBuffer()->vertexDeclaration().elementCount(NVertexBuffer::VES_TEXTURE_UV);
  for (u32 i = 0; i < component->material()->passCount() && result; ++i)
  {
    const PRenderPass& pass = component->material()->pass(i);

    // check if number of texture UV elements DO NOT match number of texture units
    if (textureUVElements != pass->textureCount())
    {
      // batching not possible
      result = false;
    }

    // check if same textures
    // NOTE: compare all texture in current pass to first texture in first pass
    for (u32 textureIndex = 0; textureIndex < pass->textureCount() && result; ++textureIndex)
    {
      // check if same texture
      // TAGE - check if possible to compare just texture objects instead of names
      if (component->material()->pass(0)->texture(0)->name() != pass->texture(textureIndex)->name())
      {
        // batching not possible
        result = false;
      }
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult BatchedRenderQueue::addForRendering(const PRenderComponent& component, const Matrix4f& modelMatrix)
{
  EGEResult result = EGE_SUCCESS;

  // NOTE: add support for point sizes and line widths if necessary
  EGE_ASSERT((1.0f == component->pointSize()) && (1.0f == component->lineWidth()));

  // check if priorities does not match or not suitable
  if ((component->priority() != priority()) || ! IsSuitable(component))
  {
    // reject
    return EGE_ERROR_NOT_SUPPORTED;
  }

  // check if not allocated yet
  if (NULL == m_renderData)
  {
    // allocate
    if ( ! allocateMasterRenderComponent(component))
    {
      // failed
      result = EGE_ERROR;
    }
  }

  // check if no material assigned yet
  // NOTE: if thats the case, first component being added defined the material
  if (NULL == m_renderData->material())
  {
    prepare(component);
  }

  // verify if component is compatible
  // NOTE: 1. Vertex semantics must be the same.
  //       2. Index semantics must be the same.
  //       3. Clip region lies within master one (if any).
  //       4. Materials must be the same:
  //          - textures
  //          - shaders
  //          - blend operators
  //       5. Seperate texture for each UV semantic type
  if (component->vertexBuffer()->vertexDeclaration() != m_renderData->vertexBuffer()->vertexDeclaration())
  {
    // reject
    result = EGE_ERROR_NOT_SUPPORTED;
  }
  else if (component->indexBuffer()->indexSize() != m_renderData->indexBuffer()->indexSize())
  {
    // reject
    result = EGE_ERROR_NOT_SUPPORTED;
  }
  else if ( ! m_renderData->clipRect().isNull() && ! m_renderData->clipRect().contains(component->clipRect()))
  {
    // reject
    result = EGE_ERROR_NOT_SUPPORTED;
  }
  else if ( ! isMaterialCompatible(component->material()))
  {
    // reject
    result = EGE_ERROR_NOT_SUPPORTED;
  }
  else if (component->material()->pass(0)->textureCount() != component->vertexBuffer()->vertexDeclaration().elementCount(NVertexBuffer::VES_TEXTURE_UV))
  {
    // reject
    result = EGE_ERROR_NOT_SUPPORTED;
  }

  if (EGE_SUCCESS == result)
  {
    // append component
    result = appendComponent(component, modelMatrix) ? EGE_SUCCESS : EGE_ERROR;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult BatchedRenderQueue::addForRendering(const PRenderQueue& queue)
{
  EGE_UNUSED(queue);
  return EGE_ERROR_NOT_SUPPORTED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void BatchedRenderQueue::clear()
{
  if (NULL != m_renderData)
  {
    // reset vertex and index data
    m_renderData->vertexBuffer()->setSize(0);

    if (0 != m_renderData->indexBuffer()->indexSize())
    {
      m_renderData->indexBuffer()->setSize(0);
    }

    // reset material
    m_renderData->setMaterial(NULL);

    // reset clip region
    m_renderData->setClipRect(Rectf::INVALID);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void BatchedRenderQueue::prepareRenderList(RenderDataList& list)
{
  if ((NULL != m_renderData) && (NULL != m_renderData->material()) && (0 < m_renderData->vertexBuffer()->vertexCount()))
  {
    SRENDERDATA data;
    data.component    = m_renderData;
    data.modelMatrix  = Matrix4f::IDENTITY;

    list.append(data);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool BatchedRenderQueue::allocateMasterRenderComponent(const PRenderComponent& component)
{
  bool result = true;

  // allocate master component
  m_renderData = ege_new RenderComponent(component->engine(), String::Format("BatchedRenderQueue@%d", component->priority()), 
                                         component->vertexBuffer()->vertexDeclaration(), component->priority(), component->primitiveType(), 
                                         NVertexBuffer::UT_DYNAMIC_WRITE);
  if ((NULL == m_renderData) || ! m_renderData->isValid())
  {
    // error!
    m_renderData = NULL;
    result = false;
  }

  // check if index data is present
  if (0 != component->indexBuffer()->indexSize())
  {
    // fix index size to some mode which allow significant batching
    m_renderData->indexBuffer()->setIndexSize(EGEIndexBuffer::IS_16BIT);
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool BatchedRenderQueue::appendComponent(const PRenderComponent& component, const Matrix4f& modelMatrix)
{
  bool result = false;

  // get texture rectangle
  Rectf textureRect = Rectf::UNIT;

  // number of texture coords must EQUAL number of textures images for proper UV conversion
  EGE_ASSERT(component->material()->pass(0)->textureCount() == component->vertexBuffer()->vertexDeclaration().elementCount(NVertexBuffer::VES_TEXTURE_UV));

  // get texture transformations
  List<const Rectf*> textureRects;
  for (u32 i = 0 ; i < component->material()->pass(0)->textureCount(); ++i)
  {
    textureRects.append(&component->material()->pass(0)->texture(i)->rect());
  }

  // append vertex buffer first
  if (appendBuffer(component->vertexBuffer(), textureRects, modelMatrix))
  {
    // append index buffer
    result = appendBuffer(component->indexBuffer());
  }
  
  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool BatchedRenderQueue::appendBuffer(const PVertexBuffer& buffer, const List<const Rectf*>& textureRects, const Matrix4f& modelMatrix)
{
  bool result = false;

  // get primitive data
  EGEGraphics::RenderPrimitiveType primitiveType = m_renderData->primitiveType();

  VertexBuffer* vertexBufferOut = m_renderData->vertexBuffer();

  // check if no data yet
  bool firstAppend = (0 == vertexBufferOut->vertexCount());
     
  // calculate number of vertices to process
  u32 verticesToProcess = 0;
  switch (primitiveType)
  {
    case EGEGraphics::RPT_TRIANGLE_STRIPS:  
      
      // +2 for degenerate vertices if not first append
      verticesToProcess = buffer->vertexCount() + ((firstAppend) ? 0 : 2); 
      break;   
  
    case EGEGraphics::RPT_TRIANGLES:  

      verticesToProcess = buffer->vertexCount(); 
      break;

    default:

      EGE_ASSERT_X(false, "Implement");
      break;
  }

  // update size to accommodate new vertices
  if (vertexBufferOut->setSize(vertexBufferOut->vertexCount() + verticesToProcess))
  {
    // lock buffers to get access to data
    u8* outData       = reinterpret_cast<u8*>(vertexBufferOut->lock(vertexBufferOut->vertexCount() - verticesToProcess, verticesToProcess));
    const u8* inData  = reinterpret_cast<u8*>(buffer->lock(0, buffer->vertexCount()));

    EGE_ASSERT((NULL != outData) && (NULL != inData));

    if ((NULL != outData) && (NULL != inData))
    {
      switch (primitiveType)
      {
        case EGEGraphics::RPT_TRIANGLES:

          // convert all incoming vertices
          if (Matrix4f::IDENTITY == modelMatrix)
          {
            convertVertices(reinterpret_cast<float32*>(outData), reinterpret_cast<const float32*>(inData), verticesToProcess, textureRects);
          }
          else
          {
            convertVertices(reinterpret_cast<float32*>(outData), reinterpret_cast<const float32*>(inData), verticesToProcess, textureRects, modelMatrix);
          }
          break;

        case EGEGraphics::RPT_TRIANGLE_STRIPS:

          // check if NOT first append
          if ( ! firstAppend)
          {
            u8* duplicateVertexData = outData - vertexBufferOut->vertexDeclaration().vertexSize();

            List<const Rectf*> textureRect;
            textureRect.append(&Rectf::UNIT);

            // duplicate last vertex
            convertVertices(reinterpret_cast<float32*>(outData), reinterpret_cast<const float32*>(duplicateVertexData), 1, textureRect, Matrix4f::IDENTITY);
            outData += vertexBufferOut->vertexDeclaration().vertexSize();

            // duplicate first vertex from input buffer
            convertVertices(reinterpret_cast<float32*>(outData), reinterpret_cast<const float32*>(inData), 1, textureRect, modelMatrix);
            outData += vertexBufferOut->vertexDeclaration().vertexSize();

            // update number of vertices to be converted
            verticesToProcess -= 2;
          }

          // convert all incoming vertices
          convertVertices(reinterpret_cast<float32*>(outData), reinterpret_cast<const float32*>(inData), verticesToProcess, textureRects, modelMatrix);
          break;
          
        default:

          EGE_ASSERT_X(false, "Implement");
          break;
      }

      // success
      result = true;
    }

    // unlock
    vertexBufferOut->unlock(NULL);
    buffer->unlock(NULL);
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool BatchedRenderQueue::appendBuffer(const PIndexBuffer& buffer)
{
  bool result = false;

  // get primitive data
  EGEGraphics::RenderPrimitiveType primitiveType = m_renderData->primitiveType();

  IndexBuffer* indexBufferOut = m_renderData->indexBuffer();

  // check if no data yet
  bool firstAppend = (0 == indexBufferOut->indexCount());

  // calculate number of indices to process
  u32 indiciesToProcess = 0;
  switch (primitiveType)
  {
    case EGEGraphics::RPT_TRIANGLE_STRIPS:  
      
      // +2 for degenerate vertices if not first append
      indiciesToProcess = buffer->indexCount() + ((firstAppend) ? 0 : 2); 
      break;   
  
    case EGEGraphics::RPT_TRIANGLES:  

      indiciesToProcess = buffer->indexCount(); 
      break;

    default:

      EGE_ASSERT_X(false, "Implement");
      break;
  }

  // check if anything to append
  if (0 != indiciesToProcess)
  {
    // store old number of indicies for later use
    const u32 oldCount = indexBufferOut->indexCount();

    EGE_ASSERT((1 << 16) > (indiciesToProcess + oldCount));

    // update size to accommodate new vertices
    if (indexBufferOut->setSize(oldCount + indiciesToProcess))
    {
      // lock buffers to get access to data
      u16* outData = reinterpret_cast<u16*>(indexBufferOut->lock(oldCount, indiciesToProcess));
      void* inData  = buffer->lock(0, buffer->indexCount());

      EGE_ASSERT((NULL != outData) && (NULL != inData));

      u8* inData8   = reinterpret_cast<u8*>(inData);
      u16* inData16 = reinterpret_cast<u16*>(inData);
      u32* inData32 = reinterpret_cast<u32*>(inData);

      if ((NULL != outData) && (NULL != inData))
      {
        // go thru all vertices
        for (u32 i = 0; i < indiciesToProcess; ++i)
        {
          // TAGE - optimize
          switch (buffer->indexSize())
          {
            case 1: *outData++ = (*inData8++) + oldCount; break;
            case 2: *outData++ = (*inData16++) + oldCount; break;
            case 4: *outData++ = (*inData32++) + oldCount; break;

            default:

              EGE_ASSERT_X(false, "WOOT");
              break;
          }
        }

        // success
        result = true;
      }

      // unlock
      indexBufferOut->unlock(outData ? (outData - 1) : NULL);

      // TAGE - optimize
      switch (buffer->indexSize())
      {
        case 1: buffer->unlock(inData8 ? (inData8 - 1) : NULL); break;
        case 2: buffer->unlock(inData16 ? (inData16 - 1) : NULL); break;
        case 4: buffer->unlock(inData32 ? (inData32 - 1) : NULL); break;

        default:

          EGE_ASSERT_X(false, "WOOT");
          break;
      }
    }
  }
  else
  {
    // nothing to append
    result = true;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void BatchedRenderQueue::convertVertices(float32* outData, const float32* inData, u32 count, const List<const Rectf*>& textureRects, 
                                         const Matrix4f& modelMatrix) const
{
  Vector4f position;

  // get vertex elements
  const VertexElementArray& vertexElements = m_renderData->vertexBuffer()->vertexDeclaration().vertexElements();

  // go thru all vertices
  for (u32 i = 0; i < count; ++i)
  {
    // point to first texture coords
    List<const Rectf*>::ConstIterator itTextureRect = textureRects.begin();
  
    VertexElementArray::const_iterator itLast = vertexElements.end();
    for (VertexElementArray::const_iterator it = vertexElements.begin(); it != itLast; ++it)
    {
      switch (it->semantic())
      {
        case NVertexBuffer::VES_POSITION_XY:

          position.x = *inData++;
          position.y = *inData++;
          position.z = 0.0f;
          position.w = 1.0f;

          position = modelMatrix * position;

          *outData++ = position.x;
          *outData++ = position.y;
          break;

        case NVertexBuffer::VES_POSITION_XYZ:

          position.x = *inData++;
          position.y = *inData++;
          position.z = *inData++;
          position.w = 1.0f;

          position = modelMatrix * position;

          *outData++ = position.x;
          *outData++ = position.y;
          *outData++ = position.z;
          break;

        case NVertexBuffer::VES_NORMAL:

          *outData++ = *inData++;
          *outData++ = *inData++;
          *outData++ = *inData++;
          break;

        case NVertexBuffer::VES_TEXTURE_UV:

          *outData++ = (*itTextureRect)->x + (*inData++) * (*itTextureRect)->width;
          *outData++ = (*itTextureRect)->y + (*inData++) * (*itTextureRect)->height;

          // update number of processed coords
          ++itTextureRect;
          break;

        case NVertexBuffer::VES_COLOR_RGBA:

          *outData++ = *inData++;
          *outData++ = *inData++;
          *outData++ = *inData++;
          *outData++ = *inData++;
          break;

        case NVertexBuffer::VES_TANGENT:

          *outData++ = *inData++;
          *outData++ = *inData++;
          break;

        case NVertexBuffer::VES_POINT_SPRITE_SIZE:

          *outData++ = *inData++;
          break;

        default:

          EGE_ASSERT_X(false, "Implement!!!");
          break;
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void BatchedRenderQueue::convertVertices(float32* outData, const float32* inData, u32 count, const List<const Rectf*>& textureRects) const
{
  // get vertex elements
  const VertexElementArray& vertexElements = m_renderData->vertexBuffer()->vertexDeclaration().vertexElements();

  // go thru all vertices
  for (u32 i = 0; i < count; ++i)
  {
    // point to first texture coords
    List<const Rectf*>::ConstIterator itTextureRect = textureRects.begin();
  
    VertexElementArray::const_iterator itLast = vertexElements.end();
    for (VertexElementArray::const_iterator it = vertexElements.begin(); it != itLast; ++it)
    {
      switch (it->semantic())
      {
        case NVertexBuffer::VES_POSITION_XY:

          *outData++ = *inData++;
          *outData++ = *inData++;
          break;

        case NVertexBuffer::VES_POSITION_XYZ:

          *outData++ = *inData++;
          *outData++ = *inData++;
          *outData++ = *inData++;
          break;

        case NVertexBuffer::VES_NORMAL:

          *outData++ = *inData++;
          *outData++ = *inData++;
          *outData++ = *inData++;
          break;

        case NVertexBuffer::VES_TEXTURE_UV:

          *outData++ = (*itTextureRect)->x + (*inData++) * (*itTextureRect)->width;
          *outData++ = (*itTextureRect)->y + (*inData++) * (*itTextureRect)->height;

          // update number of processed coords
          ++itTextureRect;
          break;

        case NVertexBuffer::VES_COLOR_RGBA:

          *outData++ = *inData++;
          *outData++ = *inData++;
          *outData++ = *inData++;
          *outData++ = *inData++;
          break;

        case NVertexBuffer::VES_TANGENT:

          *outData++ = *inData++;
          *outData++ = *inData++;
          break;

        case NVertexBuffer::VES_POINT_SPRITE_SIZE:

          *outData++ = *inData++;
          break;

        default:

          EGE_ASSERT_X(false, "Implement!!!");
          break;
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool BatchedRenderQueue::isMaterialCompatible(const PMaterial& material) const
{
  return (*material == *m_renderData->material());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void BatchedRenderQueue::prepare(const PRenderComponent& component)
{
  // clone initial material
  PMaterial newMaterial = component->material()->clone();

  // reset texture matrices
  for (u32 pass = 0; pass < newMaterial->passCount(); ++pass)
  {
    u32 textureCount = newMaterial->pass(pass)->textureCount();
    for (u32 i = 0; i < textureCount; ++i)
    {
      PTextureImage textureImage = newMaterial->pass(pass)->texture(i);
      if (NULL != textureImage)
      {
        // reset texture rectangle as all texture coordinates will be recalculated
        textureImage->setRect(Rectf::UNIT);
      }
    }
  }

  // set new material
  m_renderData->setMaterial(newMaterial);

  // copy clip rect
  m_renderData->setClipRect(component->clipRect());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END