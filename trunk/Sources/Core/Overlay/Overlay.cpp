#include "Core/Overlay/Overlay.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Overlay)
EGE_DEFINE_DELETE_OPERATORS(Overlay)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Overlay::Overlay(Application* app, const String& name, egeObjectDeleteFunc deleteFunc) : Object(app, EGE_OBJECT_UID_OVERLAY, deleteFunc), 
                                                                                         m_name(name), 
                                                                                         m_updateNeeded(false), 
                                                                                         m_visible(true)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Overlay::Overlay(Application* app, const String& name, u32 uid, egeObjectDeleteFunc deleteFunc) : Object(app, uid, deleteFunc), 
                                                                                                  m_name(name), 
                                                                                                  m_updateNeeded(false), 
                                                                                                  m_visible(true)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Overlay::~Overlay()
{
  m_physics     = NULL;
  m_renderData  = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool Overlay::isValid() const
{
  return (NULL != m_physics) && (NULL != m_renderData);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates overlay. */
void Overlay::update(const Time& time)
{
  EGE_UNUSED(time);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Initializes object. */
void Overlay::initialize()
{
  m_physics = ege_new PhysicsComponent(app(), "overlay_" + name());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Invalidates object forcing it to be updated next time it's possible. */
void Overlay::invalidate()
{
  m_updateNeeded = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Validates object. */
void Overlay::validate()
{
  m_updateNeeded = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets visibility. */
void Overlay::setVisible(bool set)
{
  if (m_visible != set)
  {
    m_visible = set;

    if (m_visible)
    {
      invalidate();
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets alignment. */
void Overlay::setAlignment(Alignment align)
{
  if (m_alignment != align)
  {
    m_alignment = align;
    invalidate();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Sets transparency level. */
void Overlay::setAlpha(float32 alpha)
{
  if (m_renderData->material())
  {
    m_renderData->material()->setDiffuseAlpha(alpha);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END
