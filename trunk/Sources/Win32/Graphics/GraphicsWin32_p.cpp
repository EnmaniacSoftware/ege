#include "Core/Graphics/Graphics.h"
#include <EGEDevice.h>

#if EGE_RENDERING_OPENGL_2 || EGE_RENDERING_OPENGLES_1
#include "Win32/Graphics/GraphicsWin32_p.h"
#include "Win32/Graphics/OpenGL/GL 2.0/RenderWindowOGLWin32.h"
#include "Core/Graphics/OpenGL/VertexBufferVAOGL.h"
#include "Core/Graphics/OpenGL/VertexBufferVBOOGL.h"
#endif // EGE_RENDERING_OPENGL_2 || EGE_RENDERING_OPENGLES_1

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(GraphicsPrivate)
EGE_DEFINE_DELETE_OPERATORS(GraphicsPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
GraphicsPrivate::GraphicsPrivate(Graphics* base, const Dictionary& params) : m_base(base)
{
  m_base->registerRenderTarget(ege_new RenderWindowOGLWin32(m_base->app(), params));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
GraphicsPrivate::~GraphicsPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates vertex buffer obejct. */
PVertexBuffer GraphicsPrivate::createVertexBuffer(EGEVertexBuffer::UsageType usage) const
{
  PVertexBuffer vertexBuffer;

  //if (Device::HasRenderCapability(EGEDevice::RENDER_CAPS_VBO))
  //{
  //  vertexBuffer = ege_new VertexBufferVBO(m_base->app());
  //}
  //else
  {
    vertexBuffer = ege_new VertexBufferVA(m_base->app());
  }

  return vertexBuffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

