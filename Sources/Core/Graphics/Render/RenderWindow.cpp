#include "Core/Graphics/Render/RenderWindow.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderWindow::RenderWindow(Application* app, const Dictionary& params) : RenderTarget(app, params), 
                                                                         m_fullScreen(false)
{
  // decompose param list
  Dictionary::const_iterator iterTitle = params.find(EGE_RENDER_WINDOW_PARAM_TITLE);
  m_title = (iterTitle != params.end()) ? iterTitle->second : "";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderWindow::~RenderWindow()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! RenderTarget override. Returns target priority. */
RenderTarget::Priority RenderWindow::priority() const
{
  return PRIORITY_RENDER_WINDOW;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! RenderTarget override. Binds render target. */
void RenderWindow::bind()
{
  // do nothing, main window does not need to be bound
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! RenderTarget override. Unbinds render target. */
void RenderWindow::unbind()
{
  // do nothing, main window does not need to be unbound
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END