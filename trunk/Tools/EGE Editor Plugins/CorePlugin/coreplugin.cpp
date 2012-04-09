#include "coreplugin.h"
#include "core.h"
#include "MainWindow.h"
#include <QtPlugin>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CorePlugin::CorePlugin(QObject* parent) : QObject(parent)
{
  qDebug() << Q_FUNC_INFO;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CorePlugin::~CorePlugin()
{
  qDebug() << Q_FUNC_INFO;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IPlugin override. Initialized plugin. */
bool CorePlugin::initialize()
{
  qDebug() << Q_FUNC_INFO;
  
  if (Core::instance()->initialize())
  {
    MainWindow* mainWindow = Core::instance()->mainWindow();
    if (mainWindow)
    {
      // initialize main window
      if (!mainWindow->initialize())
      {
        // error!
        return false;
      }

      // show main window
      mainWindow->show();
      return true;
    }
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IPlugin override. Deinitializes plugin. */
void CorePlugin::deinitialize()
{
  qDebug() << Q_FUNC_INFO;

  Core::instance()->deinitialize();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_EXPORT_PLUGIN("core", CorePlugin, "")
