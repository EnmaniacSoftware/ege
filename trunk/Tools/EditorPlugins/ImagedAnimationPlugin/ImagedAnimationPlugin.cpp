#include "ImagedAnimationPlugin.h"
#include "ImagedAnimationWindow.h"
#include <MainWindow.h>
#include <ObjectPool.h>
#include <QtPlugin>
#include <QDebug>

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
  MainWindow* mainWindow = ObjectPool::Instance()->getObject<MainWindow>();
  if (NULL == mainWindow)
  {
    // error!
    qWarning() << Q_FUNC_INFO << "No MainWindow found";
    return false;
  }

  m_window = new ImagedAnimationWindow();
  if (NULL != m_window)
  {
    mainWindow->addChildWindow(m_window);
  }

  // add to pool
  return ObjectPool::Instance()->addObject(m_window);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IPlugin override. Deinitializes plugin. */
void ImagedAnimationPlugin::deinitialize()
{
  if (m_window)
  {
    MainWindow* mainWindow = ObjectPool::Instance()->getObject<MainWindow>();
    if (mainWindow)
    {
      mainWindow->removeChildWindow(m_window);
    }

    delete m_window;
    m_window = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_EXPORT_PLUGIN(ImagedAnimationPlugin)

