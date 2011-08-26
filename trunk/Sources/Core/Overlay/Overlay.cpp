#include "Core/Overlay/Overlay.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(Overlay)
EGE_DEFINE_DELETE_OPERATORS(Overlay)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Overlay::Overlay(Application* app, const String& name, EGEGraphics::RenderPrimitiveType renderType) : Object(app, EGE_OBJECT_UID_OVERLAY), m_name(name), 
                                                                                                          m_updateNeeded(false), m_visible(true)
{
  initialize(renderType);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Overlay::Overlay(Application* app, const String& name, EGEGraphics::RenderPrimitiveType renderType, u32 uid) : Object(app, uid), m_name(name), 
                                                                                                                   m_updateNeeded(false), m_visible(true)
{
  initialize(renderType);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Overlay::~Overlay()
{
  m_physics = NULL;
  m_render  = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool Overlay::isValid() const
{
  return (NULL != m_physics) && (NULL != m_render);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates overlay. */
void Overlay::update(const Time& time)
{
  EGE_UNUSED(time);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Initializes object. */
void Overlay::initialize(EGEGraphics::RenderPrimitiveType renderType)
{
  m_physics = ege_new PhysicsComponent(app(), "overlay_" + name());
  m_render  = ege_new RenderComponent(app(), "overlay_" + name(), EGEGraphics::RP_MAIN_OVERLAY, renderType);

  // add render buffers
  if (!m_render->vertexBuffer()->setSemantics(EGEVertexBuffer::ST_V3_T2_C4))
  {
    // error!
    m_render  = NULL;
    m_physics = NULL;
  }
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
/*! Renders element. */
void Overlay::render(const Viewport* viewport, Renderer* renderer)
{
  EGE_UNUSED(viewport);
  EGE_UNUSED(renderer);
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
