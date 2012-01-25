#include "Core/Application/Application.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Overlay/ImageOverlay.h"
#include "Core/Graphics/Render/Renderer.h"
#include "Core/Resource/ResourceMaterial.h"
#include <EGEResources.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(ImageOverlay)
EGE_DEFINE_DELETE_OPERATORS(ImageOverlay)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImageOverlay::ImageOverlay(Application* app, const String& name, egeObjectDeleteFunc deleteFunc) : Overlay(app, name, EGE_OBJECT_UID_OVERLAY_IMAGE, deleteFunc), 
                                                                                                   m_alignmentOffset(Vector4f::ZERO)
{
  initialize();

  m_material = ege_new Material(app);

  ege_connect(physics(), transformationChanged, this, ImageOverlay::transformationChanged);
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
  PResourceMaterial resource = app()->resourceManager()->resource(RESOURCE_NAME_MATERIAL, materialName());
  if (resource)
  {
    resource->setInstance(m_material);
  }

  m_renderData->setMaterial(m_material);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Overlay override. Renders overlay. */
void ImageOverlay::addForRendering(Renderer* renderer, const Matrix4f& transform)
{
  if (visible())
  {
    Matrix4f worldMatrix;
    Quaternionf orientation = physics()->orientation();
    Vector4f position = physics()->position() - m_alignmentOffset;
    Vector4f scale = physics()->scale();

    Math::CreateMatrix(&worldMatrix, &position, &scale, &orientation);

    renderer->addForRendering(m_renderData, transform * worldMatrix);
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
/*! Overlay override. Sets alignment. */
void ImageOverlay::setAlignment(Alignment align)
{
  if (align != alignment())
  {
    // force alignment update
    transformationChanged();

    // call base class
    Overlay::setAlignment(align);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when physics data has been updated. */
void ImageOverlay::transformationChanged()
{
  // update alignment offset
  Vector2f size(physics()->scale().x, physics()->scale().y);

  m_alignmentOffset.x = 0;
  m_alignmentOffset.y = 0;
  Math::Align(&m_alignmentOffset, &size, ALIGN_TOP_LEFT, alignment());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Overlay override. Initializes object. */
void ImageOverlay::initialize()
{
  // call base class
  Overlay::initialize();

  m_renderData  = RenderObjectFactory::CreateQuadXY(app(), "overlay_" + name(), Vector4f::ZERO, Vector2f::ONE, ALIGN_TOP_LEFT, 
                                                    EGEVertexBuffer::ST_V2_T2, EGEGraphics::RP_MAIN_OVERLAY, EGEGraphics::RPT_TRIANGLE_STRIPS,
                                                    EGEVertexBuffer::UT_STATIC_WRITE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
