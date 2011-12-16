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

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(GraphicsPrivate)
EGE_DEFINE_DELETE_OPERATORS(GraphicsPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
GraphicsPrivate::GraphicsPrivate(Graphics* base, const Dictionary& params) : m_base(base)
{
  m_base->registerRenderTarget(ege_new RenderWindowOGLAirplay(m_base->app(), params));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
GraphicsPrivate::~GraphicsPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates vertex buffer obejct. */
PVertexBuffer GraphicsPrivate::createVertexBuffer(EGEVertexBuffer::UsageType usage) const
{
  PVertexBuffer buffer;

  if (Device::HasRenderCapability(EGEDevice::RENDER_CAPS_VBO))
  {
    buffer = ege_new VertexBufferVBO(m_base->app());
  }
  else
  {
    buffer = ege_new VertexBufferVA(m_base->app());
  }

  return buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates index buffer obejct. */
PIndexBuffer GraphicsPrivate::createIndexBuffer(EGEIndexBuffer::UsageType usage) const
{
  PIndexBuffer buffer;

  if (Device::HasRenderCapability(EGEDevice::RENDER_CAPS_VBO))
  {
    buffer = ege_new IndexBufferVBO(m_base->app());
  }
  else
  {
    buffer = ege_new IndexBufferVA(m_base->app());
  }

  return buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
