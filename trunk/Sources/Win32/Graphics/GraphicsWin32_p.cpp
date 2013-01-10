#include "Core/Graphics/Graphics.h"
#include <EGEDevice.h>

#if EGE_RENDERING_OPENGL_2 || EGE_RENDERING_OPENGLES_1
#include "Win32/Graphics/GraphicsWin32_p.h"
#include "Win32/Graphics/OpenGL/GL 2.0/RenderWindowOGLWin32.h"
#endif // EGE_RENDERING_OPENGL_2 || EGE_RENDERING_OPENGLES_1

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
  // create render window
  RenderWindowOGLWin32* renderWindow = ege_new RenderWindowOGLWin32(d_func()->app(), d_func()->m_params);
  if (NULL == renderWindow)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // add to render targets pool
  d_func()->registerRenderTarget(renderWindow);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void GraphicsPrivate::initializeWorkThreadRenderingContext()
{
  PRenderWindow renderWindow = d_func()->renderTarget(EGE_PRIMARY_RENDER_TARGET_NAME);
  if (NULL != renderWindow)
  {
    ege_cast<RenderWindowOGLWin32*>(renderWindow)->initializeWorkThreadRenderingContext();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void GraphicsPrivate::deinitializeWorkThreadRenderingContext()
{
  PRenderWindow renderWindow = d_func()->renderTarget(EGE_PRIMARY_RENDER_TARGET_NAME);
  if (NULL != renderWindow)
  {
    ege_cast<RenderWindowOGLWin32*>(renderWindow)->deinitializeWorkThreadRenderingContext();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END