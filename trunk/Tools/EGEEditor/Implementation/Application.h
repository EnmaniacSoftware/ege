#ifndef EDITOR_APPLICATION_H
#define EDITOR_APPLICATION_H

/*! Application object. */

#include <QApplication>
#include "Plugins/PluginsManager.h"
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

  private:

    /*! Plugin manager object. */
    PluginsManager m_pluginManager;
    /*! Configuration object. */
    Config m_config;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EDITOR_APPLICATION_H
