#include "Application.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Application::Application(int& argc, char** argv) : QApplication(argc, argv)
{
  // setup plugin locations
  QStringList pluginPaths;

  // TAGE - fix in repo to use capitialized first letter
  pluginPaths << "Plugins";
  m_pluginManager.setPluginPaths(pluginPaths);

//#if	defined(Q_OS_LINUX)
//	qDebug() << "libraryPaths1" << libraryPaths();
//	setLibraryPaths(QStringList() << applicationDirPath() << libraryPaths() << (applicationDirPath() + QLatin1String("/lib")) );
//	qDebug() << "libraryPaths2" << libraryPaths();
//#elif defined(QT_DEBUG) && BETA != 1
//	addLibraryPath(applicationDirPath());
//#else
//	// tylko na swoje pozwalamy!
//	setLibraryPaths(QStringList() << applicationDirPath());
//#ifdef Q_WS_MAC
//	QDir pluginDir(applicationDirPath());
//	pluginDir.cdUp();
//	pluginDir.cd(QString::fromLatin1("PlugIns"));
//	addLibraryPath(pluginDir.absolutePath());
//#endif
//#endif
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
