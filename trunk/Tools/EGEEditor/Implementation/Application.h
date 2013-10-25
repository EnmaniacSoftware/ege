#ifndef EDITOR_APPLICATION_H
#define EDITOR_APPLICATION_H

/*! Application object. */

#include <QApplication>
#include "Plugins/PluginsManager.h"
#include "Projects/ProjectFactory.h"
#include "MainWindow.h"
#include "ObjectPool.h"
#include "Config.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Application : public QApplication
{
  Q_OBJECT

	public:

    Application(int& argc, char** argv);
   ~Application();

    /*! Initializes the application. */
    bool initialize();
    /*! Returns object pool. */
    ObjectPool& objectPool();

  private:

    /*! Plugin manager object. */
    PluginsManager m_pluginManager;
    /*! Main window object. */
    MainWindow m_mainWindow;
    /*! Object pool. */
    ObjectPool m_objectPool;
    /*! Project factory object. */
    ProjectFactory m_projectFactory;
    /*! Configuration object. */
    Config m_config;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EDITOR_APPLICATION_H
