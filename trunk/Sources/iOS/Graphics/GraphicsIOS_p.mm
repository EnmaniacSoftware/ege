#include "Core/Graphics/Graphics.h"
#include "iOS/Graphics/GraphicsIOS_p.h"

#if EGE_RENDERING_OPENGL_FIXED
 #include "iOS/Graphics/OpenGL/RenderWindowOGLIOS.h"
#endif // EGE_RENDERING_OPENGLES_1

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
  RenderWindowOGLIOS* renderWindow = ege_new RenderWindowOGLIOS(d_func()->app(), d_func()->m_params);
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

  // add to render targets pool
  d_func()->registerRenderTarget(renderWindow);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END