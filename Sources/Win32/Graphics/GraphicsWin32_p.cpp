#include "Core/Graphics/Graphics.h"
#include "Core/Graphics/OpenGL/Implementation/Fixed/RenderSystemFixedOGL.h"
#include "Core/Graphics/OpenGL/Implementation/Programmable/RenderSystemProgrammableOGL.h"
#include "Win32/Graphics/GraphicsWin32_p.h"
#include "Win32/Graphics/OpenGL/RenderWindowOGLWin32.h"
#include "EGEDevice.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(GraphicsPrivate)
EGE_DEFINE_DELETE_OPERATORS(GraphicsPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
GraphicsPrivate::GraphicsPrivate(Graphics* base) : m_d(base)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
GraphicsPrivate::~GraphicsPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult GraphicsPrivate::construct()
{
  EGEResult result = EGE_SUCCESS;

  // allocate render window
  RenderWindowOGLWin32* renderWindow = ege_new RenderWindowOGLWin32(d_func()->engine(), d_func()->m_params);
  if (NULL == renderWindow)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  if (EGE_SUCCESS != (result = renderWindow->construct(d_func()->m_params)))
  {
    // error!
    return result;
  }

  // create render system
#if EGE_RENDERING_OPENGL_FIXED 
  d_func()->m_renderSystem = ege_new RenderSystemFixedOGL(d_func()->engine());
#else
  d_func()->m_renderSystem = ege_new RenderSystemProgrammableOGL(d_func()->engine());
#endif // EGE_RENDERING_OPENGL_FIXED
  if (NULL == d_func()->m_renderSystem)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  if (EGE_SUCCESS != (result = d_func()->m_renderSystem->construct()))
  {
    // error!
    return result;
  }

  // add to render targets pool
  d_func()->registerRenderTarget(renderWindow);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END