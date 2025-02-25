#include "Core/UI/UIPageIndicator.h"
#include "EGEOverlay.h"
#include "EGEMath.h"
#include "EGERenderer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define INDICATOR_SIZE    10
#define INDICATOR_SPACING 5.0f
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(UIPageIndicator)
EGE_DEFINE_DELETE_OPERATORS(UIPageIndicator)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UIPageIndicator::UIPageIndicator(Application* app, const String& name, egeObjectDeleteFunc deleteFunc) 
: Widget(app, name, EGE_OBJECT_UID_UI_PAGE_INDICATOR, deleteFunc),
  m_pageCount(0),
  m_page(0),
  m_renderPriority(EGEGraphics::RP_MAIN + 1),
  m_indicatorSize(INDICATOR_SIZE)
{
  setSize(Vector2f(INDICATOR_SIZE, INDICATOR_SIZE));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UIPageIndicator::~UIPageIndicator()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
EGEResult UIPageIndicator::construct()
{
  EGEResult result;

  // call base class
  if (EGE_SUCCESS != (result = Widget::construct()))
  {
    // error!
    return result;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool UIPageIndicator::isFrameless() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool UIPageIndicator::initialize(const Dictionary& params)
{
  // initialize base
  bool error = !Widget::initialize(params);

  return !error;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
s32 UIPageIndicator::pageCount() const
{
  return m_pageCount;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIPageIndicator::setCurrentPage(s32 pageIdx)
{
  if ((pageIdx != m_page) && (0 < pageCount()))
  {
    m_page = Math::Clamp(pageIdx, 0, m_pageCount - 1);

    // invalidate render data
    m_renderDataInvalid = true;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIPageIndicator::updateRenderData()
{
  // clean up
  m_renderData.clear();

  // update render data
  for (s32 i = 0; i < pageCount(); ++i)
  {
    // create render data
    Vector2f size(static_cast<float32>(m_indicatorSize), static_cast<float32>(m_indicatorSize));
    String renderDataName = String::Format("page-indicator-%d-%s", i, name().toAscii());
    PRenderComponent renderData = RenderObjectFactory::CreateQuadXY(app(), renderDataName, Vector4f::ZERO, size, ALIGN_TOP_LEFT, false, false, 
                                                                    RenderObjectFactory::VS_V2_T2, m_renderPriority, EGEGraphics::RPT_TRIANGLE_STRIPS, 
                                                                    NVertexBuffer::UT_STATIC_WRITE);

    if (NULL == renderData)
    {
      continue;
    }

    // set material
    renderData->setMaterial((page() == i) ? m_onMaterials.at(i, NULL) : m_offMaterials.at(i, NULL));

    // add to pool
    m_renderData.push_back(renderData);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 UIPageIndicator::page() const
{
  return m_page;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIPageIndicator::addForRendering(IRenderer* renderer, const Matrix4f& transform)
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
  const Vector2f indicatorSize(m_indicatorSize * pageCount() + INDICATOR_SPACING * (pageCount() - 1), static_cast<float32>(m_indicatorSize));
  const Vector2f offset = (size() - indicatorSize) * 0.5f;
  for (s32 i = 0; i < static_cast<s32>(m_renderData.size()); ++i)
  {
    Vector4f pos = Vector4f(offset.x + i * (m_indicatorSize + INDICATOR_SPACING), offset.y, 0);
    Matrix4f matrix = Math::CreateMatrix(pos, Vector4f::ONE, Quaternionf::IDENTITY);

    // update render priority
    m_renderData[i]->setPriority(m_renderPriority);

    // render
    renderer->addForRendering(m_renderData[i], transform.multiply(m_physics.transformationMatrix()).multiply(matrix));
  }

  // call base class
  Widget::addForRendering(renderer, transform);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIPageIndicator::setMaterials(DynamicArray<PMaterial> on, DynamicArray<PMaterial> off)
{
  m_onMaterials   = on;
  m_offMaterials  = off;

  // invalidate render data
  m_renderDataInvalid = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIPageIndicator::setRenderPriority(s32 priority)
{
  m_renderPriority = priority;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIPageIndicator::setIndicatorSize(s32 size)
{
  if (m_indicatorSize != size)
  {
    m_indicatorSize = size;

    // invalidate render data
    m_renderDataInvalid = true;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END