#include "Core/UI/WidgetFrame.h"
#include <EGETexture.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(WidgetFrame)
EGE_DEFINE_DELETE_OPERATORS(WidgetFrame)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
WidgetFrame::WidgetFrame(Application* app) : m_renderDataInvalid(true)
{
  m_renderData  = ege_new RenderComponent(app, "widget-frame", EGEGraphics::RP_MAIN, EGEGraphics::RPT_TRIANGLES, EGEVertexBuffer::UT_STATIC_WRITE);
  if (m_renderData)
  {
    m_renderData->indexBuffer()->setIndexSize(EGEIndexBuffer::IS_8BIT);
    if (!m_renderData->vertexBuffer()->setSemantics(EGEVertexBuffer::ST_V2_T2))
    {
      // error!
      m_renderData = NULL;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
WidgetFrame::~WidgetFrame()
{
  m_renderData = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool WidgetFrame::isValid() const
{
  return (NULL != m_renderData);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void WidgetFrame::addForRendering(IRenderer* renderer, const Matrix4f& transform)
{
  // regenerate render data if required
  if (m_renderDataInvalid)
  {
    generateRenderData();

    // validate
    m_renderDataInvalid = false;
  }

  renderer->addForRendering(m_renderData, transform);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void WidgetFrame::setMaterial(const PMaterial& material)
{
  EGE_ASSERT(m_renderData);
  m_renderData->setMaterial(material);

  // invalidate render data
  m_renderDataInvalid = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void WidgetFrame::setRects(const Recti& topLeft, const Recti& topMiddle, const Recti& topRight, const Recti& middleLeft, const Recti& fill, 
                           const Recti& middleRight, const Recti& bottomLeft, const Recti& bottomMiddle, const Recti& bottomRight)
{
  m_topLeftRect       = topLeft;
  m_topMiddleRect     = topMiddle;
  m_topRightRect      = topRight;
  m_middleLeftRect    = middleLeft;
  m_fillRect          = fill;
  m_middleRightRect   = middleRight;
  m_bottomLeftRect    = bottomLeft;
  m_bottomMiddleRect  = bottomMiddle;
  m_bottomRightRect   = bottomRight;

  // invalidate render data
  m_renderDataInvalid = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void WidgetFrame::generateRenderData()
{
  Vector2f frameContentSize = m_size - minSize();
  frameContentSize.x = Math::Max(frameContentSize.x, 0.0f);
  frameContentSize.y = Math::Max(frameContentSize.y, 0.0f);

  /* NOTE: WidgetFrame widget is divided into following sections:
   *
   *      *------*------------------------------------------------------*------*
   *      |      |                                                      |      |
   *      |   0  |                        1                             |  2   |
   *      |      |                                                      |      |
   *      *------*------------------------------------------------------*------*
   *      |      |                                                      |      |
   *      |      |                                                      |      |
   *      |  3   |                        4                             |  5   |
   *      |      |                                                      |      |
   *      |      |                                                      |      |
   *      *------*------------------------------------------------------*------*
   *      |      |                                                      |      |
   *      |  6   |                        7                             |  8   |
   *      |      |                                                      |      |
   *      *------*------------------------------------------------------*------*
   */

  // set buffer sizes
  if (m_renderData->vertexBuffer()->setSize(9 * 4) && m_renderData->indexBuffer()->setSize(9 * 4))
  {
    float32* data = reinterpret_cast<float32*>(m_renderData->vertexBuffer()->lock(0, 9 * 4));
    u8* indexData = reinterpret_cast<u8*>(m_renderData->indexBuffer()->lock(0, 9 * 6));

    // TAGE - need to find better way of doing this...
    Vector2f textureSize(m_renderData->material()->pass(0)->texture(0)->width() * 1.0f, m_renderData->material()->pass(0)->texture(0)->height() * 1.0f);
    textureSize.set(1.0f / textureSize.x, 1.0f / textureSize.y);

    // go thru all sections
    for (u8 i = 0; i < 9; ++i)
    {
      Rectf vertexData;
      Rectf uvData;
    
      switch (i)
      {
        case 0:

          vertexData.x      = 0.0f;
          vertexData.y      = 0.0f;
          vertexData.width  = static_cast<float32>(m_topLeftRect.width);
          vertexData.height = static_cast<float32>(m_topLeftRect.height);
          uvData.x          = m_topLeftRect.x * textureSize.x;
          uvData.y          = m_topLeftRect.y * textureSize.y;
          uvData.width      = m_topLeftRect.width * textureSize.x;
          uvData.height     = m_topLeftRect.height * textureSize.y;
          break;

        case 1:

          vertexData.x      = static_cast<float32>(m_topLeftRect.width);
          vertexData.y      = 0.0f;
          vertexData.width  = static_cast<float32>(frameContentSize.x);
          vertexData.height = static_cast<float32>(m_topMiddleRect.height);
          uvData.x          = m_topMiddleRect.x * textureSize.x;
          uvData.y          = m_topMiddleRect.y * textureSize.y;
          uvData.width      = m_topMiddleRect.width * textureSize.x;
          uvData.height     = m_topMiddleRect.height * textureSize.y;
          break;

        case 2:

          vertexData.x      = static_cast<float32>(m_topLeftRect.width + frameContentSize.x);
          vertexData.y      = 0.0f;
          vertexData.width  = static_cast<float32>(m_topRightRect.width);
          vertexData.height = static_cast<float32>(m_topRightRect.height);
          uvData.x          = m_topRightRect.x * textureSize.x;
          uvData.y          = m_topRightRect.y * textureSize.y;
          uvData.width      = m_topRightRect.width * textureSize.x;
          uvData.height     = m_topRightRect.height * textureSize.y;
          break;

        case 3:

          vertexData.x      = 0.0f;
          vertexData.y      = static_cast<float32>(m_topLeftRect.height);
          vertexData.width  = static_cast<float32>(m_middleLeftRect.width);
          vertexData.height = static_cast<float32>(frameContentSize.y);
          uvData.x          = m_middleLeftRect.x * textureSize.x;
          uvData.y          = m_middleLeftRect.y * textureSize.y;
          uvData.width      = m_middleLeftRect.width * textureSize.x;
          uvData.height     = m_middleLeftRect.height * textureSize.y;
          break;

        case 4:

          vertexData.x      = static_cast<float32>(m_middleLeftRect.width);
          vertexData.y      = static_cast<float32>(m_topMiddleRect.height);
          vertexData.width  = static_cast<float32>(frameContentSize.x);
          vertexData.height = static_cast<float32>(frameContentSize.y);
          uvData.x          = m_fillRect.x * textureSize.x;
          uvData.y          = m_fillRect.y * textureSize.y;
          uvData.width      = m_fillRect.width * textureSize.x;
          uvData.height     = m_fillRect.height * textureSize.y;
          break;

        case 5:

          vertexData.x      = static_cast<float32>(m_middleLeftRect.width + frameContentSize.x);
          vertexData.y      = static_cast<float32>(m_topRightRect.height);
          vertexData.width  = static_cast<float32>(m_middleRightRect.width);
          vertexData.height = static_cast<float32>(frameContentSize.y);
          uvData.x          = m_middleRightRect.x * textureSize.x;
          uvData.y          = m_middleRightRect.y * textureSize.y;
          uvData.width      = m_middleRightRect.width * textureSize.x;
          uvData.height     = m_middleRightRect.height * textureSize.y;
          break;

        case 6:

          vertexData.x      = 0.0f;
          vertexData.y      = static_cast<float32>(m_topLeftRect.height + frameContentSize.y);
          vertexData.width  = static_cast<float32>(m_bottomLeftRect.width);
          vertexData.height = static_cast<float32>(m_bottomLeftRect.height);
          uvData.x          = m_bottomLeftRect.x * textureSize.x;
          uvData.y          = m_bottomLeftRect.y * textureSize.y;
          uvData.width      = m_bottomLeftRect.width * textureSize.x;
          uvData.height     = m_bottomLeftRect.height * textureSize.y;
          break;

        case 7:

          vertexData.x      = static_cast<float32>(m_bottomLeftRect.width);
          vertexData.y      = static_cast<float32>(m_topLeftRect.height + frameContentSize.y);
          vertexData.width  = static_cast<float32>(frameContentSize.x);
          vertexData.height = static_cast<float32>(m_bottomMiddleRect.height);
          uvData.x          = m_bottomMiddleRect.x * textureSize.x;
          uvData.y          = m_bottomMiddleRect.y * textureSize.y;
          uvData.width      = m_bottomMiddleRect.width * textureSize.x;
          uvData.height     = m_bottomMiddleRect.height * textureSize.y;
          break;

        case 8:

          vertexData.x      = static_cast<float32>(m_bottomLeftRect.width + frameContentSize.x);
          vertexData.y      = static_cast<float32>(m_topRightRect.height + frameContentSize.y);
          vertexData.width  = static_cast<float32>(m_bottomRightRect.width);
          vertexData.height = static_cast<float32>(m_bottomRightRect.height);
          uvData.x          = m_bottomRightRect.x * textureSize.x;
          uvData.y          = m_bottomRightRect.y * textureSize.y;
          uvData.width      = m_bottomRightRect.width * textureSize.x;
          uvData.height     = m_bottomRightRect.height * textureSize.y;
          break;
      }

      // update vertex data
      *data++ = vertexData.x;
      *data++ = vertexData.y;
      *data++ = uvData.x;
      *data++ = uvData.y;

      *data++ = vertexData.x;
      *data++ = vertexData.y + vertexData.height;
      *data++ = uvData.x;
      *data++ = uvData.y + uvData.height;

      *data++ = vertexData.x + vertexData.width;
      *data++ = vertexData.y + vertexData.height;
      *data++ = uvData.x + uvData.width;
      *data++ = uvData.y + uvData.height;

      *data++ = vertexData.x + vertexData.width;
      *data++ = vertexData.y;
      *data++ = uvData.x + uvData.width;
      *data++ = uvData.y;

      // update index data
      *indexData++ = i * 4 + 0;
      *indexData++ = i * 4 + 1;
      *indexData++ = i * 4 + 2;
      *indexData++ = i * 4 + 0;
      *indexData++ = i * 4 + 2;
      *indexData++ = i * 4 + 3;
    }

    m_renderData->vertexBuffer()->unlock(data - 1);
    m_renderData->indexBuffer()->unlock(indexData - 1);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void WidgetFrame::setSize(const Vector2f& size)
{
  if (m_size != size)
  {
    // store new size
    m_size = size;

    // invalidate render data
    m_renderDataInvalid = true;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector2f WidgetFrame::minSize() const
{
  Vector2f frameSize(0, 0);

  // frame only
  frameSize.x += m_topLeftRect.width + m_topRightRect.width;
  frameSize.y += m_topLeftRect.height + m_bottomLeftRect.height;

  return frameSize;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END