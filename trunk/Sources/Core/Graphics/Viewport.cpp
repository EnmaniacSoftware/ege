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
void Viewport::setRect(Rectf rect)
{
  m_rect = rect;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Viewport::render()
{
  m_camera->render(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Viewport::setBatchCount(u32 count)
{
  m_batchCount = count;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Viewport::setVertexCount(u32 count)
{
  m_vertexCount = count;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Viewport::setClearColor(const Color& color)
{
  m_clearColor = color;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Viewport::setPolygonMode(PolygonMode mode)
{
  m_polygonMode = mode;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Viewport::setClearBufferTypes(BufferType mask)
{
  m_clearBufferTypes = mask;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Viewport::enableOverlays(bool enable)
{
  m_overlays = enable;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Rectf Viewport::physicalRect() const
{
  return Rectf(rect().x * renderTarget()->physicalWidth(), rect().y * renderTarget()->physicalHeight(), 
               rect().width * renderTarget()->physicalWidth(), rect().height * renderTarget()->physicalHeight());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Rectf Viewport::logicalRect() const
{
  return Rectf(rect().x * renderTarget()->width(), rect().y * renderTarget()->height(), 
               rect().width * renderTarget()->width(), rect().height * renderTarget()->height());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END