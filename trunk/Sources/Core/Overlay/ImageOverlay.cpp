#include "Core/Application/Application.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Overlay/ImageOverlay.h"
#include "Core/Graphics/Render/Renderer.h"
#include "Core/Resource/ResourceMaterial.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(ImageOverlay)
EGE_DEFINE_DELETE_OPERATORS(ImageOverlay)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImageOverlay::ImageOverlay(Application* app, const String& name) : Overlay(app, name, EGEGraphics::RPT_TRIANGLE_STRIPS, 
                                                                           EGE_OBJECT_UID_OVERLAY_IMAGE)
{
  m_material = ege_new Material(app);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImageOverlay::~ImageOverlay()
{
  m_material = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Overlay override. Returns TRUE if object is valid. */
bool ImageOverlay::isValid() const
{
  return Overlay::isValid() && (NULL != m_material);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Overlay override. Updates overlay. */
void ImageOverlay::update(const Time& time)
{
  // call base class
  Overlay::update(time);

  if (visible())
  {
    // check if update is needed
    if (isUpdateNeeded())
    {
      // update render data
      updateRenderData();

      // validate
      validate();
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates render data. */
void ImageOverlay::updateRenderData()
{
  PResourceMaterial resource = app()->resourceManager()->resource("material", materialName());
  if (resource)
  {
    resource->setInstance(m_material);
  }

  Color m_color = Color::WHITE;

  renderComponent()->setMaterial(m_material);

  float32* data = (float32*) renderComponent()->vertexBuffer()->lock(0, 4);

  *data++ = 0;
  *data++ = 1;
  *data++ = 0;
  *data++ = 0;
  *data++ = 1;
  *data++ = m_color.red;
  *data++ = m_color.green;
  *data++ = m_color.blue;
  *data++ = m_color.alpha;

  *data++ = 1;
  *data++ = 1;
  *data++ = 0;
  *data++ = 1;
  *data++ = 1;
  *data++ = m_color.red;
  *data++ = m_color.green;
  *data++ = m_color.blue;
  *data++ = m_color.alpha;

  *data++ = 0;
  *data++ = 0;
  *data++ = 0;
  *data++ = 0;
  *data++ = 0;
  *data++ = m_color.red;
  *data++ = m_color.green;
  *data++ = m_color.blue;
  *data++ = m_color.alpha;

  *data++ = 1;
  *data++ = 0;
  *data++ = 0;
  *data++ = 1;
  *data++ = 0;
  *data++ = m_color.red;
  *data++ = m_color.green;
  *data++ = m_color.blue;
  *data++ = m_color.alpha;

  renderComponent()->vertexBuffer()->unlock();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Overlay override. Renders element. */
void ImageOverlay::render(const Viewport* viewport, Renderer* renderer)
{
  if (visible())
  {
    PRenderComponent renderComponent = this->renderComponent();

    Matrix4f worldMatrix;
    if (NULL != physics())
    {
      Quaternionf orientation = physics()->orientation();
      Vector4f position = physics()->position();
      Vector4f scale = physics()->scale();

      Math::CreateMatrix(&worldMatrix, &position, &scale, &orientation);
    }
    else
    {
      worldMatrix = Matrix4f::IDENTITY;
    }

    renderer->addForRendering(worldMatrix, renderComponent);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets material. */
void ImageOverlay::setMaterialName(const String& name)
{
  if (name != m_materialName)
  {
    m_materialName = name;
    invalidate();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
