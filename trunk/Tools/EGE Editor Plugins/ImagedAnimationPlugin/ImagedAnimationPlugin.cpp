#include "ImagedAnimationPlugin.h"
#include "ImagedAnimationWindow.h"
#include <QtPlugin>
#include <QDebug>
#include <Core.h>
#include <MainWindow.h>
#include <ObjectPool.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImagedAnimationPlugin::ImagedAnimationPlugin(QObject* parent) : QObject(parent),
                                                                m_window(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImagedAnimationPlugin::~ImagedAnimationPlugin()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IPlugin override. Initialized plugin. */
bool ImagedAnimationPlugin::initialize()
{
  Core* core = Core::instance();
  MainWindow* mainWindow = core->mainWindow();

  m_window = new ImagedAnimationWindow();
  if (NULL != m_window)
  {
    mainWindow->addChildWindow(m_window);
  }

  ObjectPool::instance()->addObject(m_window);

  return NULL != m_window;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IPlugin override. Deinitializes plugin. */
void ImagedAnimationPlugin::deinitialize()
{
  Core* core = Core::instance();
  MainWindow* mainWindow = core->mainWindow();

  if (m_window)
  {
    mainWindow->removeChildWindow(m_window);
    delete m_window;
    m_window = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_EXPORT_PLUGIN(ImagedAnimationPlugin)

