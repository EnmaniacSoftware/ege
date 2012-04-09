#include <QApplication>
#include "Plugins/PluginsManager.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  // register resources
///*  if (!QResource::registerResource("res/resources.rcc"))
//  {
//    // error!
//    return -1;
//  }
//
//  // allocate main window
//  app = new MainWindow();
//  if (NULL == app)
//  {
//    // error!
//    return -2;
//  }
//
//  // initialize
//  if (!app->initialize())
//  {
//    // error!
//    return -3;
//  }

  PluginsManager pluginManager;
  QStringList pluginPaths;
  pluginPaths << "Plugins";
  pluginManager.setPluginPaths(pluginPaths);
  if (!pluginManager.loadPlugins())
  {
    // error!
    return -1;
  }

  // show main window
//  app->show();

  // run
  int result = a.exec();

  pluginManager.unloadPlugins();

  // clean up
  //delete app;
  //app = NULL;

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------