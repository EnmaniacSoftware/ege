#include "Core/Graphics/Graphics.h"
#include "Win32/Graphics/GraphicsWin32_p.h"
#include "Win32/Graphics/OpenGL/GL 2.0/RenderWindowOGLWin32.h"

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

