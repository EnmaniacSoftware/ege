#include "Core/UI/UIHorizontalScrollbar.h"
#include <EGEOverlay.h>
#include <EGEMath.h>
#include <EGEDebug.h>

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
/*! Creates instance of widget. This method is a registration method for factory. */
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
/*! UIAbstractScrollbar override. Updates render data. */
void UIHorizontalScrollbar::updateRenderData()
{
  // update render data
  float32* data = reinterpret_cast<float32*>(m_renderData->vertexBuffer()->lock(0, 6));
  if (data)
  {
    Vector2f thumbSize(static_cast<float32>(Math::Floor((static_cast<float32>(m_pageSize) / (m_rangeEnd - m_rangeStart + 1)) * size().x)), size().y);

    // top left
    *data++ = static_cast<float32>(m_offset);
    *data++ = 0;

    // bottom left
    *data++ = static_cast<float32>(m_offset);
    *data++ = thumbSize.y;

    // bottom right
    *data++ = m_offset + thumbSize.x;
    *data++ = thumbSize.y;

    // top left
    *data++ = static_cast<float32>(m_offset);
    *data++ = 0;

    // bottom right
    *data++ = m_offset + thumbSize.x;
    *data++ = thumbSize.y;

    // top right
    *data++ = m_offset + thumbSize.x;
    *data++ = 0;
  }

  m_renderData->vertexBuffer()->unlock(data - 1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END