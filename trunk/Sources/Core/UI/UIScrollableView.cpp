#include "Core/UI/UIScrollableView.h"
#include <EGEOverlay.h>
#include <EGEMath.h>
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(UIScrollableView)
EGE_DEFINE_DELETE_OPERATORS(UIScrollableView)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UIScrollableView::UIScrollableView(Application* app, const String& name, egeObjectDeleteFunc deleteFunc) 
: UIScrollView(app, name, EGE_OBJECT_UID_UI_SCROLLABLE_AREA, deleteFunc),
  m_dirtyContent(false),
  m_autoContentSize(true)
{
  ege_connect(&m_physics, transformationChanged, this, UIScrollableView::transformationChanged);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UIScrollableView::~UIScrollableView()
{
  ege_disconnect(&m_physics, transformationChanged, this, UIScrollableView::transformationChanged);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of widget. This method is a registration method for factory. */
PWidget UIScrollableView::Create(Application* app, const String& name)
{
  // allocate object
  PUIScrollableView object = ege_new UIScrollableView(app, name);
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
/*! UIScrollView override. Constructs object. */
EGEResult UIScrollableView::construct()
{
  EGEResult result;

  // call base class
  if (EGE_SUCCESS != (result = UIScrollView::construct()))
  {
    // error!
    return result;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! UIScrollView override. Updates object. */
void UIScrollableView::update(const Time& time)
{
  // check if content needs to be recalculated
  if (m_dirtyContent)
  {
    // recalculate content size
    recaluclateContentSize();

    // reset flag
    m_dirtyContent = false;
  }

  // update objects
  for (ObjectsList::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    switch ((*it)->uid())
    {
      case EGE_OBJECT_UID_OVERLAY_TEXT:
      case EGE_OBJECT_UID_OVERLAY_IMAGE:
        {
          Overlay* overlay = ege_cast<Overlay*>(*it);

          overlay->update(time);
        }
        break;
    }
  }

   // call base class
  UIScrollView::update(time);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! UIScrollView override. Pointer event processor. */
void UIScrollableView::pointerEvent(PPointerData data)
{
  if (!isVisible())
  {
    // do nothing
    return;
  }

  // call base class
  UIScrollView::pointerEvent(data);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! UIScrollView override. Renders object. */
void UIScrollableView::addForRendering(Renderer* renderer, const Matrix4f& transform)
{
  if (!isVisible())
  {
    // do nothing
    return;
  }

  Vector4f pos;

  // create content objects matrix affected by current scroll offset
  Matrix4f contentMatrix = Matrix4f::IDENTITY;
  pos = m_physics.position();
  pos.x -= offset().x;
  pos.y -= offset().y;
  Math::CreateMatrix(&contentMatrix, &pos, &Vector4f::ONE, &Quaternionf::IDENTITY);
  contentMatrix = transform.multiply(contentMatrix);

  // cache position
  pos = transform.multiply(m_physics.transformationMatrix()).translation();

  // render all objects
  for (ObjectsList::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    switch ((*it)->uid())
    {
      case EGE_OBJECT_UID_OVERLAY_TEXT:
      case EGE_OBJECT_UID_OVERLAY_IMAGE:
        {
          Overlay* overlay = ege_cast<Overlay*>(*it);

          overlay->renderData()->setClipRect(Rectf(pos.x, pos.y, m_size.x, m_size.y));          
          overlay->addForRendering(renderer, contentMatrix);
        }
        break;
    }
  }

  // call base class
  UIScrollView::addForRendering(renderer, transform);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds object. */
EGEResult UIScrollableView::addObject(PObject object)
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
      m_objects.push_back(object);

      // invalidate content
      m_dirtyContent = true;

      result = EGE_SUCCESS;
      break;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes object. */
void UIScrollableView::removeObject(PObject object)
{
  m_objects.remove(object);
  m_dirtyContent = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes all objects. */
void UIScrollableView::removeAllObjects()
{
  m_objects.clear();
  m_dirtyContent = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns size of content (in pixels). */
Vector2f UIScrollableView::contentSize()
{
  if (m_dirtyContent)
  {
    // recalculate content size
    recaluclateContentSize();

    // reset flag
    m_dirtyContent = false;
  }

  return m_contentSize;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Recalculates content size. */
void UIScrollableView::recaluclateContentSize()
{
  // check if no AUTO content size calculations
  if (!m_autoContentSize)
  {
    // do nothing
    return;
  }

  Vector2f newContentSize = Vector2f::ZERO;

  // go thru all objects
  for (ObjectsList::const_iterator it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    // process according to object id
    switch ((*it)->uid())
    {
      case EGE_OBJECT_UID_OVERLAY_TEXT:
      case EGE_OBJECT_UID_OVERLAY_IMAGE:
        {
          Overlay* overlay = ege_cast<Overlay*>((*it).object());

          // update content size
          newContentSize.x = Math::Max(newContentSize.x, overlay->physics()->position().x + overlay->size().x);
          newContentSize.y = Math::Max(newContentSize.y, overlay->physics()->position().y + overlay->size().y);
        }
        break;
    }
  }

  // set new content size
  setContentSize(newContentSize);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when internal transformation is changed. */
void UIScrollableView::transformationChanged()
{
  // invalidate content
  m_dirtyContent = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Widget override. Returns TRUE if widget is frameless. */
bool UIScrollableView::isFrameless() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Widget override. Initializes widget from dictionary. */
bool UIScrollableView::initialize(const Dictionary& params)
{
  // initialize base
  bool error = !Widget::initialize(params);

  return !error;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns object with a given name. */
PObject UIScrollableView::object(const String& name) const
{
  // go thru all objects
  for (ObjectsList::const_iterator it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    // process according to object id
    switch ((*it)->uid())
    {
      case EGE_OBJECT_UID_OVERLAY_TEXT:
      case EGE_OBJECT_UID_OVERLAY_IMAGE:
        {
          Overlay* overlay = ege_cast<Overlay*>((*it).object());
          
          // check if found
          if (overlay->name() == name)
          {
            // found
            return (*it);
          }
        }
        break;
    }
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Widget override. Sets transparency level. */
void UIScrollableView::setAlpha(float32 alpha)
{
  // apply to all objects
  for (ObjectsList::const_iterator it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    // process according to object id
    switch ((*it)->uid())
    {
      case EGE_OBJECT_UID_OVERLAY_TEXT:
      case EGE_OBJECT_UID_OVERLAY_IMAGE:
      {
        Overlay* overlay = ege_cast<Overlay*>((*it).object());
        overlay->renderData()->material()->setDiffuseAlpha(alpha);
      }
        break;
    }
  }

  // call base class
  Widget::setAlpha(alpha);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Reutrns list of all objects. */
UIScrollableView::ObjectsList UIScrollableView::objects() const
{
  return m_objects;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! UIScrollView override. Sets content size. 
    @note This disables auto content size generation.
  */
void UIScrollableView::setContentSize(const Vector2f& size)
{
  // call base class
  UIScrollView::setContentSize(size);
  
  // diable content size automatic determination
  m_autoContentSize = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Enables/disables auto calculation of contect size. */
void UIScrollableView::setAutoContentSize(bool enable)
{
  if (m_autoContentSize != enable)
  {
    m_autoContentSize = enable;

    // if enabled make sure it is updated
    if (enable)
    {
      // invalidate content
      m_dirtyContent = true;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END