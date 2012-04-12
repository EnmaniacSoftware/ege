#include "MaterialManagerPlugin.h"
#include "MaterialManagerWindow.h"
#include <Core.h>
#include <MainWindow.h>
#include <QtPlugin>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MaterialManagerPlugin::MaterialManagerPlugin(QObject* parent) : QObject(parent),
                                                                m_window(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MaterialManagerPlugin::~MaterialManagerPlugin()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IPlugin override. Initialized plugin. */
bool MaterialManagerPlugin::initialize()
{
  qDebug() << Q_FUNC_INFO;

  Core* core = Core::instance();
  MainWindow* mainWindow = core->mainWindow();

  m_window = new MaterialManagerWindow();
  if (NULL != m_window)
  {
    mainWindow->addChildWindow(m_window);
  }

  return NULL != m_window;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IPlugin override. Deinitializes plugin. */
void MaterialManagerPlugin::deinitialize()
{
  qDebug() << Q_FUNC_INFO;

  Core* core = Core::instance();
  MainWindow* mainWindow = core->mainWindow();

  if (NULL != m_window)
  {
    mainWindow->removeChildWindow(m_window);
    delete m_window;
    m_window = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_EXPORT_PLUGIN("materialmanager", MaterialManagerPlugin, "core resourcelibrary")

