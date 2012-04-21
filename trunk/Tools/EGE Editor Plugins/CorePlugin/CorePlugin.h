#ifndef COREPLUGIN_H
#define COREPLUGIN_H

#include <Plugins/IPlugin.h>
#include "coreplugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class MainWindow;
class ProjectFactory;
class Configuration;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class COREPLUGIN_API CorePlugin : public QObject, public IPlugin
{
  Q_OBJECT
  Q_INTERFACES(IPlugin)

  public:
  
    explicit CorePlugin(QObject* parent = NULL);
    ~CorePlugin();

    /* IPlugin override. Initialized plugin. */
    bool initialize() override;
    /* IPlugin override. Deinitializes plugin. */
    void deinitialize() override;

  private:

    /*! Main window. */
    MainWindow* m_mainWindow;
    /*! Project factory. */
    ProjectFactory* m_projectFactory;
    /*! Configuration. */
    Configuration* m_configuration;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // COREPLUGIN_H
