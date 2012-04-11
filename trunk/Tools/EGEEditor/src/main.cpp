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

  PluginsManager pluginManager;
  QStringList pluginPaths;
  pluginPaths << "Plugins";
  pluginManager.setPluginPaths(pluginPaths);
  if (!pluginManager.loadPlugins())
  {
    // error!
    return -1;
  }

  // run
  int result = a.exec();

  pluginManager.unloadPlugins();

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------