#include "Core/UI/UIPageIndicator.h"
#include <EGEOverlay.h>
#include <EGEMath.h>
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define INDICATOR_SIZE    10.0f
#define INDICATOR_SPACING 5.0f
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(UIPageIndicator)
EGE_DEFINE_DELETE_OPERATORS(UIPageIndicator)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UIPageIndicator::UIPageIndicator(Application* app, const String& name, egeObjectDeleteFunc deleteFunc) 
: Widget(app, name, EGE_OBJECT_UID_UI_PAGE_INDICATOR, deleteFunc),
  m_pageCount(0),
  m_page(0)
{
  setSize(Vector2f(INDICATOR_SIZE, INDICATOR_SIZE));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UIPageIndicator::~UIPageIndicator()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of widget. This method is a registration method for factory. */
PWidget UIPageIndicator::Create(Application* app, const String& name)
{
  // allocate object
  PUIPageIndicator object = ege_new UIPageIndicator(app, name);
  if (NULL != object)
  {
    // construct
    if (EGE_SUCCESS != object->construct())
    {
      // error!
      object = NULL;
    }
  }

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Widget override. Constructs object. */
EGEResult UIPageIndicator::construct()
{
  EGEResult result;

  // call base class
  if (EGE_SUCCESS != (result = Widget::construct()))
  {
    // error!
    return result;
  }

  // create render data
  PMaterial material = ege_new Material(app());
  if (NULL == material)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  PRenderPass renderPass = material->addPass(NULL);
  if (NULL == renderPass)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // setup material
  //material->setSrcBlendFactor(EGEGraphics::BF_SRC_ALPHA);
  //material->setDstBlendFactor(EGEGraphics::BF_ONE_MINUS_SRC_ALPHA);
    
  // create render data
  m_renderData = ege_new RenderComponent(app(), "page-indicator", EGEGraphics::RP_MAIN + 1, EGEGraphics::RPT_TRIANGLES);
  if (NULL == m_renderData)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  m_renderData->setMaterial(material);
  if (!m_renderData->vertexBuffer()->setSemantics(EGEVertexBuffer::ST_V2_C4))
  {
    // error!
    return EGE_ERROR;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Widget override. Returns TRUE if widget is frameless. */
bool UIPageIndicator::isFrameless() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Widget override. Initializes widget from dictionary. */
bool UIPageIndicator::initialize(const Dictionary& params)
{
  // initialize base
  bool error = !Widget::initialize(params);

  return !error;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets page count. */
void UIPageIndicator::setPageCount(s32 count)
{
  if (m_pageCount != count)
  {
    m_pageCount = Math::Max(count, 0);

    // invalidate render data
    m_renderDataInvalid = true;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns number of pages. */
s32 UIPageIndicator::pageCount() const
{
  return m_pageCount;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets current page. */
void UIPageIndicator::setCurrentPage(s32 pageIdx)
{
  if ((pageIdx != m_page) && (0 < pageCount()))
  {
    m_page = Math::Bound(pageIdx, 0, m_pageCount - 1);

    // invalidate render data
    m_renderDataInvalid = true;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Widget override. Updates render data. */
void UIPageIndicator::updateRenderData()
{
  Vector2f indicatorSize(INDICATOR_SIZE * pageCount() + INDICATOR_SPACING * (pageCount() - 1), INDICATOR_SIZE);
  Vector2f offset = (size() - indicatorSize) * 0.5f;

  // update render data
  float32* data = reinterpret_cast<float32*>(m_renderData->vertexBuffer()->lock(0, pageCount() * 6));
  if (data)
  {
    for (s32 i = 0; i < pageCount(); ++i)
    {
      Color color = Color::GRAY;
      if (i == page())
      {
        color = Color::WHITE;
      }

      // top left
      *data++ = offset.x + i * (INDICATOR_SIZE + INDICATOR_SPACING);
      *data++ = offset.y;
      *data++ = color.red;
      *data++ = color.green;
      *data++ = color.blue;
      *data++ = color.alpha;

      // bottom left
      *data++ = offset.x + i * (INDICATOR_SIZE + INDICATOR_SPACING);
      *data++ = offset.y + INDICATOR_SIZE;
      *data++ = color.red;
      *data++ = color.green;
      *data++ = color.blue;
      *data++ = color.alpha;

      // bottom right
      *data++ = offset.x + i * (INDICATOR_SIZE + INDICATOR_SPACING) + INDICATOR_SIZE;
      *data++ = offset.y + INDICATOR_SIZE;
      *data++ = color.red;
      *data++ = color.green;
      *data++ = color.blue;
      *data++ = color.alpha;

      // top left
      *data++ = offset.x + i * (INDICATOR_SIZE + INDICATOR_SPACING);
      *data++ = offset.y;
      *data++ = color.red;
      *data++ = color.green;
      *data++ = color.blue;
      *data++ = color.alpha;

      // bottom right
      *data++ = offset.x + i * (INDICATOR_SIZE + INDICATOR_SPACING) + INDICATOR_SIZE;
      *data++ = offset.y + INDICATOR_SIZE;
      *data++ = color.red;
      *data++ = color.green;
      *data++ = color.blue;
      *data++ = color.alpha;

      // top right
      *data++ = offset.x + i * (INDICATOR_SIZE + INDICATOR_SPACING) + INDICATOR_SIZE;
      *data++ = offset.y;
      *data++ = color.red;
      *data++ = color.green;
      *data++ = color.blue;
      *data++ = color.alpha;
    }
  }

  m_renderData->vertexBuffer()->unlock(data - 1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns current page. */
s32 UIPageIndicator::page() const
{
  return m_page;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Widget override. Renders object. */
void UIPageIndicator::addForRendering(Renderer* renderer, const Matrix4f& transform)
{
  if (!isVisible())
  {
    // do nothing
    return;
  }

  // update render data if required
  if (m_renderDataInvalid)
  {
    updateRenderData();

    // reset flag
    m_renderDataInvalid = false;
  }

  // render
  renderer->addForRendering(m_renderData, transform.multiply(m_physics.transformationMatrix()));

  // call base class
  Widget::addForRendering(renderer, transform);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END