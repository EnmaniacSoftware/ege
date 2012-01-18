#include "Core/UI/Dialog.h"
#include <EGETexture.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Dialog)
EGE_DEFINE_DELETE_OPERATORS(Dialog)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Dialog::Dialog(Application* app, const String& name, egeObjectDeleteFunc deleteFunc) : Object(app, EGE_OBJECT_UID_UI_DIALOG, deleteFunc), 
                                                                                       m_name(name), 
                                                                                       m_renderDataInvalid(true), 
                                                                                       m_visible(true),
                                                                                       m_maxSize(Vector2i::ZERO),
                                                                                       m_sizeValid(false)
{
  m_physics = ege_new PhysicsComponent(app, "dialog_" + name);
  m_renderData  = ege_new RenderComponent(app, "dialog_" + name, EGEGraphics::RP_MAIN, EGEGraphics::RPT_TRIANGLES, EGEVertexBuffer::UT_STATIC_WRITE);
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
Dialog::~Dialog()
{
  m_physics     = NULL;
  m_renderData  = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool Dialog::isValid() const
{
  return (NULL != m_physics) && (NULL != m_renderData);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates overlay. */
void Dialog::update(const Time& time)
{
  EGE_UNUSED(time);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Renders element. */
void Dialog::render(const Viewport* viewport, Renderer* renderer)
{
  EGE_UNUSED(viewport);

  if (visible())
  {
    // regenerate render data if required
    if (m_renderDataInvalid)
    {
      generateRenderData();
    }

    // render
    renderer->addForRendering(m_renderData, m_physics->transformationMatrix());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets visibility. */
void Dialog::setVisible(bool set)
{
  if (m_visible != set)
  {
    m_visible = set;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets material. */
void Dialog::setMaterial(const PMaterial& material)
{
  EGE_ASSERT(m_renderData);
  m_renderData->setMaterial(material);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets rectanlges (in pixels). */
void Dialog::setRects(const Recti& topLeft, const Recti& topMiddle, const Recti& topRight, const Recti& middleLeft, const Recti& fill, const Recti& middleRight, 
                      const Recti& bottomLeft, const Recti& bottomMiddle, const Recti& bottomRight)
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
/*! Generates render data. */
void Dialog::generateRenderData()
{
  Vector2i dlgContentSize = size() - noContentSize();

  /* NOTE: Dialog widget is divided into following sections:
   *
   *     (0)    (3)                                                    (4)    (7)
   *      *------*------------------------------------------------------*------*
   *      |      |                                                      |      |
   *      |   0  |                        1                             |  2   |
   *      |      |(2)                                                (5)|      |
   *   (1)*------*------------------------------------------------------*------* (6)
   *      |      |                                                      |      |
   *      |      |                                                      |      |
   *      |  3   |                        4                             |  5   |
   *      |      |                                                      |      |
   *      |      |(11)                                              (12)|      |
   *   (8)*------*------------------------------------------------------*------*(15)
   *      |      |                                                      |      |
   *      |  6   |                        7                             |  8   |
   *      |      |                                                      |      |
   *      *------*------------------------------------------------------*------*
   *     (9)    (10)                                                   (13)   (14)
   */

  float32* data = reinterpret_cast<float32*>(m_renderData->vertexBuffer()->lock(0, 16));
  u8* indexData = reinterpret_cast<u8*>(m_renderData->indexBuffer()->lock(0, 9 * 6));

  Vector2f textureSize(ege_cast<Texture2D*>(m_renderData->material()->pass(0)->texture(0)->texture())->width() * 1.0f,
                       ege_cast<Texture2D*>(m_renderData->material()->pass(0)->texture(0)->texture())->height() * 1.0f);

  // 0
  *data++ = 0.0f;
  *data++ = 0.0f;
  *data++ = 0.0f / textureSize.x;
  *data++ = 0.0f / textureSize.y;

  // 1
  *data++ = 0.0f;
  *data++ = 0.0f + m_topLeftRect.height;
  *data++ = 0.0f / textureSize.x;
  *data++ = (0.0f + m_topLeftRect.height) / textureSize.y;

  // 2
  *data++ = 0.0f + m_topLeftRect.width;
  *data++ = 0.0f + m_topLeftRect.height;
  *data++ = (0.0f + m_topLeftRect.width) / textureSize.x;
  *data++ = (0.0f + m_topLeftRect.height) / textureSize.y;

  // 3
  *data++ = 0.0f + m_topLeftRect.width;
  *data++ = 0.0f;
  *data++ = (0.0f + m_topLeftRect.width) / textureSize.x;
  *data++ = 0.0f / textureSize.y;

  // 4
  *data++ = 0.0f + m_topLeftRect.width + dlgContentSize.x;
  *data++ = 0.0f;
  *data++ = (0.0f + m_topLeftRect.width) / textureSize.x;
  *data++ = 0.0f / textureSize.y;

  // 5
  *data++ = 0.0f + m_topLeftRect.width + dlgContentSize.x;
  *data++ = 0.0f + m_topRightRect.height;
  *data++ = (0.0f + m_topLeftRect.width) / textureSize.x;
  *data++ = (0.0f + m_topRightRect.height) / textureSize.y;

  // 6
  *data++ = 0.0f + m_topLeftRect.width + dlgContentSize.x + m_topRightRect.width;
  *data++ = 0.0f + m_topRightRect.height;
  *data++ = (0.0f + m_topLeftRect.width + m_topRightRect.width) / textureSize.x;
  *data++ = (0.0f + m_topRightRect.height) / textureSize.y;

  // 7
  *data++ = 0.0f + m_topLeftRect.width + dlgContentSize.x + m_topRightRect.width;
  *data++ = 0.0f;
  *data++ = (0.0f + m_topLeftRect.width + m_topRightRect.width) / textureSize.x;
  *data++ = 0.0f / textureSize.y;

  // 8
  *data++ = 0.0f;
  *data++ = 0.0f + m_topLeftRect.height;
  *data++ = 0.0f / textureSize.x;
  *data++ = (0.0f + m_topLeftRect.height) / textureSize.y;

  // 9 
  *data++ = 0.0f;
  *data++ = 0.0f + m_topLeftRect.height + m_bottomLeftRect.height;
  *data++ = 0.0f / textureSize.x;
  *data++ = (0.0f + m_topLeftRect.height + m_bottomLeftRect.height) / textureSize.y;

  // 10
  *data++ = 0.0f + m_bottomLeftRect.width;
  *data++ = 0.0f + m_topLeftRect.height + m_bottomLeftRect.height;
  *data++ = (0.0f + m_bottomLeftRect.width) / textureSize.x;
  *data++ = (0.0f + m_topLeftRect.height + m_bottomLeftRect.height) / textureSize.y;

  // 11
  *data++ = 0.0f + m_bottomLeftRect.width;
  *data++ = 0.0f + m_topLeftRect.height;
  *data++ = (0.0f + m_bottomLeftRect.width) / textureSize.x;
  *data++ = (0.0f + m_topLeftRect.height) / textureSize.y;

  // 12
  *data++ = 0.0f + m_bottomLeftRect.width + dlgContentSize.x;
  *data++ = 0.0f + m_topRightRect.height;
  *data++ = (0.0f + m_bottomLeftRect.width) / textureSize.x;
  *data++ = (0.0f + m_topRightRect.height) / textureSize.y;

  // 13
  *data++ = 0.0f + m_bottomLeftRect.width + dlgContentSize.x;
  *data++ = 0.0f + m_topLeftRect.height + m_bottomRightRect.height;
  *data++ = (0.0f + m_bottomLeftRect.width) / textureSize.x;
  *data++ = (0.0f + m_topLeftRect.height + m_bottomRightRect.height) / textureSize.y;

  // 14
  *data++ = 0.0f + m_bottomLeftRect.width + dlgContentSize.x + m_bottomRightRect.width;
  *data++ = 0.0f + m_topLeftRect.height + m_bottomRightRect.height;
  *data++ = (0.0f + m_bottomLeftRect.width + m_bottomRightRect.width) / textureSize.x;
  *data++ = (0.0f + m_topLeftRect.height + m_bottomRightRect.height) / textureSize.y;

  // 15
  *data++ = 0.0f + m_bottomLeftRect.width + dlgContentSize.x + m_bottomRightRect.width;
  *data++ = 0.0f + m_topLeftRect.height + m_topRightRect.height;
  *data++ = (0.0f + m_bottomLeftRect.width + m_bottomRightRect.width) / textureSize.x;
  *data++ = (0.0f + m_topLeftRect.height + m_topRightRect.height) / textureSize.y;
  

  *indexData++ = 0;
  *indexData++ = 1;
  *indexData++ = 2;
  *indexData++ = 0;
  *indexData++ = 2;
  *indexData++ = 3;

  *indexData++ = 3;
  *indexData++ = 2;
  *indexData++ = 5;
  *indexData++ = 3;
  *indexData++ = 5;
  *indexData++ = 4;

  *indexData++ = 4;
  *indexData++ = 5;
  *indexData++ = 6;
  *indexData++ = 4;
  *indexData++ = 6;
  *indexData++ = 7;

  *indexData++ = 1;
  *indexData++ = 8;
  *indexData++ = 11;
  *indexData++ = 1;
  *indexData++ = 11;
  *indexData++ = 2;

  *indexData++ = 2;
  *indexData++ = 11;
  *indexData++ = 12;
  *indexData++ = 2;
  *indexData++ = 12;
  *indexData++ = 5;

  *indexData++ = 5;
  *indexData++ = 12;
  *indexData++ = 15;
  *indexData++ = 5;
  *indexData++ = 15;
  *indexData++ = 6;

  *indexData++ = 8;
  *indexData++ = 9;
  *indexData++ = 10;
  *indexData++ = 8;
  *indexData++ = 10;
  *indexData++ = 11;

  *indexData++ = 11;
  *indexData++ = 10;
  *indexData++ = 13;
  *indexData++ = 11;
  *indexData++ = 13;
  *indexData++ = 12;

  *indexData++ = 12;
  *indexData++ = 13;
  *indexData++ = 14;
  *indexData++ = 12;
  *indexData++ = 14;
  *indexData++ = 15;

  m_renderData->vertexBuffer()->unlock();
  m_renderData->indexBuffer()->unlock();

  // validate
  m_renderDataInvalid = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Determines size of the dialog (in pixels). */
Vector2i Dialog::size()
{
  if (!m_sizeValid)
  {
    Vector2f biggestSize = Vector2f::ZERO;

    // go thru all content areas
    for (ContentAreaDataMap::const_iterator it = m_contentAreas.begin(); it != m_contentAreas.end(); ++it)
    {
      const ContentAreaData& data = it->second;

      Vector2f contentSize(300, 100);// = data.area->contentSize();

      Vector2f sizeFromContent(contentSize.x / data.rect.width, contentSize.y / data.rect.height);

      biggestSize.x = Math::Max(biggestSize.x, sizeFromContent.x);
      biggestSize.y = Math::Max(biggestSize.y, sizeFromContent.y);
    }

    m_size.x = biggestSize.x;
    m_size.y = biggestSize.y;

    if (m_size.x < noContentSize().x)
    {
      m_size.x = noContentSize().x;
    }

    if (m_size.y < noContentSize().y)
    {
      m_size.y = noContentSize().y;
    }

    // set flag
    m_sizeValid = true;
  }


  return m_size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Determines size of the content only (in pixels). */
Vector2i Dialog::contentSize() const
{
  Vector2i size(0, 0);

  // go thru all content areas
  for (ContentAreaDataMap::const_iterator it = m_contentAreas.begin(); it != m_contentAreas.end(); ++it)
  {
    const ContentAreaData& data = it->second;

    size.x += static_cast<s32>(data.area->contentSize().x);
    size.y += static_cast<s32>(data.area->contentSize().y);
  }

  return size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets max size. */
void Dialog::setMaxSize(const Vector2i& size)
{
  m_maxSize = size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Add content area. */
EGEResult Dialog::addContentArea(const String& name, const Rectf& rect)
{
  ContentAreaData contentArea;

  // check if already exists
  if (m_contentAreas.contains(name))
  {
    // error!
    return EGE_ERROR_ALREADY_EXISTS;
  }
 
  // allocate scrollable area
  contentArea.area = ege_new ScrollableArea(app());
  contentArea.rect = rect;
  if (NULL == contentArea.area)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // add to pool
  m_contentAreas.insert(name, contentArea);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Determines size of the frame only, without any content (in pixels). */
Vector2i Dialog::noContentSize() const
{
  Vector2i dlgSize(0, 0);

  // frame only
  dlgSize.x += m_topLeftRect.width + m_topRightRect.width;
  dlgSize.y += m_topLeftRect.height + m_bottomLeftRect.height;

  return dlgSize;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
