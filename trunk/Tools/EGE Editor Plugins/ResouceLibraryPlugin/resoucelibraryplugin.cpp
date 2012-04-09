#include "resoucelibraryplugin.h"
#include <QtPlugin>
#include <QDebug>
#include <Core.h>
#include <MainWindow.h>
#include "ResourceLibraryWindow.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResouceLibraryPlugin::ResouceLibraryPlugin(QObject* parent) : QObject(parent),
                                                              m_resourceLibraryWindow(NULL)
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

  m_resourceLibraryWindow = new ResourceLibraryWindow(mainWindow);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IPlugin override. Deinitializes plugin. */
void ResouceLibraryPlugin::deinitialize()
{
  qDebug() << Q_FUNC_INFO;

  if (m_resourceLibraryWindow)
  {
    delete m_resourceLibraryWindow;
    m_resourceLibraryWindow = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_EXPORT_PLUGIN("resourcelibrary", ResouceLibraryPlugin, "core")
