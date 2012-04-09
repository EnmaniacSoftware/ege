#include "ImagedAnimationPlugin.h"
#include "ImagedAnimationWindow.h"
#include <QtPlugin>
#include <QDebug>
#include <Core.h>
#include <MainWindow.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImagedAnimationPlugin::ImagedAnimationPlugin(QObject* parent) : QObject(parent),
                                                                m_imagedAnimationWindow(NULL)
{
  qDebug() << Q_FUNC_INFO;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImagedAnimationPlugin::~ImagedAnimationPlugin()
{
  qDebug() << Q_FUNC_INFO;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IPlugin override. Initialized plugin. */
bool ImagedAnimationPlugin::initialize()
{
  qDebug() << Q_FUNC_INFO;

  Core* core = Core::instance();
  MainWindow* mainWindow = core->mainWindow();

  m_imagedAnimationWindow = new ImagedAnimationWindow();

  mainWindow->addChildWindow(m_imagedAnimationWindow);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IPlugin override. Deinitializes plugin. */
void ImagedAnimationPlugin::deinitialize()
{
  qDebug() << Q_FUNC_INFO;

  Core* core = Core::instance();
  MainWindow* mainWindow = core->mainWindow();

  if (m_imagedAnimationWindow)
  {
    mainWindow->removeChildWindow(m_imagedAnimationWindow);
    delete m_imagedAnimationWindow;
    m_imagedAnimationWindow = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_EXPORT_PLUGIN("imagedanimation", ImagedAnimationPlugin, "core resourcelibrary")

