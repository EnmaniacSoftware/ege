#include "Core/Graphics/Render/RenderWindow.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderWindow::RenderWindow(Application* app, const ConfigParams& params) : RenderTarget(app, params), m_fullScreen(false)
{
  // decompose param list
  ConfigParams::const_iterator iterTitle = params.find(EGE_RENDER_WINDOW_PARAM_TITLE);
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