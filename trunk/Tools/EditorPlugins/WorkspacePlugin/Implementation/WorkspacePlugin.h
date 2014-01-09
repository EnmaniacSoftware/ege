#ifndef WORKSPACE_WORKSPACEPLUGIN_H
#define WORKSPACE_WORKSPACEPLUGIN_H

#include <Plugins/Plugin.h>
#include "WorkspacePlugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class MainWindow;
class Configuration;
class PropertiesWindow;
class QAction;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class WORKSPACEPLUGIN_API WorkspacePlugin : public QObject, public IPlugin
{
  Q_OBJECT
  Q_INTERFACES(IPlugin)

  public:
  
    explicit WorkspacePlugin(QObject* parent = NULL);
    ~WorkspacePlugin();

    /*! @see IPlugin::initialize. */
    bool initialize() override;
    /*! @see IPlugin::deinitialize. */
    void deinitialize() override;

  private:

    /*! Main window. */
    MainWindow* m_mainWindow;
    /*! Configuration window. */
    Configuration* m_configuration;
    /*! Properties window. */
    PropertiesWindow* m_propertiesWindow;
    /*! View menu action. */
    QAction* m_viewAction;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // WORKSPACE_WORKSPACEPLUGIN_H
