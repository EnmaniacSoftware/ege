#ifndef CORE_COREPLUGIN_H
#define CORE_COREPLUGIN_H

#include <Plugins/Plugin.h>
#include "CorePlugin_global.h"

class ProjectFactory;
//class Configuration;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class COREPLUGIN_API CorePlugin : public QObject, public IPlugin
{
  Q_OBJECT
  Q_INTERFACES(IPlugin)

  public:
  
    explicit CorePlugin(QObject* parent = NULL);
    ~CorePlugin();

    /*! @see IPlugin::initialize. */
    bool initialize() override;
    /*! @see IPlugin::deinitialize. */
    void deinitialize() override;

  private:

    /*! Main window. */
    //MainWindow* m_mainWindow;
    /*! Project factory. */
    ProjectFactory* m_projectFactory;
    /*! Configuration. */
   // Configuration* m_configuration;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // CORE_COREPLUGIN_H
