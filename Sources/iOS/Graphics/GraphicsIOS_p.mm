#include "Core/Graphics/Graphics.h"
#include "Core/Graphics/OpenGL/Implementation/Fixed/RenderSystemFixedOGL.h"
#include "Core/Graphics/OpenGL/Implementation/Programmable/RenderSystemProgrammableOGL.h"	
#include "iOS/Graphics/GraphicsIOS_p.h"
#include "iOS/Graphics/OpenGL/RenderWindowOGLIOS.h"

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

  // create render system
#if EGE_RENDERING_OPENGL_FIXED
  d_func()->m_renderSystem = ege_new RenderSystemFixedOGL(d_func()->app());
#else
  d_func()->m_renderSystem = ege_new RenderSystemProgrammableOGL(d_func()->app());
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