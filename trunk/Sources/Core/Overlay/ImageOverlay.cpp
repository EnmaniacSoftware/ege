#include "Core/Application/Application.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Overlay/ImageOverlay.h"
#include "Core/Resource/ResourceMaterial.h"
#include "EGEResources.h"
#include "EGERenderer.h"

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
void ImageOverlay::addForRendering(IRenderer* renderer, const Matrix4f& transform)
{
  if (visible())
  {
    Vector2f pos(physics()->position().x, physics()->position().y);
    Vector2f finalSize(size().x * physics()->scale().x, size().y * physics()->scale().y);

    pos = Math::Align(pos, finalSize, ALIGN_TOP_LEFT, alignment());

    // NOTE: scale factor is a combined value from size and current scale cause render item is created with Vector::ONE
    Matrix4f matrix = Math::CreateMatrix(Vector4f(pos.x, pos.y, physics()->position().z), Vector4f(finalSize.x, finalSize.y, 0), Quaternionf::IDENTITY);

    renderer->addForRendering(m_renderData, transform * matrix);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImageOverlay::initialize()
{
  // call base class
  Overlay::initialize();

  m_renderData  = RenderObjectFactory::CreateQuadXY(app(), "overlay_" + name(), Vector4f::ZERO, Vector2f::ONE, ALIGN_TOP_LEFT, false, false, 
                                                    RenderObjectFactory::VS_V2_T2, EGEGraphics::RP_MAIN_OVERLAY, EGEGraphics::RPT_TRIANGLE_STRIPS,
                                                    NVertexBuffer::UT_STATIC_WRITE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END