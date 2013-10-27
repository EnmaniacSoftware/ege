#include "Application.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Application::Application(int& argc, char** argv) : QApplication(argc, argv)
{
  // setup plugin locations
  QStringList pluginPaths(QString("Plugins"));
  m_pluginManager.setPluginPaths(pluginPaths);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Application::~Application(void)
{
  // unload plugins
  m_pluginManager.unloadPlugins();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Application::initialize()
{
  // load plugins
  bool result = m_pluginManager.loadPlugins();

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
