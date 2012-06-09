#include "Core/UI/UIVerticalScrollbar.h"
#include <EGEOverlay.h>
#include <EGEMath.h>
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(UIVerticalScrollbar)
EGE_DEFINE_DELETE_OPERATORS(UIVerticalScrollbar)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UIVerticalScrollbar::UIVerticalScrollbar(Application* app, const String& name, egeObjectDeleteFunc deleteFunc) 
: UIAbstractScrollbar(app, name, EGE_OBJECT_UID_UI_VERTICAL_SCROLLBAR, deleteFunc)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UIVerticalScrollbar::~UIVerticalScrollbar()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of widget. This method is a registration method for factory. */
PWidget UIVerticalScrollbar::Create(Application* app, const String& name)
{
  // allocate object
  PUIVerticalScrollbar object = ege_new UIVerticalScrollbar(app, name);
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
void UIVerticalScrollbar::updateRenderData()
{
  // update render data
  float32* data = reinterpret_cast<float32*>(m_renderData->vertexBuffer()->lock(0, 6));
  if (data)
  {
    Vector2f thumbSize(size().x, static_cast<float32>(Math::Floor((static_cast<float32>(m_pageSize) / (m_rangeEnd - m_rangeStart + 1))) * size().y));

    // top left
    *data++ = 0;
    *data++ = static_cast<float32>(m_offset);

    // bottom left
    *data++ = 0;
    *data++ = m_offset + thumbSize.y;

    // bottom right
    *data++ = thumbSize.x;
    *data++ = m_offset + thumbSize.y;

    // top left
    *data++ = 0;
    *data++ = static_cast<float32>(m_offset);

    // bottom right
    *data++ = thumbSize.x;
    *data++ = m_offset + thumbSize.y;

    // top right
    *data++ = thumbSize.x;
    *data++ = static_cast<float32>(m_offset);
  }

  m_renderData->vertexBuffer()->unlock(data - 1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END