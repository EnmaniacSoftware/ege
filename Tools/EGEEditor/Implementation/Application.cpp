#include "Application.h"
#include <QString>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KPluginDirectoryOption = "--plugin-dir";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Application::Application(int& argc, char** argv) : QApplication(argc, argv)
{
  QString pluginPath("Plugins");

  // process arguments
  for(int i = 1; i < argc; )
  {
    // get option (assumed)
    const QString opt = QString::fromLocal8Bit(argv[i]);
    if((KPluginDirectoryOption == opt) && ((i + 1) < argc))
    {
      // get option argument
      const QString arg = QString::fromLocal8Bit(argv[i + 1]);

      // override to pool
      pluginPath = arg;

      // skip over
      i = i + 2;
    }
    else
    {
      // skip
      ++i;
    }
  }

  // setup plugin location
  m_pluginManager.setPluginPath(pluginPath);
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
