#include "Core/UI/UIAbstractScrollbar.h"
#include <EGEOverlay.h>
#include <EGEMath.h>
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(UIAbstractScrollbar)
EGE_DEFINE_DELETE_OPERATORS(UIAbstractScrollbar)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UIAbstractScrollbar::UIAbstractScrollbar(Application* app, const String& name, u32 uid, egeObjectDeleteFunc deleteFunc) : Widget(app, name, uid, deleteFunc),
                                                                                                                          m_rangeStart(0),
                                                                                                                          m_rangeEnd(100),
                                                                                                                          m_pageSize(10),
                                                                                                                          m_offset(0),
                                                                                                                          m_state(STATE_HIDDEN),
                                                                                                                          m_fadeDuration(1.0f),
                                                                                                                          m_renderDataInvalid(true)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UIAbstractScrollbar::~UIAbstractScrollbar()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool UIAbstractScrollbar::isFrameless() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool UIAbstractScrollbar::initialize(const Dictionary& params)
{
  // initialize base
  bool error = !Widget::initialize(params);

  return !error;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIAbstractScrollbar::update(const Time& time)
{
  if (!isVisible() || (STATE_HIDDEN == m_state))
  {
    // do nothing
    return;
  }

  // update state time
  m_stateTime += time;

  // process according to state
  switch (m_state)
  {
    case STATE_FADING_OUT:

      // check if done
      if (m_stateTime > m_fadeDuration)
      {
        m_stateTime = m_fadeDuration;
      }

      // update material
      m_renderData->material()->setDiffuseAlpha(1.0f - m_stateTime.seconds() / m_fadeDuration.seconds());

      // check if done
      if (m_stateTime == m_fadeDuration)
      {
        m_state = STATE_HIDDEN;
      }
      break;

    case STATE_FADING_IN:

      // check if done
      if (m_stateTime > m_fadeDuration)
      {
        m_stateTime = m_fadeDuration;
      }

      // update material
      m_renderData->material()->setDiffuseAlpha(m_stateTime.seconds() / m_fadeDuration.seconds());

      // check if done
      if (m_stateTime == m_fadeDuration)
      {
        m_fadeDuration = STATE_SHOWN;
      }
      break;
  }

  // call base class
  Widget::update(time);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult UIAbstractScrollbar::construct()
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
  material->setSrcBlendFactor(EGEGraphics::BF_SRC_ALPHA);
  material->setDstBlendFactor(EGEGraphics::BF_ONE_MINUS_SRC_ALPHA);
  material->setDiffuseColor(Color::NONE);
    
  // create render data
  m_renderData = ege_new RenderComponent(app(), "scrollbar", EGEGraphics::RP_MAIN + 1, EGEGraphics::RPT_TRIANGLES);
  if (NULL == m_renderData)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  m_renderData->setMaterial(material);
  if (!m_renderData->vertexBuffer()->setSemantics(EGEVertexBuffer::ST_V2))
  {
    // error!
    return EGE_ERROR;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIAbstractScrollbar::setRange(s32 from, s32 to)
{
  EGE_ASSERT(from <= to);

  if ((m_rangeStart != from) || (m_rangeEnd != to))
  {
    m_rangeStart = from;
    m_rangeEnd   = to;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIAbstractScrollbar::setPageSize(s32 size)
{
  if ((size != m_pageSize) && (0 < size))
  {
    m_pageSize = size;

    // invalid render data
    m_renderDataInvalid = true;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIAbstractScrollbar::setOffset(s32 offset)
{
  if (m_offset != offset)
  {
    // make sure offset is in range
    m_offset = Math::Bound(offset, m_rangeStart, m_rangeEnd - m_pageSize);

    // invalidate render data
    m_renderDataInvalid = true;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIAbstractScrollbar::addForRendering(Renderer* renderer, const Matrix4f& transform)
{
  if (!isVisible() || (STATE_HIDDEN == m_state))
  {
    // do nothing
    return;
  }

  // check if render data needs update
  if (m_renderDataInvalid)
  {
    updateRenderData();

    // validate
    m_renderDataInvalid = false;
  }

  // add for rendering
  renderer->addForRendering(m_renderData, transform.multiply(m_physics.transformationMatrix()));

  // call base class
  Widget::addForRendering(renderer, transform);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIAbstractScrollbar::startShowing()
{
  // process according to state
  switch (m_state)
  {
    case STATE_HIDDEN:
        
      // reset time
      m_stateTime = 0.0f;

      // set state
      m_state = STATE_FADING_IN;
      break;

    case STATE_FADING_IN:
    case STATE_SHOWN:

      // do nothing
      break;

    case STATE_FADING_OUT:

      // set time
      m_stateTime = m_fadeDuration - m_stateTime;

      // set state
      m_stateTime = STATE_FADING_IN;
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIAbstractScrollbar::startHiding()
{
  // process according to state
  switch (m_state)
  {
    case STATE_SHOWN:
        
      // reset time
      m_stateTime = 0.0f;

      // set state
      m_state = STATE_FADING_OUT;
      break;

    case STATE_FADING_OUT:
    case STATE_HIDDEN:

      // do nothing
      break;

    case STATE_FADING_IN:

      // set time
      m_stateTime = m_fadeDuration - m_stateTime;

      // set state
      m_state = STATE_FADING_OUT;
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END