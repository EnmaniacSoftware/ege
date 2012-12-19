#include "Core/Graphics/Graphics.h"
#include <EGEDevice.h>

#if EGE_RENDERING_OPENGL_2 || EGE_RENDERING_OPENGLES_1
#include "Airplay/Graphics/GraphicsAirplay_p.h"
#include "Airplay/Graphics/OpenGL/ES 1.0/RenderWindowOGLAirplay.h"
#include "Core/Graphics/OpenGL/VertexBufferVAOGL.h"
#include "Core/Graphics/OpenGL/VertexBufferVBOOGL.h"
#include "Core/Graphics/OpenGL/IndexBufferVAOGL.h"
#include "Core/Graphics/OpenGL/IndexBufferVBOOGL.h"
#endif // EGE_RENDERING_OPENGL_2 || EGE_RENDERING_OPENGLES_1

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(GraphicsPrivate)
EGE_DEFINE_DELETE_OPERATORS(GraphicsPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
GraphicsPrivate::GraphicsPrivate(Graphics* base) : m_base(base)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
GraphicsPrivate::~GraphicsPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult GraphicsPrivate::construct()
{
  // create render window
  RenderWindowOGLAirplay* renderWindow = ege_new RenderWindowOGLAirplay(m_base->app(), m_base->m_params);
  if (NULL == renderWindow)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // add to render targets pool
  m_base->registerRenderTarget(renderWindow);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PVertexBuffer GraphicsPrivate::createVertexBuffer(EGEVertexBuffer::UsageType usage) const
{
  PVertexBuffer buffer;

  if ( ! Device::HasRenderCapability(EGEDevice::RENDER_CAPS_VBO))
  {
    buffer = ege_new VertexBufferVBO(m_base->app(), usage);
//    EGE_PRINT("GraphicsPrivate::createVertexBuffer - VBO %p", buffer.object());
  }
  else
  {
    buffer = ege_new VertexBufferVA(m_base->app(), usage);
  //  EGE_PRINT("GraphicsPrivate::createVertexBuffer - VA %p", buffer.object());
  }

  return buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PIndexBuffer GraphicsPrivate::createIndexBuffer(EGEIndexBuffer::UsageType usage) const
{
  PIndexBuffer buffer;

  if ( ! Device::HasRenderCapability(EGEDevice::RENDER_CAPS_VBO))
  {
    buffer = ege_new IndexBufferVBO(m_base->app(), usage);
   // EGE_PRINT("GraphicsPrivate::createIndexBuffer - VBO %p", buffer.object());
  }
  else
  {
    buffer = ege_new IndexBufferVA(m_base->app(), usage);
    //EGE_PRINT("GraphicsPrivate::createIndexBuffer - VA %p", buffer.object());
  }

  return buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void GraphicsPrivate::initializeWorkThreadRenderingContext()
{
  PRenderWindow renderWindow = d_func()->renderTarget(EGE_PRIMARY_RENDER_TARGET_NAME);
  if (NULL != renderWindow)
  {
    ege_cast<RenderWindowOGLAirplay*>(renderWindow)->initializeWorkThreadRenderingContext();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void GraphicsPrivate::deinitializeWorkThreadRenderingContext()
{
  PRenderWindow renderWindow = d_func()->renderTarget(EGE_PRIMARY_RENDER_TARGET_NAME);
  if (NULL != renderWindow)
  {
    ege_cast<RenderWindowOGLAirplay*>(renderWindow)->deinitializeWorkThreadRenderingContext();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END