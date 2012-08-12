#include "Core/Application/Application.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Overlay/ImageOverlay.h"
#include "Core/Graphics/Render/Renderer.h"
#include "Core/Resource/ResourceMaterial.h"
#include <EGEResources.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ImageOverlay)
EGE_DEFINE_DELETE_OPERATORS(ImageOverlay)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImageOverlay::ImageOverlay(Application* app, const String& name, egeObjectDeleteFunc deleteFunc) : Overlay(app, name, EGE_OBJECT_UID_OVERLAY_IMAGE, deleteFunc)
{
  initialize();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImageOverlay::~ImageOverlay()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Overlay override. Renders overlay. */
void ImageOverlay::addForRendering(Renderer* renderer, const Matrix4f& transform)
{
  if (visible())
  {
    Matrix4f matrix = Matrix4f::IDENTITY;
    Vector4f pos = physics()->position();
    Vector4f finalSize = Vector4f(size().x, size().y, 0);
    finalSize.x *= physics()->scale().x;
    finalSize.y *= physics()->scale().y;

    Math::Align(&pos, &finalSize, ALIGN_TOP_LEFT, alignment());

    // NOTE: scale factor is a combined value from size and current scale cause render item is created with Vector::ONE
    Math::CreateMatrix(&matrix, &pos, &finalSize, &Quaternionf::IDENTITY);

    renderer->addForRendering(m_renderData, transform * matrix);
  }
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

EGE_NAMESPACE_END