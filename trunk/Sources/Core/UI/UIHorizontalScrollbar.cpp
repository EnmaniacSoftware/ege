#include "Core/UI/UIHorizontalScrollbar.h"
#include "EGEOverlay.h"
#include "EGEMath.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(UIHorizontalScrollbar)
EGE_DEFINE_DELETE_OPERATORS(UIHorizontalScrollbar)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UIHorizontalScrollbar::UIHorizontalScrollbar(Application* app, const String& name, egeObjectDeleteFunc deleteFunc) 
: UIAbstractScrollbar(app, name, EGE_OBJECT_UID_UI_HORIZONTAL_SCROLLBAR, deleteFunc)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UIHorizontalScrollbar::~UIHorizontalScrollbar()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PWidget UIHorizontalScrollbar::Create(Application* app, const String& name)
{
  // allocate object
  PUIHorizontalScrollbar object =  ege_new UIHorizontalScrollbar(app, name);
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
void UIHorizontalScrollbar::updateRenderData()
{
  const float32 rangeRep = (m_rangeEnd == m_rangeStart) ? 0.0f : (1.0f / (m_rangeEnd - m_rangeStart));

  // set buffer size
  if (m_renderData->vertexBuffer()->setSize(6))
  {
    // update render data
    float32* data = reinterpret_cast<float32*>(m_renderData->vertexBuffer()->lock(0, 6));
    if (NULL != data)
    {
      Vector2f thumbSize(static_cast<float32>(Math::Floor(m_pageSize * rangeRep * size().x)), size().y);
      Vector2f startPos((m_offset - m_rangeStart) * rangeRep * size().x, 0);

      // top left
      *data++ = startPos.x;
      *data++ = startPos.y;

      // bottom left
      *data++ = startPos.x;
      *data++ = startPos.y + thumbSize.y;

      // bottom right
      *data++ = startPos.x + thumbSize.x;
      *data++ = startPos.y + thumbSize.y;

      // top left
      *data++ = startPos.x;
      *data++ = startPos.y;

      // bottom right
      *data++ = startPos.x + thumbSize.x;
      *data++ = startPos.y + thumbSize.y;

      // top right
      *data++ = startPos.x + thumbSize.x;
      *data++ = startPos.y;
    }

    m_renderData->vertexBuffer()->unlock(data - 1);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END