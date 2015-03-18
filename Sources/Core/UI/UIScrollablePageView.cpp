#include "Core/UI/UIScrollablePageView.h"
#include "Core/UI/UIPageIndicator.h"
#include "EGEOverlay.h"
#include "EGEMath.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define PAGE_CHANGE_VELOCITY_THRESHOLD 20.0f
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(UIScrollablePageView)
EGE_DEFINE_DELETE_OPERATORS(UIScrollablePageView)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UIScrollablePageView::UIScrollablePageView(Engine& engine, const String& name, egeObjectDeleteFunc deleteFunc) 
: UIScrollView(engine, name, EGE_OBJECT_UID_UI_SCROLLABLE_PAGE_VIEW, deleteFunc)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UIScrollablePageView::~UIScrollablePageView()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PWidget UIScrollablePageView::Create(Engine& engine, const String& name)
{
  // allocate object
  PUIScrollablePageView object = ege_new UIScrollablePageView(engine, name);
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
EGEResult UIScrollablePageView::construct()
{
  EGEResult result;

  // call base class
  if (EGE_SUCCESS != (result = UIScrollView::construct()))
  {
    // error!
    return result;
  }

  // allocate page indicator
  m_pageIndicator = UIPageIndicator::Create(engine(), name() + "-page-indicator");
  if (NULL == m_pageIndicator)
  {
    // error!
    return EGE_ERROR;
  }

  // setup view
  setDirection(DIRECTION_HORIZONTAL);

  // hide scrollbars
  setScrollbarsEnabled(false);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool UIScrollablePageView::initialize(const Dictionary& params)
{
  // initialize base
  bool error = !UIScrollView::initialize(params);

  return !error;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIScrollablePageView::addForRendering(IRenderer* renderer, const Matrix4f& transform)
{
  if (!isVisible())
  {
    // do nothing
    return;
  }

  // create content objects matrix affected by current scroll offset
  Vector4f pos = m_physics.position();
  pos.x -= offset().x;
  pos.y -= offset().y;
  Matrix4f contentMatrix = Math::CreateMatrix(pos, Vector4f::ONE, Quaternionf::IDENTITY);
  contentMatrix = transform.multiply(contentMatrix);

  // determine clip rectangle for entire widget
  Matrix4f combined = transform.multiply(m_physics.transformationMatrix());

  pos = Vector4f(combined.translationX(), combined.translationY(), combined.translationZ());
  Rectf clipRect(pos.x, pos.y, size().x, size().y);

  // render objects
  s32 fromPage  = Math::Max(m_pageIndicator->page() - 1, 0);
  s32 toPage    = Math::Min(m_pageIndicator->page() + 1, m_pageIndicator->pageCount() - 1);
  for (s32 i = fromPage; i <= toPage; ++i)
  {
    const ObjectsList& list = m_objects[i];

    // apply page offset to content matrix
    Matrix4f matrix = contentMatrix;
    matrix.setTranslation(matrix.translationX() + i * pageSize().x, matrix.translationY(), matrix.translationZ());

    for (ObjectsList::ConstIterator it = list.begin(); it != list.end(); ++it)
    {
      switch ((*it)->uid())
      {
        case EGE_OBJECT_UID_OVERLAY_TEXT:
          {
            TextOverlay* overlay = ege_cast<TextOverlay*>(*it);

            overlay->renderData()->setClipRect(clipRect);
            overlay->addForRendering(renderer, matrix);
          }
          break;

        case EGE_OBJECT_UID_OVERLAY_IMAGE:
          {
            ImageOverlay* overlay = ege_cast<ImageOverlay*>(*it);

            overlay->renderData()->setClipRect(clipRect);
            overlay->addForRendering(renderer, matrix);
          }
          break;
      }
    }
  }

  // render page indicator at a bottom of a widget
  m_pageIndicator->addForRendering(renderer, transform);

  // call base class
  UIScrollView::addForRendering(renderer, transform);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIScrollablePageView::update(const Time& time)
{
  // update objects
  s32 fromPage  = Math::Max(m_pageIndicator->page() - 1, 0);
  s32 toPage    = Math::Min(m_pageIndicator->page() + 1, m_pageIndicator->pageCount() - 1);
  for (s32 i = fromPage; i <= toPage; ++i)
  {
    const ObjectsList& list = m_objects[i];
    for (ObjectsList::ConstIterator it = list.begin(); it != list.end(); ++it)
    {
      switch ((*it)->uid())
      {
        case EGE_OBJECT_UID_OVERLAY_TEXT:
          {
            TextOverlay* overlay = ege_cast<TextOverlay*>(*it);

            overlay->update(time);
          }
          break;

        case EGE_OBJECT_UID_OVERLAY_IMAGE:
          {
            ImageOverlay* overlay = ege_cast<ImageOverlay*>(*it);

            overlay->update(time);
          }
          break;
      }
    }
  }

  // call base class
  UIScrollView::update(time);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIScrollablePageView::setPageIndicatorEnabled(bool set)
{
  if (m_pageIndicator->isVisible() != set)
  {
    m_pageIndicator->setVisible(set);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool UIScrollablePageView::isPageIndicatorEnabled() const
{
  return m_pageIndicator->isVisible();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult UIScrollablePageView::addObject(PObject object, u32 pageIdx)
{
  EGEResult result = EGE_ERROR_NOT_SUPPORTED;

  // sanity check
  if (NULL == object)
  {
    // error!
    return EGE_ERROR;
  }

  // process according to object id
  switch (object->uid())
  {
    case EGE_OBJECT_UID_OVERLAY_TEXT:
    case EGE_OBJECT_UID_OVERLAY_IMAGE:

      // add to pool
      m_objects[pageIdx].append(object);

      result = EGE_SUCCESS;
      break;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIScrollablePageView::removeObject(PObject object)
{
  // locate object
  for (PageObjects::Iterator it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    ObjectsList& list = it->second;
    for (ObjectsList::Iterator itList = list.begin(); itList != list.end(); ++itList)
    {
      PObject& listObject = *itList;
      if (object == listObject)
      {
        // remove from list
        list.remove(itList);

        // done
        return;
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PObject UIScrollablePageView::object(const String& name) const
{
  // locate object
  for (PageObjects::ConstIterator it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    const ObjectsList& list = it->second;
    for (ObjectsList::ConstIterator itList = list.begin(); itList != list.end(); ++itList)
    {
      // process according to object id
      switch ((*itList)->uid())
      {
        case EGE_OBJECT_UID_OVERLAY_TEXT:
          {
            TextOverlay* overlay = ege_cast<TextOverlay*>(*itList);
          
            // check if found
            if (overlay->name() == name)
            {
              // found
              return (*itList);
            }
          }
          break;

        case EGE_OBJECT_UID_OVERLAY_IMAGE:
          {
            ImageOverlay* overlay = ege_cast<ImageOverlay*>(*itList);
          
            // check if found
            if (overlay->name() == name)
            {
              // found
              return (*itList);
            }
          }
          break;
      }
    }
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UIScrollablePageView::ObjectsList UIScrollablePageView::objects(s32 pageIdx) const
{
  ObjectsList list;

  // check if all objects to be returned
  if (0 > pageIdx)
  {
    // go thru all pages
    for (s32 i = 0; i < static_cast<s32>(m_objects.size()); ++i)
    {
      list << objects(i);
    }
  }
  else
  {
    if (m_objects.contains(pageIdx))
    {
      list << m_objects.value(pageIdx);
    }
  }

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIScrollablePageView::removeAllObjects()
{
  m_objects.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIScrollablePageView::setPageCount(s32 count)
{
  if (count != m_pageIndicator->pageCount())
  {
    // set page count
    m_pageIndicator->setPageCount(count);

    // update content size
    recalculateContentArea();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIScrollablePageView::setPage(s32 page, bool animate)
{
  // make sure its in range
  page = Math::Clamp(page, 0, m_pageIndicator->pageCount() - 1);

  // scroll to page
  Vector2f offset(page * pageSize().x, 0);
  setOffset(offset, animate);

  // set new page
  m_pageIndicator->setCurrentPage(page);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 UIScrollablePageView::page() const
{
  return m_pageIndicator->page();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIScrollablePageView::recalculateContentArea()
{
  Vector2f contentSize(m_pageIndicator->pageCount() * size().x, size().y);
  setContentSize(contentSize);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIScrollablePageView::onSizeChanged(const Vector2f& size)
{
  // set page indicator size
  m_pageIndicator->setSize(Vector2f(size.x, m_pageIndicator->size().y));

  // position page indicator
  m_pageIndicator->setPosition(Vector4f(m_physics.position().x, m_physics.position().y + size.y, 0));

  // recalculate content area
  recalculateContentArea();

  // call base class
  UIScrollView::onSizeChanged(size);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIScrollablePageView::onPositionChanged(const Vector4f& position)
{
  // position page indicator
  m_pageIndicator->setPosition(Vector4f(position.x, position.y + size().y, 0));

  // call base class
  UIScrollView::onPositionChanged(position);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector2f UIScrollablePageView::pageSize() const
{
  return size();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIScrollablePageView::endMove()
{
  s32 page;

  // reset state
  m_state = STATE_IDLE;

  // check if velocity over the page change threshold
  if ((PAGE_CHANGE_VELOCITY_THRESHOLD * throwCoefficient()) < Math::Abs(scrollVelocity().x))
  {
    // change page according to velocity vector direction
    if (0 < scrollVelocity().x)
    {
      page = m_pageIndicator->page() + 1;
    }
    else
    {
      page = m_pageIndicator->page() - 1;
    }
  }
  else
  {
    // calculate page from current offset
    page = Math::Floor((offset().x - pageSize().x * 0.5f) / pageSize().x) + 1;
  }

  // make sure its in range
  page = Math::Clamp(page, 0, m_pageIndicator->pageCount() - 1);

  // scroll to page
  setOffset(Vector2f(page * pageSize().x, 0), true);

  // set new page
  m_pageIndicator->setCurrentPage(page);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIScrollablePageView::setBaseRenderPriority(s32 priority)
{
  m_pageIndicator->setRenderPriority(priority + 1);

  // call base class
  UIScrollView::setBaseRenderPriority(priority);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PUIPageIndicator UIScrollablePageView::pageIndicator() const
{
  return m_pageIndicator;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END