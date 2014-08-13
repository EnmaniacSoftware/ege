#include "Core/Graphics/Render/RenderWindow.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderWindow::RenderWindow(const Dictionary& params) : RenderTarget(params)
                                                     , m_fullScreen(false)
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
RenderTarget::Priority RenderWindow::priority() const
{
  return PRIORITY_RENDER_WINDOW;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderWindow::bind()
{
  // do nothing, main window does not need to be bound
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderWindow::unbind()
{
  // do nothing, main window does not need to be unbound
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END