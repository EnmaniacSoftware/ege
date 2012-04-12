#include "resoucelibraryplugin.h"
#include <QtPlugin>
#include <QDebug>
#include <Core.h>
#include <MainWindow.h>
#include <ObjectPool.h>
#include "ResourceLibraryWindow.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResouceLibraryPlugin::ResouceLibraryPlugin(QObject* parent) : QObject(parent),
                                                              m_window(NULL)
{
  qDebug() << Q_FUNC_INFO;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResouceLibraryPlugin::~ResouceLibraryPlugin()
{
  qDebug() << Q_FUNC_INFO;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IPlugin override. Initialized plugin. */
bool ResouceLibraryPlugin::initialize()
{
  qDebug() << Q_FUNC_INFO;

  Core* core = Core::instance();
  MainWindow* mainWindow = core->mainWindow();

  m_window = new ResourceLibraryWindow(mainWindow);
  if (NULL != m_window)
  {
    return ObjectPool::instance()->addObject(m_window);
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IPlugin override. Deinitializes plugin. */
void ResouceLibraryPlugin::deinitialize()
{
  qDebug() << Q_FUNC_INFO;

  if (m_window)
  {
    ObjectPool::instance()->removeObject(m_window);

    delete m_window;
    m_window = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_EXPORT_PLUGIN("resourcelibrary", ResouceLibraryPlugin, "core")
