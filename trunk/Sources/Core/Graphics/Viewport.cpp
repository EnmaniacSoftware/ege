#include "Core/Graphics/Viewport.h"
#include "Core/Graphics/Camera.h"
#include "Core/Graphics/Render/RenderTarget.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Viewport)
EGE_DEFINE_DELETE_OPERATORS(Viewport)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Viewport::Viewport(Application* app, const String& name, PCamera camera, RenderTarget* renderTarget) : Object(app), 
                                                                                                       m_polygonMode(POLYGON_MODE_SOLID), 
                                                                                                       m_clearBufferTypes(BUFFER_TYPE_COLOR), 
                                                                                                       m_vertexCount(0), 
                                                                                                       m_batchCount(0), 
                                                                                                       m_overlays(true), 
  m_renderTarget(renderTarget)
{
  m_name = name;
  m_camera = camera;

  // by default set viewport to full area of render target it is associated with
  setRect(Rectf(0, 0, 1, 1));

  setClearColor(Color::BLUE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Viewport::~Viewport()
{
  m_camera = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets viewport rectangle within render target. */
void Viewport::setRect(Rectf rect)
{
  m_rect = rect;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Renders viewport from associated camera's point of view. */
void Viewport::render()
{
  m_camera->render(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets batch count rendered last frame. */
void Viewport::setBatchCount(u32 count)
{
  m_batchCount = count;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets vertex count rendered last frame. */
void Viewport::setVertexCount(u32 count)
{
  m_vertexCount = count;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets viewport clear color. Can be NULL color if viewport should not be cleared. */
void Viewport::setClearColor(const Color& color)
{
  m_clearColor = color;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets polygon mode for rendered data. */
void Viewport::setPolygonMode(PolygonMode mode)
{
  m_polygonMode = mode;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets buffer types to clear. */
void Viewport::setClearBufferTypes(BufferType mask)
{
  m_clearBufferTypes = mask;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Enables/Disables overlays. */
void Viewport::enableOverlays(bool enable)
{
  m_overlays = enable;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns physical (in pixels) rect within render target. */
Rectf Viewport::physicalRect() const
{
  return Rectf(rect().x * renderTarget()->physicalWidth(), rect().y * renderTarget()->physicalHeight(), 
               rect().width * renderTarget()->physicalWidth(), rect().height * renderTarget()->physicalHeight());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns logical (in pixels) rect within render target. */
Rectf Viewport::logicalRect() const
{
  return Rectf(rect().x * renderTarget()->width(), rect().y * renderTarget()->height(), 
               rect().width * renderTarget()->width(), rect().height * renderTarget()->height());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END