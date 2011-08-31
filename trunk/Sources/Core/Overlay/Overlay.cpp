#include "Core/Overlay/Overlay.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(Overlay)
EGE_DEFINE_DELETE_OPERATORS(Overlay)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Overlay::Overlay(Application* app, const String& name, EGEGraphics::RenderPrimitiveType renderType, egeObjectDeleteFunc deleteFunc) 
: Object(app, EGE_OBJECT_UID_OVERLAY, deleteFunc), m_name(name), m_updateNeeded(false), m_visible(true)
{
  initialize(renderType);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Overlay::Overlay(Application* app, const String& name, EGEGraphics::RenderPrimitiveType renderType, u32 uid, egeObjectDeleteFunc deleteFunc) 
: Object(app, uid, deleteFunc), m_name(name), m_updateNeeded(false), m_visible(true)
{
  initialize(renderType);
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
void Overlay::initialize(EGEGraphics::RenderPrimitiveType renderType)
{
  m_physics     = ege_new PhysicsComponent(app(), "overlay_" + name());
  m_renderData  = RenderObjectFactory::CreateQuadXY(app(), "overlay_" + name(), Vector4f::ZERO, Vector2f::ONE, EGEAlignment::ALIGN_TOP_LEFT, 
                                                    EGEVertexBuffer::ST_V3_T2_C4, EGEGraphics::RP_MAIN_OVERLAY, renderType);
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
